using System;

namespace ari
{
	[CRepr]
	struct Node2dHandle
	{	
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MaxValue;
		public void* _obj = null;
		public Entity Owner = null;

		[CLink]
		static extern bool IsValidNode2D(ref uint32 _handle);

		bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidNode2D(ref Handle);
		}
	}

	public class Node2D : Node
	{
		public Vector2* Position;
		public float* Rotation;
		public Vector2* Scale;

		protected Node2dHandle handle;

		// Constructor
		public this(Node2dHandle _handle) : base(_handle._obj)
		{
			this.handle = _handle;
		}

		[CRepr]
		struct Node2dMembers
		{
			public Vector2* Position;
			public float* Rotation;
			public Vector2* Scale;
		}

		[CLink]
		static extern Node2dMembers GetNode2dMembers(void* _node);

		// Init the members
		protected virtual void Init()
		{
			Runtime.Assert(handle._obj != null);
			var m = GetNode2dMembers(handle._obj);
			Position = m.Position;
			Rotation = m.Rotation;
			Scale = m.Scale;
		}
	}
}
