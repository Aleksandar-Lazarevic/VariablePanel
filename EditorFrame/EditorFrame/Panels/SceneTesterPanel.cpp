#include "SceneTesterPanel.h"
#include "Hazel/Scene/Components.h"

#include "Hazel/Scripting/ScriptEngine.h"
#include "Hazel/UI/UI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <Widgets/imgui_scopes.hpp>
#include <Widgets/imgui_utils.h>

#include "../TestScene.h"

#include "../EditorData.h"

SceneTesterPanel::SceneTesterPanel(EditorData &dataSource):m_editor(dataSource)
{
}

void SceneTesterPanel::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(250, 200), ImVec2(FLT_MAX, FLT_MAX));
	auto window = imgui_scopes::Window::Begin("Scene List",nullptr,0);
	if (!window)
		return;

	auto id = m_editor.m_entity;
	auto& registry(m_editor.m_registry);
	auto& scene_list = registry.get<ScenesList>(id);

	auto& scenes(scene_list.m_scenes);

	if (scenes.empty())
	{
		ImGui::Text("No scenes yet");
		return;
	}

	auto listbox = imgui_scopes::ListBox::Begin("##Scenes", imgui_utils::parent_window_content_region());
	if (!listbox)
		return;

	for (int n = 0; n < scenes.size(); n++)
	{
		const bool is_selected = (m_ui.selected == n);
		if (ImGui::Selectable(("Scene" + std::to_string(n)).c_str(), is_selected))
			UI_SetSceneSelection(n);

		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		//if (is_selected)
		//	ImGui::SetItemDefaultFocus();
	}
}

void SceneTesterPanel::UI_SetSceneSelection(int n)
{
	auto id = m_editor.m_entity;
	auto& registry(m_editor.m_registry);
	auto& scenes = registry.get<ScenesList>(id);
	if (scenes.m_scenes.empty())
		return;

	m_ui.selected = std::min(n,(int)scenes.m_scenes.size()-1);
	
	auto& active = registry.get<ActiveScene>(id);
	active.m_scene = scenes.m_scenes[m_ui.selected];
	registry.patch<ActiveScene>(id);
}

