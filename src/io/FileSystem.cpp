#include "FileSystem.hpp"
#include <utility>
#include "FileSystemBase.hpp"
#include "core/containers/Map.hpp"
#include "core/LockScope.hpp"
#include "core/string/StringBuilder.hpp"
#include "URL.hpp"
#include "core/log.h"

namespace ari::io
{
	core::Map<core::String, FileSystemBase*>	g_mFileSystems;
	core::Map<core::String, core::String>		g_mAssigns;
	core::SpinLock								g_lAssigns;

	//------------------------------------------------------------------------------
	void RegisterFileSystem(const core::String& _scheme, FileSystemBase* _fs)
	{
        LOCKSCOPE;
		g_mFileSystems.Add(_scheme, _fs);
	}

	//------------------------------------------------------------------------------
	void AddAssigns(const core::String& _assign, const core::String& _path)
	{
		core::LockScope lock(&g_lAssigns);

		a_assert(_assign.Length() > 2);

		g_mAssigns.Add(_assign, _path);
	}

	//------------------------------------------------------------------------------
	core::String ResolveAssigns(const core::String& _path)
	{
		core::LockScope lock(&g_lAssigns);
		core::StringBuilder builder;
		builder.Set(_path);

		// while there are assigns to replace...
		int index;
		while ((index = builder.FindFirstOf(0, core::EndOfString, ":")) != core::EndOfString) {
			// ignore DOS drive letters
			if (index > 1) {
				core::String assignString = builder.GetSubString(0, index + 1);

				// lookup the assign, ignore unknown assigns, may be URL schemes
				if (g_mAssigns.Contains(assignString)) {

					// replace assign string
					builder.SubstituteFirst(assignString, g_mAssigns[assignString]);
				}
				else break;
			}
			else break;
		}
		core::String result = builder.GetString();
		return result;
	}

	//------------------------------------------------------------------------------
	void LoadFile(core::String _path, std::function<void(core::Buffer*)> OnData)
	{
		LoadFile(_path, std::move(OnData), [_path](IOStatus::Code status)
			{
				log_error("failed to load file %s reasone %s", _path, IOStatus::ToString(status));
			});
	}

	//------------------------------------------------------------------------------
	void LoadFile(const core::String& _path, std::function<void(core::Buffer*)> OnData, std::function<void(IOStatus::Code)> OnFail)
	{
		// get the exact path URL
		URL url(ResolveAssigns(_path));
		
		// find the FileSystem
		if (!g_mFileSystems.Contains(url.Scheme()))
		{
			log_error("Failed to load file %s from url %s.", _path.AsCStr(), url.AsCStr());
			log_error("No file system registered for scheme %s.", url.Scheme().AsCStr());
			return;
		}
		g_mFileSystems[url.Scheme()]->LoadFile(_path, std::move(OnData), std::move(OnFail));
	}

} // namespace ari::io
