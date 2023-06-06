#pragma once

#include <memory>

class TestScene;
class EditorData;
class EntListener;

class TypeTesterPanel
{
public:
	TypeTesterPanel(EditorData&);
	~TypeTesterPanel();
	void OnImGuiRender();
	void setContext(std::weak_ptr<TestScene> newcontext);
private:
	EditorData& m_editor;
	std::weak_ptr<TestScene> m_context;
	std::unique_ptr<EntListener> m_listener;
};
