#include "World.hpp"
#include "System.hpp"
#include "sx/allocator.h"
#include "core/memory/MemoryPool.hpp"

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

static void system_update_job_cb(int _index, void* _userData)
{
	UpdateJobData* data = reinterpret_cast<UpdateJobData*>(_userData);
	data->System->Update(data->World, data->ElaspedTime, data->UpdateState);
}

namespace ari
{
	namespace en
	{
		World::World()
		{
			// Create Job context
			const sx_alloc* alloc = sx_alloc_malloc;
			static sx_job_context_desc cd;
			cd.thread_init_cb = thread_init;
			cd.thread_shutdown_cb = thread_shutdown;
			JobContext = sx_job_create_context(alloc, &cd);
		}

		EntityHandle World::CreateEntity()
		{
			core::MemoryPool<Entity>::Setup(65536);
			uint32_t i;
			uint32_t h = core::HandleManager<EntityHandle>::GetNewHandle(i);
			core::MemoryPool<Entity>::New<Entity>(i);
			return { h , i };
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
					_system->UnConfigure(this);
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
				// Dispatch jobs
				static core::Array<sx_job_desc> 	gameplayJobs,
													sceneJobs,
													frameJobs;
				static core::Array<UpdateJobData>	jobDatas;
				sx_job_t							frameJobHandle;

				gameplayJobs.Clear();
				sceneJobs.Clear();
				frameJobs.Clear();
				jobDatas.Clear();
				int c = 0;

				// 2nd Run frame update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::FrameState))
					{
						jobDatas.Add({m_aSystems[i], this, UpdateState::FrameState, _elapsedTime });
						frameJobs.Add({system_update_job_cb, &jobDatas[c], SX_JOB_PRIORITY_HIGH});
						c++;
					}
				}				
				if (!frameJobs.Empty())
				{
					frameJobHandle = sx_job_dispatch(JobContext, &frameJobs[0], frameJobs.Size());
				}

				// 3rd, Run gameplay update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::GamePlayState))
					{
						jobDatas.Add({m_aSystems[i], this, UpdateState::GamePlayState, _elapsedTime });
						gameplayJobs.Add({system_update_job_cb, &jobDatas[c], SX_JOB_PRIORITY_HIGH});
						c++;
					}
				}				
				if (!gameplayJobs.Empty())
				{
					auto h = sx_job_dispatch(JobContext, &gameplayJobs[0], gameplayJobs.Size());
					sx_job_wait_and_del(JobContext, h);
				}

				// 4th, Run scene update state
				for (int i = 0; i < m_aSystems.Size(); i++)
				{
					if (m_aSystems[i]->NeedUpdateOn(UpdateState::SceneState))
					{
						jobDatas.Add({m_aSystems[i], this, UpdateState::SceneState, _elapsedTime });
						sceneJobs.Add({system_update_job_cb, &jobDatas[c], SX_JOB_PRIORITY_HIGH});
						c++;
					}
				}
				if (!sceneJobs.Empty())
				{
					auto h = sx_job_dispatch(JobContext, &sceneJobs[0], sceneJobs.Size());
					sx_job_wait_and_del(JobContext, h);
				}

				// Wait on frame rendering updates
				sx_job_wait_and_del(JobContext, frameJobHandle);
			}		
		}

	} // en
	
} // ari
