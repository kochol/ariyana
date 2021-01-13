using System;

namespace ari
{
	public class FileSystemLocal : FileSystemBase
	{
		[CLink]
		static extern void* CreateFileSystemLocal();

		public this()
		{
			_obj = CreateFileSystemLocal();
		}

		[CLink]
		static extern void DeleteFileSystemLocal(void* _obj);

		public ~this()
		{
			DeleteFileSystemLocal(_obj);
			_obj = null;
		}
	}
}
