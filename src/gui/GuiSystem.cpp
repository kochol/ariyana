#include "en/World.hpp"
#include "GuiSystem.hpp"
#include "sokol_gfx.h"
#include "imgui.h"

#ifdef ARI_GLFW
#define SOKOL_IMGUI_NO_SOKOL_APP
#else
#include "sokol_app.h"
#endif

#define SOKOL_IMGUI_IMPL
#include "util/sokol_imgui.h"
#include "io/Window.hpp"

namespace ari
{
	namespace gui
	{
		GuiSystem::GuiSystem()
		{
			core::MemoryPool<Gui>::Setup(65536);
		}

		GuiSystem::~GuiSystem()
		{
		}

		void GuiSystem::Configure(World* _world)
		{
			simgui_desc_t desc;
			core::Memory::Fill(&desc, sizeof(simgui_desc_t), 0);
			simgui_setup(&desc);
		}

		void GuiSystem::UnConfigure(World* _world)
		{
			simgui_shutdown();
		}

		void GuiSystem::Update(World* _world, float _elapsed,
			UpdateState::Enum _state)
		{
			core::RectI size = io::GetWindowSize({ 0,0 });
			simgui_new_frame(size.width, size.height, 0);

			_world->GetDerivedComponents<Gui>([this](uint32_t _entity_handle, const ComponentHandle<Gui>& gui)
				{
					this->RenderGui(gui.Component);
				});

			simgui_render();
		}

		bool GuiSystem::NeedUpdateOn(UpdateState::Enum _state)
		{
			return _state == UpdateState::MainThreadState;
		}

		void GuiSystem::RenderGui(Gui * _gui) const
		{
			if (!_gui->Visible)
				return;

			if (_gui->SameLine)
				ImGui::SameLine();

			if (_gui->Separator)
				ImGui::Separator();

			if (_gui->BeginRender())
			{
				for (auto child: _gui->GetChildren(_gui->GetId()))
					this->RenderGui(static_cast<Gui*>(child));
			}
			_gui->EndRender();
		}

	} // gui
	
} // ari
