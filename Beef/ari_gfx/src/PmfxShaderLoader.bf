using System;
using System.Diagnostics;
using System.Threading;
using ari.core;
using ari.gfx.Material;
using ari.io;

namespace ari.gfx
{
	public class PmfxShaderLoader : IResourceLoader, WorkerThread
	{
		private MpScQueue<Shader> shadersToLoad = new MpScQueue<Shader>() ~ delete _;

		private ProcessStartInfo processStartInfo = new ProcessStartInfo() ~ delete _;

		public this(): base("PmfxShaderLoader")
		{
			//processStartInfo.SetFileName()
		}

		public ~this()
		{
		}

		public bool CanLoadResource(String path)
		{
			return path.EndsWith(".pmfx");
		}

		public T LoadResource<T>(String path) where T : Resource
		{
			Shader r = new Shader();
			r.[Friend]state = .Loading;
			r.[Friend]path = path;

			// Add shader to the queue
			shadersToLoad.Push(ref r);

			// Wake up the worker thread 
			ThreadWaitEvent.Set();

			return r;
		}

		public override void OnWorkerUpdate()
		{
			// Thread worker function to load the Shaders
			Shader shader = null;
			while(shadersToLoad.TryPop(ref shader))
			{
				// run the pmfx shader compiler
				
			}
		}
	}
}
