/* Copyright (c) 2023 GameMart Inc. */
#pragma once

#include "Node.hpp"
#include <unordered_map>
#include "VariableValue.hpp"

namespace treeview
{
	typedef std::unordered_map<std::string, std::string> ReplacementList;
	class VariableFolder;

	class LogicRuntime
	{
	public:
		virtual ~LogicRuntime()=0;
		virtual void setVariables(const VariableFolder& rootFolder) const=0;
		virtual void setVariables(const variables::Values& values) const = 0;
		virtual void removeVariable(std::string_view name) const=0;
	};

	class VariableFolder : public Node
	{
	public:
		static void updateRuntime(const Node* node);
		static void removeFromRuntime(const Node* node);
		VariableFolder();
		VariableFolder(const VariableFolder& other);
		VariableFolder(LogicRuntime* logic);
		std::string getString() const;
		void setLogicRuntime(LogicRuntime* logic);
		ReplacementList getReplacementList() const;
		virtual bool rename(const std::string& name);
		virtual std::string getName() const;
		virtual std::unique_ptr<Node> allocateCopy() const;
		virtual std::string getJson(bool addReturns) const;
		virtual void setJson(const rapidjson::Value& source);
		virtual std::string getDescription() const;
		virtual ~VariableFolder();
		virtual bool canBeParent() const;
		bool getExclusive() const;
		void setExclusive(bool);
	private:
		virtual void childRemoved();
		virtual void parentChanged();
		std::string _name;
		bool _exclusive;
		LogicRuntime* _currentRuntime{ nullptr };
		VariableFolder& operator=(const VariableFolder& other) = delete;
		VariableFolder& operator=(const VariableFolder&& other) = delete;
	};
}