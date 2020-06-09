#include "core.h"
#include "sx/hash.h"
#include "sx/timer.h"
#include "sx/os.h"
#include "core/log.h"

uint32_t HashStringFNV32(char* _str)
{
	return sx_hash_fnv32_str(_str);
}

void ari_os_sleep(int ms)
{
	sx_os_sleep(ms);
}

// log functions
void ari_log_debug(char* _text)
{
	log_debug(_text);
}

// Time functions
void ari_tm_init()
{
	sx_tm_init();
}

uint64_t ari_tm_now()
{
	return sx_tm_now();
}

uint64_t ari_tm_diff(uint64_t new_ticks, uint64_t old_ticks)
{
	return sx_tm_diff(new_ticks, old_ticks);
}

uint64_t ari_tm_since(uint64_t start_ticks)
{
	return sx_tm_since(start_ticks);
}

uint64_t ari_tm_laptime(uint64_t* last_ticks)
{
	return sx_tm_laptime(last_ticks);
}


double ari_tm_sec(uint64_t ticks)
{
	return sx_tm_sec(ticks);
}

double ari_tm_ms(uint64_t ticks)
{
	return sx_tm_ms(ticks);
}

double ari_tm_us(uint64_t ticks)
{
	return sx_tm_us(ticks);
}

double ari_tm_ns(uint64_t ticks)
{
	return sx_tm_ns(ticks);
}
