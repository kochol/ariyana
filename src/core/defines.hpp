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

#define ARI_COMPONENT \
static const uint32_t Id; \
virtual uint32_t GetId() { return Id; } \
virtual uint32_t GetBaseId() { return Id; }

#define ARI_COMPONENT_IMP(_name) \
const uint32_t _name::Id = COMPILE_TIME_CRC32_STR(#_name);

#define ARI_COMPONENT_CHILD(_base) \
static const uint32_t Id; \
virtual uint32_t GetId() override { return Id; } \
virtual uint32_t GetBaseId() override { return _base::Id; }
