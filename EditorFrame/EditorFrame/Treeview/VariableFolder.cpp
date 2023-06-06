/* Copyright (c) 2023 GameMart Inc. */
#include "VariableFolder.hpp"
#include "VariableNode.hpp"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "string_view_utils.hpp"

using namespace treeview;
using namespace std::string_literals;

VariableFolder::VariableFolder()
{
	_currentRuntime = nullptr;
	_exclusive = false;
}

void VariableFolder::removeFromRuntime(const Node* node)
{
	auto* p = dynamic_cast<const VariableFolder*>(node->getRoot());
	if (p && p->_currentRuntime && node->getParent() == p)
	{
		p->_currentRuntime->removeVariable(node->getName());
	}
}

void VariableFolder::updateRuntime(const Node* node)
{
	auto* r = dynamic_cast<const VariableFolder*>(node->getRoot());
	if (r && r->_currentRuntime)
	{
		r->_currentRuntime->setVariables(*r);
	}
}

VariableFolder::~VariableFolder()
{
}

VariableFolder::VariableFolder(const VariableFolder& other) :Node(other)
{
	_name = other._name;
	_currentRuntime = other._currentRuntime;
	_exclusive = other._exclusive;
}

VariableFolder::VariableFolder(LogicRuntime* logic)
{
	_currentRuntime = logic;
	_exclusive = false;
}

std::unique_ptr<Node> VariableFolder::allocateCopy() const
{
	return std::make_unique<VariableFolder>(*dynamic_cast<const VariableFolder*>(this));
}

//using namespace fsutils;

std::string VariableFolder::getJson(bool addReturns) const
{
	std::string result;

	/// TODO use json writer from rapidjson
	/*if (addReturns)
		result += "\n";

	result += "{\"typeof\":\"variables\"";

	if (_name.size())
	{
		auto escaped = _name;
		stlstring::escapeJson(escaped);
		result += ",\"name\":\""s + escaped + "\""s;
	}

	if (_exclusive)
	{
		result += ",\"exclusive\":true"s;
	}

	if (size())
	{
		result += ",\"children\":[";
		auto t = this->begin();
		result += (*t)->getJson(addReturns);
		++t;
		while (t != this->end())
		{
			result += ",";
			result += (*t)->getJson(addReturns);
			++t;
		}
		result += "]";
	}

	result += "}";*/

	return result;
}

static std::unique_ptr<Node> NodeFromJson(const rapidjson::Value& o)
{
	auto foundType = o.FindMember("typeof");
	if (foundType != o.MemberEnd() && foundType->value.IsString())
	{
		auto s = foundType->value.GetString();
		if (!strcmp(s, "variables"))
		{
			auto variableFolder = std::make_unique<VariableFolder>();
			variableFolder->setJson(o);
			return variableFolder;
		}
		else if (!strcmp(s, "variable"))
		{
			auto variableNode = std::make_unique<VariableNode>();
			variableNode->setJson(o);
			return variableNode;
		}
		
	}
	return {};
}

void VariableFolder::setJson(const rapidjson::Value& o)
{
	auto name = o.FindMember("name");
	if (name != o.MemberEnd())
	{
		rename(name->value.GetString());
	}

	auto exclusive = o.FindMember("exclusive");
	if (exclusive != o.MemberEnd() && exclusive->value.IsBool() && exclusive->value.GetBool())
	{
		setExclusive(true);
	}

	auto children = o.FindMember("children");
	if (children != o.MemberEnd() && children->value.IsArray())
	{
		auto settings = children->value.GetArray();
		for (auto& s : settings)
		{
			if (s.IsObject())
			{
				addChild(NodeFromJson(s));
			}
		}
	}
}

std::string VariableFolder::getDescription() const
{
	return getName();
}

bool VariableFolder::canBeParent() const
{
	return true;
}

void VariableFolder::childRemoved()
{
	updateRuntime(this);
}

std::string VariableFolder::getName() const
{
	return _name;
}

std::string VariableFolder::getString() const
{
	if (!getExclusive())
		return ""s;
	for (auto& c : *this)
	{
		auto isvar = dynamic_cast<const VariableNode*>(c.get());
		if (isvar && isvar->getVis())
			return isvar->getName();
	}
	return "(no value set)";
}

void VariableFolder::setLogicRuntime(LogicRuntime* runtime)
{
	_currentRuntime = runtime;
	if (runtime)
		runtime->setVariables(*this);
}

static std::string ExclusiveChildName(const VariableFolder& folder)
{
	for (auto& it : folder)
	{
		auto isVariable = dynamic_cast<VariableNode*>(it.get());
		if (isVariable && isVariable->getVis())
			return isVariable->getName();
	}
	return "";
}

