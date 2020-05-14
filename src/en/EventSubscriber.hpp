#pragma once

#include "Entity.hpp"
#include "io/IOEvents.hpp"
#include "io/Window.hpp"

namespace ari::en
{
	struct World;
	struct FrameData;
	struct FrameData2D;

	namespace Internal
	{
		class BaseEventSubscriber
		{
		public:

			virtual ~BaseEventSubscriber() = default;
		};
		
	} // Internal

	/**
	* Subclass this as EventSubscriber<EventType> and then call World::Subscribe() in order to Subscribe to events. Make sure
	* to call World::unsubscribe() or World::unsubscribeAll() when your subscriber is deleted!
	*/
	template<typename T>
	class EventSubscriber: public Internal::BaseEventSubscriber
	{
	public:

		virtual ~EventSubscriber() = default;

		/**
		* Called when an event is emitted by the world.
		*/
		virtual void Receive(World* world, const T& event) = 0;

	}; // EventSubscriber


	namespace events
	{
		// Called when a new entity is created.
		struct OnEntityCreated
		{
			EntityHandle entity;
		};

		// Called when an entity is about to be destroyed.
		struct OnEntityDestroyed
		{
			EntityHandle entity;
		};

		// Called when a component is assigned (not necessarily created).
		template <class T>
		struct OnComponentAssigned
		{
			EntityHandle entity;
			T* component;
		};

		// Called when a component is removed
		template <class T>
		struct OnComponentRemoved
		{
			EntityHandle entity;
			T* component;
		};

		struct OnFrameData
		{
			FrameData* frame_data;
		};

		struct OnFrameData2D
		{
			FrameData2D* frame_data_2d;
		};

		struct OnInputEvent
		{
			ari_event* event;
			io::WindowHandle window;
		};

	} // events

} // ari::en
