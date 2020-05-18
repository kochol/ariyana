#include "core.h"
#include "sx/hash.h"

uint32_t HashStringFNV32(char* _str)
{
	return sx_hash_fnv32_str(_str);
}
