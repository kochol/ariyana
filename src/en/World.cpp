#include "World.hpp"
#include "System.hpp"
#include "sx/allocator.h"
#include "sx/jobs.h"
#include "core/memory/MemoryPool.hpp"
#include "ComponentManager.hpp"

ari::TypeIndex ari::TypeRegistry::nextIndex = 0;
ari::core::Map<uint32_t, ari::en::ComponentManager::ComponentData>*	
	ari::en::ComponentManager::m_mComponentsData = nullptr;

static void thread_init(sx_job_context* ctx, int thread_index, uint32_t thread_id, void* user) 
{
}

static void thread_shutdown(sx_job_context* ctx, int thread_index, uint32_t thread_id, void* user) 
{
}

struct UpdateJobData
{
	ari::en::System			*	System;
	ari::en::World			*	World;
	ari::en::UpdateState::Enum	UpdateState;
	float						ElaspedTime;
};

static void system_update_job_cb(int range_start, int range_end, int thread_index, void* _userData)
{
	auto jobDataArray = reinterpret_cast<ari::core::Array<UpdateJobData>*>(_userData);
	for (int i = range_start; i < range_end; i++)
	{
		const auto& data = (*jobDataArray)[i];
		data.System->Update(data.World, data.ElaspedTime, data.UpdateState);
	}
}

namespace ari
{
	namespace en
	{
		World::World()
		{
		}

		EntityHandle World::CreateEntity()
		{
			core::MemoryPool<Entity>::Setup(65536);
			uint32_t i;
			uint32_t h = core::HandleManager<EntityHandle>::GetNewHandle(i);
			core::MemoryPool<Entity>::New<Entity>(i);
			emit<events::OnEntityCreated>({ h , i });
			return { h , i };
		}

		Entity* World::GetEntity(const EntityHandle& _handle)
		{
			return core::MemoryPool<Entity>::GetByIndex(_handle.Index);
		}

		// Removes a component from an entity
		void World::RemoveComponent(const EntityHandle& _entity, const uint32_t& _id)
		{
			m_mEntityComponents[_id].Erase(_entity.Handle);
		}

		//! Add a system to the world
		void World::AddSystem(System* _system)
		{
			m_aSystems.Add(_system);
			_system->Configure(this);
		}

		//! Removes a system from world
		void World::RemoveSystem(System* _system)
		{
			for (int i = 0; i < m_aSystems.Size(); i++)
			{
				if (m_aSystems[i] == _system)
				{
					m_aSystems.EraseSwap(i);
					_system->Unconfigure(this);
					return;
				}
			}
		}

		void World::Update(float _elapsedTime)
		{
			// 1st, Run the main thread update state.
			for (int i = 0; i < m_aSystems.Size(); i++)
			{
				if (m_aSystems[i]->NeedUpdateOn(UpdateState::MainThreadState))
					m_aSystems[i]->Update(this, _elapsedTime, UpdateState::MainThreadState);
			}

			if (UpdateType == UpdateType::Sync)
			{
				// 2nd, Run gameplay update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::GamePlayState))
						m_aSystems[i]->Update(this, _elapsedTime, UpdateState::GamePlayState);
				}

				// 3rd, Run scene update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::SceneState))
						m_aSystems[i]->Update(this, _elapsedTime, UpdateState::SceneState);
				}

				// 4th, Run frame update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::FrameState))
						m_aSystems[i]->Update(this, _elapsedTime, UpdateState::FrameState);
				}				
			}
			else
			{
				if (!JobContext)
				{
					// Create Job context
					const sx_alloc* alloc = sx_alloc_malloc();
					sx_job_context_desc cd;
					core::Memory::Fill(&cd, sizeof(sx_job_context_desc), 0);
					cd.thread_init_cb = thread_init;
					cd.thread_shutdown_cb = thread_shutdown;
					JobContext = sx_job_create_context(alloc, &cd);
				}

				// Dispatch jobs
				static ari::core::Array<UpdateJobData>	 	gameplayJobs,
															sceneJobs,
															frameJobs;
				sx_job_t									frameJobHandle;
				gameplayJobs.Clear();
				sceneJobs.Clear();
				frameJobs.Clear();

				// 2nd Run frame update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::FrameState))
					{
						frameJobs.Add({m_aSystems[i], this, UpdateState::FrameState, _elapsedTime });
					}
				}				
				if (!frameJobs.Empty())
				{
					frameJobHandle = sx_job_dispatch(JobContext, frameJobs.Size(), system_update_job_cb, &frameJobs);
				}

				// 3rd, Run gameplay update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::GamePlayState))
					{
						gameplayJobs.Add({m_aSystems[i], this, UpdateState::GamePlayState, _elapsedTime });
					}
				}				
				if (!gameplayJobs.Empty())
				{
					auto h = sx_job_dispatch(JobContext, gameplayJobs.Size(), system_update_job_cb, &gameplayJobs);
					sx_job_wait_and_del(JobContext, h);
				}

				// 4th, Run scene update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::SceneState))
					{
						sceneJobs.Add({m_aSystems[i], this, UpdateState::SceneState, _elapsedTime });
					}
				}
				if (!sceneJobs.Empty())
				{
					auto h = sx_job_dispatch(JobContext, sceneJobs.Size(), system_update_job_cb, &sceneJobs);
					sx_job_wait_and_del(JobContext, h);
				}

				// Wait on frame rendering updates
				if (!frameJobs.Empty())
					sx_job_wait_and_del(JobContext, frameJobHandle);
			}		
		}

	} // en
	
} // ari
