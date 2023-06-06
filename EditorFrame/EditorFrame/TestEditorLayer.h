#pragma once

#include <Hazel.h>
#include "EditorData.h"

namespace Hazel
{
	class KeyPressedEvent;
	class MouseButtonPressedEvent;
}

class TestScene;
class SceneTesterPanel;
class TypeTesterPanel;
namespace force_panel
{
	class ForcePanel;
}

class TestEditorLayer : public Hazel::Layer
{
public:
	TestEditorLayer(std::shared_ptr<force_panel::ForcePanel> forcePanel);
	virtual ~TestEditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	void ViewportRender();
	virtual void OnImGuiRender() override;
	void UI_Menu();
	void UI_Menu_Create_Force();
	void UI_Menu_Create_Scene();
	void UI_Menu_Create_Demo();
	void OnEvent(Hazel::Event& e) override;
private:
	void OnActiveSceneUpdated(entt::registry& registry, entt::entity entity);
	bool OnKeyPressed(Hazel::KeyPressedEvent& e);
	bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& e);
		
	void OnOverlayRender();
private:
	Hazel::OrthographicCameraController m_CameraController;

	std::shared_ptr<force_panel::ForcePanel> m_forcePanel;

	EditorData m_editor;

	std::shared_ptr<Hazel::Framebuffer> m_Framebuffer;

	std::shared_ptr<Hazel::Texture2D> m_CheckerboardTexture;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];

	float m_color{ 0.0f };
	std::unique_ptr<TypeTesterPanel> m_typeTesterPanel;
	std::unique_ptr<SceneTesterPanel> m_sceneTesterPanel;

	bool m_demo{ true };
};
