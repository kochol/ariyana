using System;

namespace ari.en
{
	extension World
	{
		[CLink]
		static extern Node3dHandle CreateCameraComponent();

		public static Camera CreateCamera()
		{
			var cam = new Camera(CreateCameraComponent());
			cam.[Friend]Init();
			return cam;
		}

		[CLink]
		static extern void AddCameraToWorld(void* _world, ref EntityHandle _entity, Node3dHandle _camera);

		public void AddComponent(Entity _entity, Camera _camera)
		{
			//AddCameraToWorld(_obj, ref _entity.Handle, _camera.[Friend]handle);
			_camera.[Friend]handle.Owner = _entity;
		}

		[CLink]
		static extern Node3dHandle CreateBoxShapeComponent();

		public static BoxShape CreateBoxShape()
		{
			var boxshape = new BoxShape(CreateBoxShapeComponent());
			boxshape.[Friend]Init();
			return boxshape;
		}

		[CLink]
		static extern void AddBoxShapeToWorld(void* _world, ref EntityHandle _entity, Node3dHandle _camera);

		public void AddComponent(Entity _entity, BoxShape _box)
		{
			//AddBoxShapeToWorld(_obj, ref _entity.Handle, _box.[Friend]handle);
			_box.[Friend]handle.Owner = _entity;
		}
	}
}
