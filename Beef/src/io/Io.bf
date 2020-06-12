using System;

namespace ari
{
	[CRepr]
	public struct WindowSetup
	{
		public int32 Width = 640;
		public int32 Height = 480;
		public bool FullScreen = false;
		public bool HighDpi = false;
		public float DpiScale = 1.0f;
	}

	[CRepr]
	public struct WindowHandle
	{
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MinValue;
	}

	public static class Io
	{
		[CLink]
		public static extern void UpdateIo();

		[CLink]
		public static extern bool Run();

		[CLink]
		public static extern WindowHandle CreateAriWindow(ref WindowSetup _setup, char8* _title);

		[CLink]
		public static extern void GetWindowSize(ref WindowHandle _handle, out int width, out int height); 

		[CLink]
		static extern void RegisterFileSystemLink(void* _obj, char8* _scheme);

		public static void RegisterFileSystem(String _scheme, FileSystemBase _fs)
		{
			RegisterFileSystemLink(_fs.[Friend]_obj, _scheme);
		}
	}

}
