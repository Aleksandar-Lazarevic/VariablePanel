#include "TypeTesterPanel.h"
#include "Hazel/Scene/Components.h"

#include "Hazel/Scripting/ScriptEngine.h"
#include "Hazel/UI/UI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <cstring>

#include <Widgets/imgui_scopes.hpp>

#include "../TestScene.h"

#include "../EditorData.h"
#include <entt/entt.hpp>

#include <Widgets/imgui_utils.h>

#include <fmt/format.h>

class EntListener
{
public:
	EntListener(entt::registry& reg, TypeTesterPanel&tester);
	virtual ~EntListener();
	void on_construct(entt::registry&, entt::entity);
	void on_update(entt::registry&, entt::entity);
	entt::registry& m_reg;
private:
	TypeTesterPanel& m_tester;
};

EntListener::EntListener(entt::registry& reg, TypeTesterPanel& tester):m_reg(reg), m_tester(tester)
{
	reg.on_construct<ActiveScene>().connect<&EntListener::on_construct>(this);
	reg.on_update<ActiveScene>().connect< &EntListener::on_update>(this);
}

EntListener::~EntListener()
{
	m_reg.on_construct<ActiveScene>().disconnect<&EntListener::on_construct>(this);
	m_reg.on_update<ActiveScene>().disconnect< &EntListener::on_update>(this);
}

void EntListener::on_construct(entt::registry & registry, entt::entity entity)
{
	auto& scene = registry.get<ActiveScene>(entity);
	m_tester.setContext(scene.m_scene);
}

void EntListener::on_update(entt::registry& registry, entt::entity entity)
{
	on_construct(registry, entity);
}

TypeTesterPanel::TypeTesterPanel(EditorData &editor):m_editor(editor)
{
	m_listener = std::make_unique< EntListener>(editor.m_registry, *this);

	auto& scene = editor.m_registry.get<ActiveScene>(editor.m_entity);
	setContext(scene.m_scene);
}

TypeTesterPanel::~TypeTesterPanel()
{

}

void TypeTesterPanel::setContext(std::weak_ptr<TestScene> newcontext)
{
	m_context = std::move(newcontext);
	auto lock = m_context.lock();
	if (!lock)
		return;

}

void TypeTesterPanel::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(250, 200), ImVec2(FLT_MAX, FLT_MAX));
	auto window = imgui_scopes::Window::Begin("Object List", nullptr, 0);
	if (!window)
		return;

	auto& active_scene = m_editor.m_registry.get<ActiveScene>(m_editor.m_entity);

	auto scene = active_scene.m_scene.lock();

	if (!scene)
	{
		ImGui::Text("No scene yet");
		return;
	}

	auto names = scene->getNames();

	if (names.empty())
	{
		ImGui::Text("No objects yet");
		return;
	}

	auto listbox = imgui_scopes::ListBox::Begin("##Objects", imgui_utils::parent_window_content_region());
	if (!listbox)
		return;

	for (int n = 0; n < names.size(); n++)
	{
		const bool is_selected = (active_scene.m_selected == n);
		auto& name(names[n]);

		ImVec2 backup_pos = ImGui::GetCursorScreenPos();
		if (ImGui::Selectable(fmt::format(FMT_STRING("##Object{}"), n).c_str(), is_selected))
		{
			active_scene.m_selected = n;
			m_editor.m_registry.patch<ActiveScene>(m_editor.m_entity);
		}
		ImGui::SetCursorScreenPos(backup_pos);
		ImGui::TextUnformatted(name.c_str(), name.c_str() + name.length());


		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		//if (is_selected)
		//	ImGui::SetItemDefaultFocus();
	}
}

