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
		static EntityHandle CreateEntity();

		static void DeleteEntity(const EntityHandle& _entity);

		void AddEntity(const EntityHandle& _handle);

		//! Creates a component from object pool
		template<class T>
		static ComponentHandle<T> CreateComponent();

		//! Creates a component from memory pool
		template<class T, class BASE>
		static ComponentHandle<T> CreateComponent();

		// Dispose will destroy component two frame later so every system has two frame time before deleting it.
		template<class T>
		void DisposeComponent(ComponentHandle<T>& _cmp);

		template<class T>
		static void DestroyComponent(const ComponentHandle<T>& _cmp);

		template<class T, class BASE>
		static void DestroyComponent(const ComponentHandle<T>& _cmp);

		//! Add a component to an entity
		template<class T>
		void AddComponent(const EntityHandle& _entity, ComponentHandle<T>& _cmp);

		template<class T, class BASE>
		void AddDerivedComponent(const EntityHandle& _entity, ComponentHandle<T>& _cmp);

		template<class T>
		void RemoveComponent(const EntityHandle& _entity, ComponentHandle<T>& _cmp, bool _dispose = true);

		void* GetComponent(const uint32_t& cmp_id, const uint32_t& cmp_handle);

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
				if (subIndex != core::InvalidIndex)
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

		void AddComponentToDispose(const uint32_t& _id, const uint32_t& base_id, uint32_t& handle, uint32_t& index);

		struct cmp_handle
		{
			uint32_t handle;
			void* cmp;
			Entity* entity;
			bool IsBased = false;
		};

		core::Map<uint32_t /* cmp id */,
			core::Map<uint32_t /* cmp handle */, cmp_handle /* cmp handle */>>
			m_mComponents;

		core::Map<TypeIndex,
			core::Array<Internal::BaseEventSubscriber* >> subscribers;

		core::Array<System*>	m_aSystems;			

		int	m_iTurnIndex = 0;
	};

	template <class T>
	ComponentHandle<T> World::CreateComponent()
	{
		core::ObjectPool<T>::Setup(100);
		uint32_t i;
		const uint32_t h = core::HandleManager<T>::GetNewHandle(i);
		
		return { h, i, core::ObjectPool<T>::New(i) };
	}

	// Creates a component from memory pool
	template<class T, class BASE>
	ComponentHandle<T> World::CreateComponent()
	{
		core::MemoryPool<BASE>::Setup(302400);
		uint32_t i;
		const uint32_t h = core::HandleManager<BASE>::GetNewHandle(i);

		return { h, i, (T*)core::MemoryPool<BASE>::template New<T>(i) };
	}

	template<class T>
	void World::DisposeComponent(ComponentHandle<T>& _cmp)
	{
		AddComponentToDispose(T::Id, _cmp.Component->GetBaseId(), _cmp.Handle, _cmp.Index);
		_cmp.Handle = ari::core::aInvalidHandle;
	}

	template<class T>
	void World::DestroyComponent(const ComponentHandle<T>& _cmp)
	{
		core::HandleManager<T>::RemoveHandle(_cmp.Handle);
		core::ObjectPool<T>::Delete(core::ObjectPool<T>::GetByIndex(_cmp.Index));
	}

	template <class T, class BASE>
	void World::DestroyComponent(const ComponentHandle<T>& _cmp)
	{
		core::HandleManager<BASE>::RemoveHandle(_cmp.Handle);
		core::MemoryPool<BASE>::Delete(core::MemoryPool<BASE>::GetByIndex(_cmp.Index));
	}

	// Add a component to an entity
	template <class T>
	void World::AddComponent(const EntityHandle& _entity, ComponentHandle<T>& _cmp)
	{
		const uint32_t cmpId = T::Id;

		// Add component to the world
		if (!m_mComponents.Contains(cmpId))
			m_mComponents.Add(cmpId, core::Map<uint32_t, cmp_handle>());
		if (!m_mComponents[cmpId].AddUnique(_cmp.Handle, { _cmp.Handle, (void*)_cmp.Component, _entity.entity }))
			m_mComponents[cmpId][_cmp.Handle].entity = _entity.entity;
		
		// Add component to the entity
		if (!_entity->mComponents.Contains(cmpId))
			_entity->mComponents.Add(cmpId, core::Array<Entity::cmp_pair>());
		_entity->mComponents[cmpId].Add({ _cmp.Handle, _cmp.Index, (void*)_cmp.Component, false });

		// Set the component owner
		_cmp.Owner = _entity.entity;

		// Emit to the world
		emit<events::OnComponentAssigned<T>>({ _entity, _cmp.Component });
	}

	template<class T, class BASE>
	void World::AddDerivedComponent(const EntityHandle& _entity, ComponentHandle<T>& _cmp)
	{
		const uint32_t cmpId = BASE::Id;

		// Add component to the world
		if (!m_mComponents.Contains(cmpId))
			m_mComponents.Add(cmpId, core::Map<uint32_t, cmp_handle>());
		if (!m_mComponents[cmpId].AddUnique(_cmp.Handle, { _cmp.Handle, (void*)_cmp.Component, _entity.entity, true }))
			m_mComponents[cmpId][_cmp.Handle].entity = _entity.entity;

		// Add component to the entity
		if (!_entity->mComponents.Contains(cmpId))
			_entity->mComponents.Add(cmpId, core::Array<Entity::cmp_pair>());
		_entity->mComponents[cmpId].Add({ _cmp.Handle, _cmp.Index, (void*)_cmp.Component, true });

		// Also add T class to the list
		AddComponent(_entity, _cmp);
	}

	template<class T>
	void World::RemoveComponent(const EntityHandle& _entity, ComponentHandle<T>& _cmp, bool _dispose)
	{
		_entity.entity->RemoveComponent(_cmp);
		emit<events::OnComponentRemoved<T>>({ _entity, _cmp.Component });
		if (_dispose)
			DisposeComponent(_cmp);
	}

	template<class T, typename Func>
	void World::GetComponents(uint32_t _id, Func _func)
	{
		if (!m_mComponents.Contains(_id))
			return;
		auto& m = m_mComponents[_id];
		for (auto it = m.begin(); it != m.end(); it++)
		{
			const cmp_handle& h = it->value;
			if (h.entity == nullptr)
				continue;
			uint32_t i = core::HandleManager<T>::FindIndex(h);
			ComponentHandle<T> cmp = { h.handle, i, core::ObjectPool<T>::GetByIndex(i), h.entity };
			_func(cmp);
		}
	}

	template<class BASE, typename FUNC>
	void World::GetDerivedComponents(FUNC _func)
	{
		if (!m_mComponents.Contains(BASE::Id))
			return;
		auto& m = m_mComponents[BASE::Id];
		for (auto it = m.begin(); it != m.end(); ++it)
		{
			const cmp_handle& h = it->value;
			if (h.entity == nullptr)
				continue;
			uint32_t i = core::HandleManager<BASE>::FindIndex(h.handle);
			ComponentHandle<BASE> cmp = { h.handle, i, core::MemoryPool<BASE>::GetByIndex(i), h.entity };
			_func(cmp);
		}
	}

} // ari::en
