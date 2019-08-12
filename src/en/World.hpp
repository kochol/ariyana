#pragma once

#include "Entity.hpp"
#include "sx/jobs.h"
#include "ComponentHandle.hpp"
#include "core/containers/Array.hpp"
#include "core/memory/ObjectPool.hpp"
#include "core/memory/MemoryPool.hpp"
#include "EventSubscriber.hpp"
#include "AriTypeId.hpp"

namespace ari::en
{
	class System;

	struct World
	{
		enum class UpdateType
		{
			Sync,
			Async
		};

		UpdateType UpdateType = UpdateType::Async;

		//! Constructor
		World();

		//! Creates a new entity
		EntityHandle CreateEntity();

		Entity* GetEntity(const EntityHandle& _handle);

		void AddEntity(const EntityHandle& _handle);

		//! Creates a component from object pool
		template<class T>
		ComponentHandle<T> CreateComponent();

		//! Creates a component from memory pool
		template<class T, class BASE>
		ComponentHandle<T> CreateComponent();

		template<class T>
		void DestroyComponent(const ComponentHandle<T>& _cmp);

		//! Add a component to an entity
		template<class T>
		void AddComponent(const EntityHandle& _entity, const ComponentHandle<T>& _cmp);

		template<class T, class BASE>
		void AddDerivedComponent(const EntityHandle& _entity, const ComponentHandle<T>& _cmp);

		//! Removes a component from an entity
		void RemoveComponent(const EntityHandle& _entity, const uint32_t& _id);

		//! Add a system to the world
		void AddSystem(System* _system);

		//! Removes a system from world
		void RemoveSystem(System* _system);

		void Update(float _elapsedTime);

		template<class T, typename Func>
		void GetComponents(uint32_t _id, Func _func);

		template<class BASE, typename FUNC>
		void GetDerivedComponents(FUNC _func);

		/**
		* Subscribe to an event.
		*/
		template<typename T>
		void Subscribe(EventSubscriber<T>* subscriber)
		{
			auto index = AriTypeId<T>::GetTypeId();
			const int found = subscribers.FindIndex(index);
			if (found == core::InvalidIndex)
			{
				core::Array<Internal::BaseEventSubscriber*> subList;
				subList.Add(subscriber);

				subscribers.Add(index, subList);
			}
			else
			{
				subscribers.ValueAtIndex(found).Add(subscriber);
			}
		}

		/**
		* Unsubscribe from an event.
		*/
		template<typename T>
		void unsubscribe(EventSubscriber<T>* subscriber)
		{
			auto index = AriTypeId<T>::GetTypeId();
			const int found = subscribers.FindIndex(index);
			if (found != core::InvalidIndex)
			{
				auto& subList = subscribers.ValueAtIndex(found);
				const int subIndex = subList.FindIndexLinear(subscriber, 0, subList.Size());
				subList.EraseSwap(subIndex);
			}
		}

		/**
		* Unsubscribe from all events. Don't be afraid of the void pointer, just pass in your subscriber as normal.
		*/
		void unsubscribeAll(void* subscriber)
		{
			for (auto kv : subscribers)
			{
				const int subIndex = kv.value.FindIndexLinear(
					reinterpret_cast<Internal::BaseEventSubscriber*>(subscriber),
					0, kv.value.Size());
				kv.value.EraseSwap(subIndex);
			}
		}


		/**
		* Emit an event. This will do nothing if there are no subscribers for the event type.
		*/
		template<typename T>
		void emit(const T& event)
		{
			const int found = subscribers.FindIndex(AriTypeId<T>::GetTypeId());
			if (found != core::InvalidIndex)
			{
				for (auto* base : subscribers.ValueAtIndex(found))
				{
					auto* sub = reinterpret_cast<EventSubscriber<T>*>(base);
					sub->Receive(this, event);
				}
			}
		}

		sx_job_context	*	JobContext = nullptr;

	private:

		core::Map<uint32_t /* cmp id */ , 
			core::Map<uint32_t /* entity handle */, uint32_t /* cmp handle */>>
								m_mEntityComponents;

		core::Map<TypeIndex,
			core::Array<Internal::BaseEventSubscriber* >> subscribers;


		core::Array<System*>	m_aSystems;			

	};

	template <class T>
	ComponentHandle<T> World::CreateComponent()
	{
		core::ObjectPool<T>::Setup(100);
		uint32_t i;
		const uint32_t h = core::HandleManager<T>::GetNewHandle(i);
		
		return { h, i, core::ObjectPool<T>::New(i) };
	}

	//! Creates a component from memory pool
	template<class T, class BASE>
	ComponentHandle<T> World::CreateComponent()
	{
		core::MemoryPool<BASE>::Setup(102400);
		uint32_t i;
		const uint32_t h = core::HandleManager<BASE>::GetNewHandle(i);

		return { h, i, (T*)core::MemoryPool<BASE>::template New<T>(i) };
	}

	template<class T>
	void World::DestroyComponent(const ComponentHandle<T>& _cmp)
	{
		core::HandleManager<T>::RemoveHandle(_cmp.Handle);
		core::ObjectPool<T>::Delete(core::ObjectPool<T>::GetByIndex(_cmp.Index));
	}

	// Add a component to an entity
	template <class T>
	void World::AddComponent(const EntityHandle& _entity, const ComponentHandle<T>& _cmp)
	{
		const uint32_t cmpId = _cmp.Component->GetId();
		if (!m_mEntityComponents.Contains(cmpId))
			m_mEntityComponents.Add(cmpId, core::Map<uint32_t, uint32_t>());
		
		m_mEntityComponents[cmpId].Add(_entity.Handle, _cmp.Handle);

		emit<events::OnComponentAssigned<T>>({ _entity, _cmp.Component });
	}

	template<class T, class BASE>
	void World::AddDerivedComponent(const EntityHandle& _entity, const ComponentHandle<T>& _cmp)
	{
		const uint32_t cmpId = BASE::Id;
		if (!m_mEntityComponents.Contains(cmpId))
			m_mEntityComponents.Add(cmpId, core::Map<uint32_t, uint32_t>());

		m_mEntityComponents[cmpId].Add(_entity.Handle, _cmp.Handle);

		// Also add T class to the list
		AddComponent(_entity, _cmp);
	}

	template<class T, typename Func>
	void World::GetComponents(uint32_t _id, Func _func)
	{
		if (!m_mEntityComponents.Contains(_id))
			return;
		auto& m = m_mEntityComponents[_id];
		for (auto it = m.begin(); it != m.end(); it++)
		{
			uint32_t h = it->value;
			uint32_t i = core::HandleManager<T>::FindIndex(h);
			ComponentHandle<T> cmp = { h, i, core::ObjectPool<T>::GetByIndex(i) };
			_func(it->key, cmp);
		}
	}

	template<class BASE, typename FUNC>
	void World::GetDerivedComponents(FUNC _func)
	{
		if (!m_mEntityComponents.Contains(BASE::Id))
			return;
		auto& m = m_mEntityComponents[BASE::Id];
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			uint32_t h = it->value;
			uint32_t i = core::HandleManager<BASE>::FindIndex(h);
			ComponentHandle<BASE> cmp = { h, i, core::MemoryPool<BASE>::GetByIndex(i) };
			_func(it->key, cmp);
		}
	}

} // ari::en
