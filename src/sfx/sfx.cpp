#include "sfx.hpp"
#include "core/log.h"
#include "soloud.h"
#include "en/ComponentManager.hpp"
#include "en/World.hpp"

namespace ari::sfx
{
	SoLoud::Soloud* soloud;
	
	//------------------------------------------------------------------------------
	void InitSfx()
	{
		int res = soloud->init();
		if (res != SoLoud::SO_NO_ERROR)
			log_error("Sfx cannot be initialized");
	}

	//------------------------------------------------------------------------------
	void ShutdownSfx()
	{
		soloud->deinit();
	}

	int Play(AudioSource* audioSource)
	{

	}

	//------------------------------------------------------------------------------

} // namespace ari::sfx

