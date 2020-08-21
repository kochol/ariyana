using System;

namespace ari.io
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

	function void on_file_load(uint8* _data, int32 size, void* _user_data);
	delegate void dOnFileLoad(uint8* _data, int32 size);
	function void on_file_failed(int32 _error_code, void* _user_data);
	delegate void dOnFileFailed(int32 _error_code);

	public static class Io
	{
		[CLink]
		public static extern void UpdateIo();

		[CLink]
		public static extern bool Run();

		[CLink]
		public static extern WindowHandle CreateAriWindow(ref WindowSetup _setup, char8* _title);

		[CLink]
		public static extern void GetWindowSize(ref WindowHandle _handle, out int32 width, out int32 height);

		[CLink]
		public static extern uint32 GetDeviceID();

		[CLink]
		static extern void RegisterFileSystemLink(void* _obj, char8* _scheme);

		public static void RegisterFileSystem(String _scheme, FileSystemBase _fs)
		{
			RegisterFileSystemLink(_fs.[Friend]_obj, _scheme);
		}

		[CLink]
		public static extern void LoadFile (char8* _path, on_file_load _onLoad, void* _load_userdata,
			 on_file_failed _onFailed, void* _failed_userData);

		static void onFileLoad(uint8* _data, int32 size, void* _user_data)
		{
			let fn = (dOnFileLoad)Internal.UnsafeCastToObject(_user_data);
			fn(_data, size);
		}

		static void onFileFailed(int32 _error_code, void* _user_data)
		{
			let fn = (dOnFileFailed)Internal.UnsafeCastToObject(_user_data);
			fn(_error_code);
		}

		// Load a file
		public static void LoadFile(char8* _path, dOnFileLoad _onLoad, dOnFileFailed _onFailed)
		{
			if (_onFailed != null)
			{
				LoadFile(_path, => onFileLoad, Internal.UnsafeCastToPtr(_onLoad),
					=> onFileFailed, Internal.UnsafeCastToPtr(_onFailed));
			}
			else
			{
				LoadFile(_path, => onFileLoad, Internal.UnsafeCastToPtr(_onLoad), null, null);
			}
		}
	}

}
