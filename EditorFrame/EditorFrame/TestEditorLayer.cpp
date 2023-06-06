#include "TestEditorLayer.h"
#include "Hazel/Scene/SceneSerializer.h"
#include "Hazel/Utils/PlatformUtils.h"
#include "Hazel/Math/Math.h"
#include "Hazel/Scripting/ScriptEngine.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imguizmo/ImGuizmo.h>

#include <Hazel/Events/KeyEvent.h>

#include "Panels/TypeTesterPanel.h"
#include "Panels/SceneTesterPanel.h"

#include <Widgets/imgui_scopes.hpp>

#include "TestScene.h"

#include <ForcePanel/ForcePanel.hpp>

#include "TestScene.h"

#include <math.h>

using namespace Hazel;
using namespace force_panel;

static void initialize_editor(EditorData& editor)
{
	auto& reg(editor.m_registry);
	auto main = editor.m_entity = reg.create();
	auto &active = reg.emplace<ActiveScene>(main);
	auto &scenes = reg.emplace<ScenesList>(main);
	scenes.m_scenes.emplace_back(std::make_shared<TestScene>());
	active.m_scene = scenes.m_scenes.back();
}


TestEditorLayer::TestEditorLayer(std::shared_ptr<ForcePanel> forcePanel)
	: Layer("TestEditorLayer"), m_CameraController(1280.0f / 720.0f), m_forcePanel(std::move(forcePanel))
{
	initialize_editor(m_editor);

	m_editor.m_registry.on_update<ActiveScene>().connect<&TestEditorLayer::OnActiveSceneUpdated>(this);

	m_typeTesterPanel = std::make_unique<TypeTesterPanel>(m_editor);
	m_sceneTesterPanel = std::make_unique<SceneTesterPanel>(m_editor);
}

void TestEditorLayer::OnActiveSceneUpdated(entt::registry& registry, entt::entity entity)
{
	auto &active_scene = registry.get<ActiveScene>(entity);
	auto scene = active_scene.m_scene.lock();
	if (!scene)
	{
		m_forcePanel->attachDataSource(nullptr);
		return;
	}

	m_forcePanel->attachDataSource(scene->getObject(active_scene.m_selected));
}

void TestEditorLayer::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CheckerboardTexture = Texture2D::Create("Resources/Textures/Checkerboard.png");

	FramebufferSpecification fbSpec;
	fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Framebuffer::Create(fbSpec);

	
	Renderer2D::SetLineWidth(4.0f);
}

void TestEditorLayer::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void TestEditorLayer::OnUpdate(Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	

	// Resize
	if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
	}

	// Render
	Renderer2D::ResetStats();
	m_Framebuffer->Bind();
	m_color += 0.005f; ;
	RenderCommand::SetClearColor({ (float)sin(m_color)*.3f+.5f, (float)sin(m_color*.73f) * .3f + .5f, (float)sin(m_color*.35f) * .3f + .5f, 1 });
	RenderCommand::Clear();

	// Clear our entity ID attachment to -1
	m_Framebuffer->ClearAttachment(1, -1);

	auto[mx, my] = ImGui::GetMousePos();
	mx -= m_ViewportBounds[0].x;
	my -= m_ViewportBounds[0].y;
	glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
	my = viewportSize.y - my;
	int mouseX = (int)mx;
	int mouseY = (int)my;

	RenderCommand::SetViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

	m_Framebuffer->Unbind();
}

void TestEditorLayer::ViewportRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Example Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


	ImGui::End();
	ImGui::PopStyleVar();
}

void TestEditorLayer::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	UI_Menu();

	ViewportRender();
	m_sceneTesterPanel->OnImGuiRender();
	m_typeTesterPanel->OnImGuiRender();
	
	if(m_demo)
		ImGui::ShowDemoWindow(&m_demo);

	ImGui::End();
}

void TestEditorLayer::UI_Menu_Create_Force()
{
	auto& scene = m_editor.m_registry.get<ActiveScene>(m_editor.m_entity);
	auto locked = scene.m_scene.lock();
	if (!locked)
		return;
	locked->addForce();
	//scenes.m_scenes.push_back({});
	//m_sceneTesterPanel->UI_SetSceneSelection(scenes.m_scenes.size() - 1);
}

void TestEditorLayer::UI_Menu_Create_Scene()
{
	auto id = m_editor.m_entity;
	auto& registry(m_editor.m_registry);
	auto& scenes = registry.get<ScenesList>(id);
	scenes.m_scenes.push_back(std::make_shared< TestScene>());
	registry.patch<ScenesList>(id);

	m_sceneTesterPanel->UI_SetSceneSelection(scenes.m_scenes.size() - 1);
}

void TestEditorLayer::UI_Menu_Create_Demo()
{
	m_demo = true;
}

void TestEditorLayer::UI_Menu()
{
	auto menubar = imgui_scopes::MenuBar::Begin();
	if (!menubar)
		return;

	if (auto file = imgui_scopes::Menu::Begin("File",true))
	{
		if (ImGui::MenuItem("Create Scene"))
			UI_Menu_Create_Scene();

		if (ImGui::MenuItem("Create Demo"))
			UI_Menu_Create_Demo();

		ImGui::Separator();

		if (ImGui::MenuItem("Exit"))
			Application::Get().Close();
	}

	if (auto object = imgui_scopes::Menu::Begin("Object", true))
	{
		if (ImGui::MenuItem("Create force"))
			UI_Menu_Create_Force();
	}
}


void TestEditorLayer::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(TestEditorLayer::OnKeyPressed));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(TestEditorLayer::OnMouseButtonPressed));
}

bool TestEditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
	// Shortcuts
	if (e.IsRepeat())
		return false;

	bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
	bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

	return false;
}

bool TestEditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Mouse::ButtonLeft)
	{
		//if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
			//m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
	}
	return false;
}

void TestEditorLayer::OnOverlayRender()
{

}