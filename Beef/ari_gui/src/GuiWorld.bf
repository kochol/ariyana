using System;
using ari.gui;

namespace ari.en
{
	extension World
	{
		// Window
		[CLink]
		static extern GuiHandle CreateWindowComponent();

		public static Window CreateWindow(String name)
		{
			var window = new Window(CreateWindowComponent(), name);
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
	}
}
