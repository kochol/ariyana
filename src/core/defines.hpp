#pragma once

#include <cstdint>
#include "Types.hpp"
#include "HandleManager.hpp"
#include "StringHash.hpp"

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
static const uint32_t Id = ari::StringHash::StaticHash(#_name); \
virtual uint32_t GetId() { return Id; }
