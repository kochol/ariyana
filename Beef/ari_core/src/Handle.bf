namespace ari.core
{
	public struct Handle
	{
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MaxValue;
		public function bool(ref uint32) IsValidFn = null;

		public bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidFn(ref Handle);
		}

	}
}
