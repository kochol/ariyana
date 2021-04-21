using System;
using ari;
using ari.io;
using ari.en;

namespace t01Init
{
	public class GameApp: Application
	{
		World _world = new World();
		RenderSystem _renderSystem = new RenderSystem();
		SceneSystem _sceneSystem = new SceneSystem();
		FileSystemLocal _fs = new FileSystemLocal();
		EntityHandle _entity;
		BoxShape _box;
		Camera _cam;

		public override void OnInit()
		{
			base.OnInit();
			_world.AddSystem(_renderSystem);
			_world.AddSystem(_sceneSystem);

			Io.RegisterFileSystem("file", _fs);

			_entity = World.CreateEntity();
			_world.AddEntity(ref _entity);

			_cam = World.CreateCamera();
			_cam.Position.x = _cam.Position.y = _cam.Position.z = 3.0f;
			_cam.Target.z = 0.0f;
			//_world.AddComponent(ref _entity, ref _cam);

			_box = World.CreateBoxShape();
			*_box.Texture = Gfx.LoadTexture("res:baboon.png");
			//_world.AddComponent(_entity, _box);
		}

		public override void OnFrame(float _elapsedTime)
		{
			base.OnFrame(_elapsedTime);
			_world.Update(_elapsedTime);
		}

		public override void OnEvent(ari_event* _event, ref WindowHandle _handle)
		{
			base.OnEvent(_event, ref _handle);

		}

		public override void OnCleanup()
		{
			base.OnCleanup();
			delete _fs;
			World.DeleteEntity(ref _entity);
			delete _box;
			delete _cam;
			delete _world;
		}
	}
}
