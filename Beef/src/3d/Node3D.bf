using System;

namespace ari
{
	[CRepr]
	struct Node3dHandle
	{	
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MaxValue;
		public void* _obj = null;
		public Entity Owner = null;

		[CLink]
		static extern bool IsValidNode3D(ref uint32 _handle);

		bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidNode3D(ref Handle);
		}
	}

	public class Node3D	: Node
	{
		public Vector3* Position;
		public Quat* Rotation;
		public Vector3* Scale;

		protected Node3dHandle handle;

		// Constructor
		public this(Node3dHandle _handle) : base(_handle._obj)
		{
			this.handle = _handle;
		}

		[CRepr]
		struct Node3dMembers
		{
			public Vector3* Position;
			public Quat* Rotation;
			public Vector3* Scale;
		}

		[CLink]
		static extern Node3dMembers GetNode3dMembers(void* _node);

		// Init the members
		protected virtual void Init()
		{
			Runtime.Assert(handle._obj != null);
			var m = GetNode3dMembers(handle._obj);
			Position = m.Position;
			Rotation = m.Rotation;
			Scale = m.Scale;
		}
	}
}
