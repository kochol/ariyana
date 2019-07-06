#pragma once

namespace ari::en
{
	struct World;
	struct Entity;
	class Component;
	class FrameData;

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
			Entity* entity;
		};

		// Called when an entity is about to be destroyed.
		struct OnEntityDestroyed
		{
			Entity* entity;
		};

		// Called when a component is assigned (not necessarily created).
		template <class T>
		struct OnComponentAssigned
		{
			Entity* entity;
			T* component;
		};

		// Called when a component is removed
		template <class T>
		struct OnComponentRemoved
		{
			Entity* entity;
			T* component;
		};

		struct OnFrameData
		{
			FrameData* frame_data;
		};

	} // events

} // ari::en
