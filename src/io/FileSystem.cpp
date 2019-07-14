#include "FileSystem.hpp"
#include "FileSystemBase.hpp"
#include "core/containers/Map.hpp"
#include "core/LockScope.hpp"
#include "core/string/StringBuilder.hpp"

namespace ari::io
{
	core::Map<core::String, FileSystemBase*>	g_mFileSystems;
	core::Map<core::String, core::String>		g_mAssigns;
	core::SpinLock								g_lAssigns;

	void RegisterFileSystem(const core::String& _scheme, FileSystemBase* _fs)
	{
        LOCKSCOPE;
		g_mFileSystems.Add(_scheme, _fs);
	}

	void AddAssigns(const core::String& _assign, const core::String& _path)
	{
		core::LockScope lock(&g_lAssigns);

		a_assert(_assign.Length() > 2);

		g_mAssigns.Add(_assign, _path);
	}

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

} // namespace ari::io
