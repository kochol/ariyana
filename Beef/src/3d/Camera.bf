using System;

namespace ari
{

	public class Camera	: Node3D
	{
		public Vector3	*	Target;
		public Vector3	*	Up;
		public Vector3	*	Right;
		public float	*	AspectRatio;
		public float	*	Fov;
		public float	*	xMag;
		public float	*	yMag;
		public float	*	zNear;
		public float	*	zFar;

		public this(Node3dHandle _handle) : base(_handle)
		{
		}

		[CRepr]
		struct CameraMembers
		{
			public Vector3	*	Target;
			public Vector3	*	Up;
			public Vector3	*	Right;
			public float	*	AspectRatio;
			public float	*	Fov;
			public float	*	xMag;
			public float	*	yMag;
			public float	*	zNear;
			public float	*	zFar;
			//CameraType*	Type;
		};

		[CLink]
		static extern CameraMembers GetCameraMembers(void* _node);

		protected override void Init()
		{
			base.Init();
			var m = GetCameraMembers(handle._obj);
			Target = m.Target;
			Up = m.Up;
			Right = m.Right;
			AspectRatio = m.AspectRatio;
			Fov = m.Fov;
			xMag = m.xMag;
			yMag = m.yMag;
			zNear = m.zNear;
			zFar = m.zFar;
		}
	}
}
