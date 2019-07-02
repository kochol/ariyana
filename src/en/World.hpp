#pragma once

#include "Entity.hpp"
#include "sx/jobs.h"
#include "ComponentHandle.hpp"
#include "core/containers/Array.hpp"
#include "core/memory/ObjectPool.hpp"
#include "core/memory/MemoryPool.hpp"

namespace ari
{
	namespace en
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

			sx_job_context	*	JobContext = nullptr;

		private:

			core::Map<uint32_t /* cmp id */ , 
				core::Map<uint32_t /* entity handle */, uint32_t /* cmp handle */>>
									m_mEntityComponents;

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
			core::MemoryPool<BASE>::Setup(4096);
			uint32_t i;
			const uint32_t h = core::HandleManager<BASE>::GetNewHandle(i);

			return { h, i, (T*)core::MemoryPool<BASE>::New<T>(i) };
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
		}

		template<class T, class BASE>
		void World::AddDerivedComponent(const EntityHandle& _entity, const ComponentHandle<T>& _cmp)
		{
			const uint32_t cmpId = BASE::Id;
			if (!m_mEntityComponents.Contains(cmpId))
				m_mEntityComponents.Add(cmpId, core::Map<uint32_t, uint32_t>());

			m_mEntityComponents[cmpId].Add(_entity.Handle, _cmp.Handle);
		}
	
		template<class T, typename Func>
		void World::GetComponents(uint32_t _id, Func _func)
		{
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
			auto& m = m_mEntityComponents[BASE::Id];
			for (auto it = m.begin(); it != m.end(); ++it)
			{
				uint32_t h = it->value;
				uint32_t i = core::HandleManager<BASE>::FindIndex(h);
				ComponentHandle<BASE> cmp = { h, i, core::MemoryPool<BASE>::GetByIndex(i) };
				_func(it->key, cmp);
			}
		}

	} // en
	
} // ari
