using System;
using System.Collections;
using ari.gui;
using ari.io;

namespace ari
{
	public class World
	{
		void* _obj = null;
		List<AriSystem> BeefSystems = new List<AriSystem>() ~ DeleteContainerAndItems!(_);

		// Constructor
		[CLink]
		static extern void* CreateWorld();

		public this()
		{
			_obj = CreateWorld();
		}

		// Destructor
		[CLink]
		static extern void DeleteWorld(void* _obj);

		public ~this()
		{
			DeleteWorld(_obj);
			_obj = null;
		}

		[CLink]
		static extern void UpdateWorld(void* _obj, float _elapsedTime);

		public void Update(float _elapsedTime)
		{
			for (var sys in BeefSystems)
				sys.[Friend]Update(this, _elapsedTime);

			UpdateWorld(_obj, _elapsedTime);
		}

		[CLink]
		static extern void EmitOnInput(void* _world, ari_event* _event, ref WindowHandle _handle);

		// Emit
		public void Emit(ari_event* _event, ref WindowHandle _handle)
		{
			EmitOnInput(_obj, _event, ref _handle);
		}

		// Add system
		[CLink]
		static extern void AddSystemToWorld(void* _world, void* _system);

		public void AddSystem(AriSystem pSystem)
		{
			if (pSystem.[Friend]IsNativeSystem)
				AddSystemToWorld(_obj, pSystem.[Friend]_obj);
			else
				BeefSystems.Add(pSystem);
			pSystem.[Friend]Configure(this);
		}

		[CLink]
		static extern void AddEntityToWorld(void* _world, ref EntityHandle _entity);

		public void AddEntity(Entity _entity)
		{
			AddEntityToWorld(_obj, ref _entity.Handle);
			_entity.[Friend]world = this;
		}

		[CLink]
		static extern EntityHandle CreateEntityWorld();

		[CLink]
		static extern bool* GetEntityMembers(ref EntityHandle _entity);

		public static Entity CreateEntity()
		{
			var e = new [Friend]Entity(CreateEntityWorld());
			e.Replicates = GetEntityMembers(ref e.Handle);
			return e;
		}

		public static T CreateEntity<T>() where T : Entity
		{
			var e = new [Friend]T(CreateEntityWorld());
			e.Replicates = GetEntityMembers(ref e.Handle);
			return e;
		}

		//***************************************
		// 3D components
		//***************************************

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
			AddCameraToWorld(_obj, ref _entity.Handle, _camera.[Friend]handle);
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
			AddBoxShapeToWorld(_obj, ref _entity.Handle, _box.[Friend]handle);
			_box.[Friend]handle.Owner = _entity;
		}

		//***************************************
		// 2D components
		//***************************************

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

		//***************************************
		// Gui components
		//***************************************

		// Window
		[CLink]
		static extern GuiHandle CreateWindowComponent();

		public static Window CreateWindow()
		{
			var window = new Window(CreateWindowComponent());
			window.[Friend]Init();
			return window;
		}

		[CLink]
		static extern void AddWindowToWorld(void* _world, ref EntityHandle _entity, ref GuiHandle _node);

		public void AddComponent(Entity _entity, Window _cmp)
		{
			AddWindowToWorld(_obj, ref _entity.Handle, ref _cmp.Handle);
			_cmp.Handle.Owner = _entity;
		}

		[CLink]
		static extern void RemoveWindowFromWorld(void* _world, ref EntityHandle _entity, ref GuiHandle _node, bool _dispose);

		public void RemoveComponent(Entity _entity, Window _cmp, bool _dispose)
		{
			RemoveWindowFromWorld(_obj, ref _entity.Handle, ref _cmp.Handle, _dispose);
			_cmp.Handle.Owner = null;
			if (_dispose)
				delete _cmp;
		}

		// Script GUI
		[CLink]
		static extern void AddScriptGuiToWorld(void* _world, ref EntityHandle _entity, ref GuiHandle _node);

		public void AddComponent(Entity _entity, ScriptGui _cmp)
		{
			AddScriptGuiToWorld(_obj, ref _entity.Handle, ref _cmp.Handle);
			_cmp.[Friend]_world = this;
			_cmp.Handle.Owner = _entity;
		}

		[CLink]
		static extern void RemoveScriptGuiFromWorld(void* _world, ref EntityHandle _entity, ref GuiHandle _node, bool _dispose);

		public void RemoveComponent(Entity _entity, ScriptGui _cmp, bool _dispose)
		{
			RemoveScriptGuiFromWorld(_obj, ref _entity.Handle, ref _cmp.Handle, _dispose);
			if (_dispose)
				delete _cmp;
			else
			{
				_cmp.Handle.Owner = null;
				_cmp.[Friend]_world = this;
			}
		}

		//***************************************
		// Net components
		//***************************************

		[CLink]
		static extern PropertyReplicatorHandle CreatePropertyReplicatorComponent();

		public static PropertyReplicator CreatePropertyReplicator()
		{
			return new PropertyReplicator(CreatePropertyReplicatorComponent());
		}

		[CLink]
		static extern void AddPropertyReplicatorToWorld(void* _world, ref EntityHandle _entity, ref PropertyReplicatorHandle _cmp);

		public void AddComponent(Entity _entity, PropertyReplicator _pr)
		{
			AddPropertyReplicatorToWorld(_obj, ref _entity.Handle, ref _pr.[Friend]handle);
		}
	}
}
