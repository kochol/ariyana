#pragma once

#include <cstdint>
#include "Types.hpp"
#include "HandleManager.hpp"
#include "string/StringHash.hpp"

#define ARI_HANDLE(_name) \
struct _name \
{ \
    uint32_t Handle = ari::core::aInvalidHandle; \
	uint32_t Index = ari::core::aInvalidHandle; \
	bool IsValid() const\
	{ \
		if (Handle == ari::core::aInvalidHandle || Index == ari::core::aInvalidHandle) \
			return false; \
		return ari::core::HandleManager<_name>::IsHandleValid(Handle); \
	} \
};

#define ARI_COMPONENT(_name) \
static const uint32_t Id; \
static bool IsRegisteredWithComponentManager; \
virtual uint32_t GetId() { return Id; } \
static uint32_t GetBaseId() { return Id; } \
inline static ari::en::ComponentHandleBase CreateComponent(ari::en::World* _world) \
{ \
	auto c = _world->CreateComponent<_name>(); \
	return { c.Handle, c.Index }; \
}

#define ARI_COMPONENT_IMP(_name) \
const uint32_t _name::Id = COMPILE_TIME_CRC32_STR(#_name); \
bool _name::IsRegisteredWithComponentManager = ComponentManager::RegisterComponent<_name>();

#define ARI_COMPONENT_CHILD(_name, _base) \
static const uint32_t Id; \
static bool IsRegisteredWithComponentManager; \
virtual uint32_t GetId() override { return Id; } \
static uint32_t GetBaseId() { return _base::Id; } \
inline static ari::en::ComponentHandleBase CreateComponent(ari::en::World* _world) \
{ \
	auto c = _world->CreateComponent<_name, _base>(); \
	return { c.Handle, c.Index }; \
}

#define ARI_COMPONENT_IMP_CHILD(_name) \
const uint32_t _name::Id = COMPILE_TIME_CRC32_STR(#_name); \
bool _name::IsRegisteredWithComponentManager = ComponentManager::RegisterComponent<_name>();
