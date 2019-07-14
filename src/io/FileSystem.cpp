#include "FileSystem.hpp"
#include "FileSystemBase.hpp"
#include "core/containers/Map.hpp"

namespace ari::io
{
	core::Map<core::String, FileSystemBase*>	g_mFileSystems;
	core::Map<core::String, core::String>		g_mAssigns;

	void RegisterFileSystem(const core::String& _scheme, FileSystemBase* _fs)
	{
		g_mFileSystems.Add(_scheme, _fs);
	}

	void AddAssigns(const core::String& _assign, const core::String& _path)
	{
		g_mAssigns.Add(_assign, _path);
	}

} // namespace ari::io
