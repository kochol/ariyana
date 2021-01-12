using System;

namespace ari.en
{
	extension World
	{
		[CLink]
		static extern Node2dHandle CreateCamera2dComponent();

		public static Camera2D CreateCamera2D()
		{
			var cam2D = new Camera2D(CreateCamera2dComponent());
			cam2D.[Friend]Init();
			return cam2D;
		}

		[CLink]
		static extern void AddCamera2dToWorld(void* _world, ref EntityHandle _entity, ref Node2dHandle _camera);

		public void AddComponent(Entity _entity, Camera2D _cam)
		{
			AddCamera2dToWorld(_obj, ref _entity.Handle, ref _cam.[Friend]handle);
			_cam.[Friend]handle.Owner = _entity;
		}

		[CLink]
		static extern void RemoveCamera2dFromWorld(void* _world, ref EntityHandle _entity, ref Node2dHandle _camera, bool _dispose);

		public void RemoveComponent(Entity _entity, Camera2D _cam, bool _dispose)
		{
			RemoveCamera2dFromWorld(_obj, ref _entity.Handle, ref _cam.[Friend]handle, _dispose);
			_cam.[Friend]handle.Owner = null;
			if (_dispose)
				delete _cam;
		}

		[CLink]
		static extern Node2dHandle CreateSprite2dComponent();

		public static Sprite2D CreateSprite2D()
		{
			var node2d = new Sprite2D(CreateSprite2dComponent());
			node2d.[Friend]Init();
			return node2d;
		}

		[CLink]
		static extern void AddSprite2dToWorld(void* _world, ref EntityHandle _entity, ref Node2dHandle _node2d);

		public void AddComponent(Entity _entity, Sprite2D _cmp)
		{
			AddSprite2dToWorld(_obj, ref _entity.Handle, ref _cmp.[Friend]handle);
			_cmp.[Friend]handle.Owner = _entity;
		}

		[CLink]
		static extern void RemoveSprite2dFromWorld(void* _world, ref EntityHandle _entity, ref Node2dHandle _node, bool _dispose);

		public void RemoveComponent(Entity _entity, ref Sprite2D _cmp, bool _dispose)
		{
			RemoveSprite2dFromWorld(_obj, ref _entity.Handle, ref _cmp.[Friend]handle, _dispose);
			_cmp.[Friend]handle.Owner = null;
			if (_dispose)
			{
				delete _cmp;
				_cmp = null;
			}
		}

		[CLink]
		static extern Node2dHandle CreateCanvasComponent();

		public static Canvas CreateCanvas()
		{
			var node2d = new Canvas(CreateCanvasComponent());
			node2d.[Friend]Init();
			return node2d;
		}

		[CLink]
		static extern void AddCanvasToWorld(void* _world, ref EntityHandle _entity, ref Node2dHandle _node2d);

		public void AddComponent(Entity _entity, Canvas _cmp)
		{
			AddCanvasToWorld(_obj, ref _entity.Handle, ref _cmp.[Friend]handle);
			_cmp.[Friend]handle.Owner = _entity;
		}

		[CLink]
		static extern void RemoveCanvasFromWorld(void* _world, ref EntityHandle _entity, ref Node2dHandle _node, bool _dispose);

		public void RemoveComponent(Entity _entity, Canvas _cmp, bool _dispose)
		{
			RemoveCanvasFromWorld(_obj, ref _entity.Handle, ref _cmp.[Friend]handle, _dispose);
			_cmp.[Friend]handle.Owner = null;
			if (_dispose)
				delete _cmp;
		}
	}
}
