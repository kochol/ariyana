#include "Platform.hpp"
#include <sx/hash.h>

namespace ari::io
{
	uint32_t GetDeviceID()
	{
		static uint32_t dev_id = 0;
		if (dev_id != 0)
			return dev_id;

		dev_id = sx_hash_fnv32_str(GetMacAddress().AsCStr());
		return dev_id;
	}

} // namespace ari::io

