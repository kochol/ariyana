using ari;
using ari.en;
using ari.gui;

namespace projectmanager
{
	class ProjMgrApp: Application
	{
		// Engine stuff
		World world;
		Entity app_entity;
		Window w;
		GuiSystem gui_system;


		public this()
		{
			setup = new GfxSetup();
			setup.window.Width = 800;
			setup.window.Height = 600;
			setup.window.HighDpi = false;
			setup.swap_interval = 1;
		}

		public ~this()
		{
			delete w;
			delete app_entity;

			delete gui_system;

			delete world;
		}

		public override void OnInit()
		{
			// init world
			world = new World();

			// init entities
			app_entity = World.CreateEntity();
			// add components here
			w = World.CreateWindow("test");
			world.AddComponent(app_entity, w);
			
			world.AddEntity(app_entity);

			// init systems
			gui_system = new GuiSystem();
			world.AddSystem(gui_system);
		}

		public override void OnFrame(float _elapsedTime)
		{
			Gfx.BeginDefaultPass();
			world.Update(_elapsedTime);
			Gfx.EndPass();
			Gfx.Commit();
			Gfx.Present();
		}

		public override void OnEvent(ari_event* _event, ref ari.io.WindowHandle _handle)
		{
			world.Emit(_event, ref _handle);
		}

		public override void OnCleanup()
		{
			base.OnCleanup();
		}
	}
}
