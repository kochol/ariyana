#include "sfx.hpp"
#include "soloud.h"
#include "soloud_wav.h"

#include "core/log.h"
#include "core/string/StringBuilder.hpp"
#include "io/FileSystem.hpp"

namespace ari::sfx
{
	
	SoLoud::Soloud* soloud = nullptr;
	

	//------------------------------------------------------------------------------
	void InitSfx()
	{
		SoLoud::Soloud* soloud =
			core::Memory::New<SoLoud::Soloud>();

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
	AudioSourceHandle LoadAudioSource(core::String _path)
	{

		SoLoud::Wav* wave;
		io::LoadFile(_path, [wave](core::Buffer * buffer)
			{
				SoLoud::result result = wave->loadMem(buffer->Data(), buffer->Size());

				if (result != SoLoud::SO_NO_ERROR)
				{
					log_error("Auidsource didn't load successfully!");
					return;
				}
			});

		return { core::HandleManager<AudioSourceHandle>::CreateHandleByIndex(wave->mAudioSourceID), wave->mAudioSourceID };
	}
	// TODO: Play sound by its handle parameter
	//------------------------------------------------------------------------------
	ChannelHandle Play(SoLoud::Wav& _wave)
	{
		uint32_t channelId = soloud->play(_wave);
		return { core::HandleManager<ChannelHandle>::CreateHandleByIndex(channelId), channelId };
	}

	//------------------------------------------------------------------------------
	void Stop(ChannelHandle& _channelHandle)
	{
		soloud->stop(_channelHandle.Index);
	}

	//------------------------------------------------------------------------------


} // namespace ari::sfx

