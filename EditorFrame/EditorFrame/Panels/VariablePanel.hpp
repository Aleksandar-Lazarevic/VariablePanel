#pragma once

#include <Hazel.h>
#include <optional>
#include <imgui/imgui.h>

namespace treeview
{
	class Node;

}

namespace panels
{
class VariablePanel : public Hazel::Layer
{
public:
	VariablePanel();
	~VariablePanel() override = default;

	void attachDataSource(std::shared_ptr<treeview::Node> newdata);

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

	void ClickExclusiveIcon(treeview::Node* node);
	void ClickOptionValueIcon(treeview::Node* node);
	void ClickDataValueIcon(treeview::Node* node);

private:
	void DrawNode(treeview::Node* node);
	void DrawContextMenu();
	treeview::Node* m_selectedNode;
	treeview::Node* m_renameNode;

	treeview::Node* m_selectedDataNode;
	treeview::Node* m_setDataNode;

	treeview::Node* m_curHoverNode;
	treeview::Node* m_contextMenuNode;
	
	treeview::Node* m_srcMoveNode;
	treeview::Node* m_dstMoveNode;


	ImFont* IconFont;

private:
	std::shared_ptr<treeview::Node> m_treeRoot;
	struct
	{
		bool m_show_window = true;
	}m_guiState;
};
}