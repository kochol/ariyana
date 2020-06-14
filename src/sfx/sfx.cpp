#include "sfx.hpp"
#include "soloud.h"

#include "core/log.h"

namespace ari::sfx
{
	SoLoud::Soloud* soloud =
		core::Memory::New<SoLoud::Soloud>();

	

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

	//------------------------------------------------------------------------------
	ChannelHandle Play(SoLoud::Wav& _wave)
	{
		uint32_t channelId = soloud->play(_wave);
		return { core::HandleManager<ChannelHandle>::CreateHandleByIndex(channelId), channelId };
	}

	//------------------------------------------------------------------------------

} // namespace ari::sfx