static void IterateVariables(ReplacementList& result, const std::string_view path, const Node& folder)
{
	for (auto& it : folder)
	{
		auto child = it.get();
		auto isVariable = dynamic_cast<VariableNode*>(child);
		auto isFolder = dynamic_cast<VariableFolder*>(child);
		std::string subPath;
		if (path.size())
			subPath = std::string(path) + "."s + child->getName();
		else
			subPath = child->getName();
		if (isFolder)
		{
			if (isFolder->getExclusive())
			{
				auto exclusiveName = ExclusiveChildName(*isFolder);
				result.insert(std::make_pair("$" + subPath + "$", exclusiveName));
			}
			else
			{
				IterateVariables(result, subPath, *isFolder);
			}
		}
		else if (isVariable)
		{
			auto val = isVariable->getString();
			if (isVariable->hasCSV())
			{
				auto values = svutils::explode(",", val);
				val = values[isVariable->getCSVIndex() % values.size()];
			}
			if (val.size())
				result.insert(std::make_pair("$" + subPath + "$", val));
		}
	}
}

ReplacementList VariableFolder::getReplacementList() const
{
	ReplacementList result;
	IterateVariables(result, "", *this);
	return result;
}

bool VariableFolder::getExclusive() const
{
	return _exclusive;
}

class GenerateJson
{
public:
	GenerateJson(std::string& result) :_result(result)
	{

	}
	void operator()(bool arg)
	{
		_result = "false";
	}
	void operator()(int arg)
	{
		_result = "0";
	}
	void operator()(float arg)
	{
		_result = "0.0";
	}
	void operator()(const std::string& val)
	{
		bool hasPeriod = false;
		bool hasAlpha = false;
		bool hasNumbers = false;
		bool firstChar = true;
		for (auto ch : val)
		{
			if (ch == '.')
			{
				hasPeriod = true;
				firstChar = false;
			}
			else if (ch == '-' && firstChar)
			{
				hasNumbers = true;
				firstChar = false;
			}
			else if ((ch >= '0' && ch <= '9'))
			{
				hasNumbers = true;
				firstChar = false;
			}
			else if (ch == ',')
				firstChar = true;
			else
			{
				firstChar = false;
				hasAlpha = true;
			}
		}
		if (hasAlpha)
			_result = "\"\"";
		else if (hasPeriod)
			_result = "0.0";
		else
			_result = "0";
	}
private:
	std::string& _result;
};

class GenerateType
{
public:
	GenerateType(std::string& result) :_result(result)
	{

	}
	void operator()(bool arg)
	{
		_result = "Bool";
	}
	void operator()(int arg)
	{
		_result = "Int";
	}
	void operator()(float arg)
	{
		_result = "Float";
	}
	void operator()(const std::string& val)
	{
		bool hasPeriod = false;
		bool hasAlpha = false;
		bool hasNumbers = false;
		bool firstChar = true;
		for (auto ch : val)
		{
			if (ch == '.')
			{
				hasPeriod = true;
				firstChar = false;
			}
			else if (ch == '-' && firstChar)
			{
				hasNumbers = true;
				firstChar = false;
			}
			else if ((ch >= '0' && ch <= '9'))
			{
				hasNumbers = true;
				firstChar = false;
			}
			else if (ch == ',')
				firstChar = true;
			else
			{
				firstChar = false;
				hasAlpha = true;
			}
		}
		if (hasAlpha)
			_result = "String";
		else if (hasPeriod)
			_result = "Float";
		else
			_result = "Int";
	}
private:
	std::string& _result;
};

void VariableFolder::setExclusive(bool newExclusive)
{
	_exclusive = newExclusive;
	if (_exclusive)
	{
		if (getParent())
		{
			auto parentPos = getParent()->find(*this);
			for (unsigned position = 0; position < size(); ++position)
			{
				auto variable = dynamic_cast<VariableNode*>(&at(position));
				if (!variable)
				{
					if (move(position, *getParent(), parentPos))
					{
						parentPos++;
						position--;
					}
				}
			}
		}
		if (size())
		{
			bool foundVis = false;
			for (auto& c : *this)
			{
				auto variable = dynamic_cast<VariableNode*>(c.get());
				if (variable)
				{
					if (foundVis)
					{
						variable->setVis(false);
					}
					else if (variable->getVis())
					{
						foundVis = true;
					}
				}
			}
			if (!foundVis)
			{
				auto variable = dynamic_cast<VariableNode*>(&at(0));
				if (variable)
					variable->setVis(true);
			}
		}
	}
	updateRuntime(this);
}

bool VariableFolder::rename(const std::string& name)
{
	if (!name.size())
		return false;
	auto uniqueName = make_name_unique(this, variables::sanitize_for_lua(name), true);
	VariableFolder::removeFromRuntime(this);
	_name = uniqueName;
	updateRuntime(this);
	return true;
}

void VariableFolder::parentChanged()
{
	if (!_name.size())
		return;
	_name = make_name_unique(this, _name, true);
}
