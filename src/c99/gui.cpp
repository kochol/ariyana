#include "gui.h"
#include "gui/GuiSystem.hpp"
#include "gui/Window.hpp"
#include "cimgui/cimgui.h"
#include "en/ComponentManager.hpp"

// Add a temp function to export cimgui to cari dll
void tempFunc()
{
	igIsAnyItemActive();
}

// System functions
void* CreateGuiSystem()
{
	return ari::core::Memory::New<ari::gui::GuiSystem>();
}

// GUI...
bool IsValidGui(uint32_t& _handle)
{
	return ari::core::HandleManager<ari::gui::Gui>::IsHandleValid(_handle);
}

GuiMembers GetGuiMembers(void* _node)
{
	auto node = reinterpret_cast<ari::gui::Gui*>(_node);
	return { &node->SameLine, &node->Separator, &node->Visible };
}

// Window
GuiHandle CreateWindowComponent()
{
	const union { ari::en::ComponentHandle<ari::gui::Window> cpp; GuiHandle c; } h =
	{ ari::en::World::CreateComponent<ari::gui::Window, ari::gui::Gui>() };
	return h.c;
}

void AddWindowToWorld(void* _world, EntityHandle* _entity, GuiHandle* _node)
{
	union { GuiHandle c{}; ari::en::ComponentHandle<ari::gui::Window> cpp; } node = { *_node };
	const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
	reinterpret_cast<ari::en::World*>(_world)->AddDerivedComponent<ari::gui::Window, ari::gui::Gui>(en.cpp, node.cpp);
}

void RemoveWindowFromWorld(void* _world, EntityHandle* _entity, GuiHandle* _node, bool _dispose)
{
	union { GuiHandle c{}; ari::en::ComponentHandle<ari::gui::Window> cpp; } node = { *_node };
	const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
	reinterpret_cast<ari::en::World*>(_world)->RemoveComponent(en.cpp, node.cpp, _dispose);
}

WindowMembers GetWindowMembers(void* _node)
{
	auto node = reinterpret_cast<ari::gui::Window*>(_node);
	return { &node->Name, &node->CloseButton, &node->isOpen,
		&node->Pos, &node->Size, &node->Flags };
}

// Script GUI
class ScriptGui : public ari::gui::Gui
{
    public:

    ARI_COMPONENT_CHILD(ScriptGui, ari::gui::Gui)

    sgBeginRender* OnBeginRender = nullptr;
    sgEndRender* OnEndRender = nullptr;
    void* UserData = nullptr;

    bool BeginRender() override
    {
		if (UserData)
			return OnBeginRender(UserData);
		return false;
    }

    void EndRender() override
    {
		if (UserData)
			OnEndRender(UserData);
    }
};

ARI_COMPONENT_IMP_CHILD(ScriptGui, ari::gui::Gui)

GuiHandle CreateScriptGuiComponent()
{
	const union { ari::en::ComponentHandle<ScriptGui> cpp; GuiHandle c; } h =
	{ ari::en::World::CreateComponent<ScriptGui, ari::gui::Gui>() };
	return h.c;
}

void AddScriptGuiToWorld(void* _world, EntityHandle* _entity, GuiHandle* _node)
{
	union { GuiHandle c{}; ari::en::ComponentHandle<ScriptGui> cpp; } node = { *_node };
	const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
	reinterpret_cast<ari::en::World*>(_world)->AddDerivedComponent<ScriptGui, ari::gui::Gui>(en.cpp, node.cpp);
}

void RemoveScriptGuiFromWorld(void* _world, EntityHandle* _entity, GuiHandle* _node, bool _dispose)
{
	union { GuiHandle c{}; ari::en::ComponentHandle<ScriptGui> cpp; } node = { *_node };
	const union { EntityHandle c{}; ari::en::EntityHandle cpp; } en = { *_entity };
	reinterpret_cast<ari::en::World*>(_world)->RemoveComponent(en.cpp, node.cpp, _dispose);
}

void InitScriptGuiMembers(void* _node, void* _userData, sgBeginRender* OnBegineRender, sgEndRender* OnEndRender)
{
    auto node = reinterpret_cast<ScriptGui*>(_node);
    node->OnBeginRender = OnBegineRender;
    node->OnEndRender = OnEndRender;
    node->UserData = _userData;
}