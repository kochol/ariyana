#include "FileSystem.hpp"
#include <utility>
#include "FileSystemBase.hpp"
#include "core/containers/Map.hpp"
#include "core/LockScope.hpp"
#include "core/string/StringBuilder.hpp"
#include "URL.hpp"
#include "core/log.h"
#include "sx/jobs.h"
#include "sx/lin-alloc.h"

namespace ari::io
{
	core::Map<core::String, FileSystemBase*>	g_mFileSystems;
	core::Map<core::String, core::String>		g_mAssigns;
	sx_job_context							*	g_job_context = nullptr;

	//------------------------------------------------------------------------------
	void RegisterFileSystem(const core::String& _scheme, FileSystemBase* _fs)
	{
#if ARI_HAS_THREADS
		if (!g_job_context)
		{
			// Create IO job fibers
			sx_job_context_desc desc;
			core::Memory::Fill(&desc, sizeof(sx_job_context_desc), 0);
			desc.num_threads = 2;
			desc.fiber_stack_sz = 65536; // 64 KB
			g_job_context = sx_job_create_context(sx_alloc_malloc, &desc);
		}
#endif
		g_mFileSystems.Add(_scheme, _fs);
	}

	//------------------------------------------------------------------------------
	void AddAssigns(const core::String& _assign, const core::String& _path)
	{
		a_assert(_assign.Length() > 2);

		g_mAssigns.Add(_assign, _path);
	}

	//------------------------------------------------------------------------------
	core::String ResolveAssigns(const core::String& _path)
	{
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
		g_mFileSystems[url.Scheme()]->LoadFile(url, std::move(OnData), std::move(OnFail));
	}

	//------------------------------------------------------------------------------
	void Update()
	{
		for (auto fs : g_mFileSystems)
			fs.Value()->Update();
	}

	//------------------------------------------------------------------------------
	sx_job_context* GetIoJobContext()
	{
		return g_job_context;
	}

} // namespace ari::io
