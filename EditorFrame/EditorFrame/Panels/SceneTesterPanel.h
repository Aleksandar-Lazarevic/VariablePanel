#pragma once

#include <memory>

class TestScene;
class EditorData;

class SceneTesterPanel
{
public:
	SceneTesterPanel(EditorData &);
	void OnImGuiRender();

	void UI_SetSceneSelection(int n);
private:
	
	SceneTesterPanel() = delete;
	EditorData& m_editor;
	struct
	{
		int selected=0;
	}m_ui{};
};
