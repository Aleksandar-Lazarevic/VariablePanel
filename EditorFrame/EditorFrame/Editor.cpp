/* Copyright (c) 2023 GameMart Inc. */
#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "TestEditorLayer.h"
#include "ForcePanel/ForcePanel.hpp"
#include "Panels/VariablePanel.hpp"

#include <entt/entt.hpp>
#include "Treeview/VariableFolder.hpp"
#include "Treeview/VariableNode.hpp"


using namespace Hazel;


std::shared_ptr<treeview::VariableFolder> make_example_variables()
{
	using namespace treeview;
	auto result = std::make_shared<VariableFolder>();
	if (auto* boolnode = result->addChild<VariableNode>("boolnode"))
		boolnode->setValue("true");

	if (auto* stringnode = result->addChild<VariableNode>("stringnode"))
		stringnode->setValue("some random text");

	if (auto* intnode = result->addChild<VariableNode>("intnode"))
		intnode->setValue("1");

	if (auto* fltnode = result->addChild<VariableNode>("fltnode"))
		fltnode->setValue("3.14");

	if (auto* csvnode = result->addChild<VariableNode>("csvnode"))
	{
		csvnode->setValue("a,b,c");
		csvnode->setCSVIndex(1);
	}

	if (auto* foldernode = result->addChild<VariableFolder>())
	{
		foldernode->rename("foldernode");
		if (auto* childvariable = foldernode->addChild<VariableNode>("childvariable"))
			childvariable->setValue("1");
	}

	if (auto* exclusivenode = result->addChild<VariableFolder>())
	{
		exclusivenode->rename("exclusivenode");
		exclusivenode->setExclusive(true);
		if (auto* boolnode = exclusivenode->addChild<VariableNode>("option_1"))
			boolnode->setValue("true");
		if (auto* boolnode = exclusivenode->addChild<VariableNode>("option_2"))
			boolnode->setValue("false");
		if (auto* boolnode = exclusivenode->addChild<VariableNode>("option_3"))
			boolnode->setValue("false");
	}
	return result;
}

class Editor : public Hazel::Application
{
public:
	Editor(const ApplicationSpecification& spec)
		: Application(spec)
	{
		m_forcePanel = std::make_shared<force_panel::ForcePanel>();
		m_variablePanel = std::make_shared<panels::VariablePanel>();

		m_variablesRoot = make_example_variables();

		m_variablePanel->attachDataSource(m_variablesRoot);

		PushLayer(std::make_shared<TestEditorLayer>(m_forcePanel));
		
		
		PushLayer(m_forcePanel);
		PushLayer(m_variablePanel);
	}


private:
	std::shared_ptr<treeview::VariableFolder> m_variablesRoot;
	std::shared_ptr<panels::VariablePanel> m_variablePanel;
	std::shared_ptr<force_panel::ForcePanel> m_forcePanel;
};

Application* Hazel::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Editor";
	spec.CommandLineArgs = args;

	return new Editor(spec);
}