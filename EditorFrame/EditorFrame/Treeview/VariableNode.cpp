/* Copyright (c) 2023 GameMart Inc. */
#include "VariableNode.hpp"
#include "string_view_utils.hpp"
#include <algorithm>
#include "VariableFolder.hpp"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


using namespace treeview;

VariableNode::VariableNode()
{
	_name = "";
	_value = false;
	_isCSV = false;
}

VariableNode::~VariableNode()
{
}

VariableNode::VariableNode(std::string_view name)
{
	_name = name;
	_value = false;
	_isCSV = false;
}

VariableNode::VariableNode(const VariableNode& other) :Node(other)
{
	_value = other._value;
	_name = other._name;
	_isCSV = other._isCSV;
	_csvIndex = other._csvIndex;
}

std::string VariableNode::getName() const
{
	return _name;
}

void VariableNode::setValue(const std::string_view newvalue)
{
	_value = variables::string_to_value(newvalue);
	if (std::find_if(newvalue.begin(), newvalue.end(), [](auto ch) {return ch == ','; }) != newvalue.end())
	{
		_isCSV = true;
		_csvIndex = 0;
	}
	else
	{
		_isCSV = false;
		_csvIndex = 0;
	}
	//VariableFolder::updateRuntime(this);
}

variables::Value VariableNode::getCurrentValue() const
{
	if (!hasCSV())
		return _value;
	auto* stringVal = std::get_if<std::string>(&_value);
	if (!stringVal)
		return _value;
	const auto values = svutils::explode(std::string_view(","), std::string_view(*stringVal));
	auto& val = values[getCSVIndex() % values.size()];
	return variables::string_to_value(val);
}

variables::Value VariableNode::getValue() const
{
	return _value;
}

std::string VariableNode::getString() const
{
	std::string result;
	struct VisitValue
	{
		VisitValue(std::string& result) :m_result(result)
		{

		}
		void operator()(bool arg)
		{
		}
		void operator()(int arg)
		{
			m_result = std::to_string(arg);
		}
		void operator()(float arg)
		{
			m_result = std::to_string(arg);
		}
		void operator()(const std::string& val)
		{
			m_result = val;
		}
		std::string& m_result;
	};
	std::visit(VisitValue(result), _value);
	return result;
}

void VariableNode::setCSVIndex(std::string const& newIndex)
{
	auto pval = std::get_if<std::string>(&_value);
	if (_isCSV && pval)
	{
		auto exploded = svutils::explode(std::string_view(","), std::string_view(*pval));

		for (size_t i = 0; i != exploded.size(); ++i)
		{
			if (exploded[i] == newIndex)
			{
				setCSVIndex(static_cast<unsigned>(i));
				return;
			}
		}
	}
	else
	{
		setValue(newIndex);
	}
}

void VariableNode::setCSVIndex(unsigned newIndex)
{
	auto pval = std::get_if<std::string>(&_value);
	if (_isCSV && pval)
	{
		auto max = std::count_if(pval->begin(), pval->end(), [](auto ch) {return ch == ','; }) + 1;
		_csvIndex = newIndex % max;
		VariableFolder::updateRuntime(this);
	}
	else
	{
		_csvIndex = 0;
	}
}

unsigned VariableNode::getCSVIndex() const
{
	return _csvIndex;
}

bool VariableNode::hasCSV() const
{
	return _isCSV;
}

ValueType treeview::VariableNode::GetValueType()
{
	auto val = this->getValue();
	if (((VariableFolder*)this->getParent())->getExclusive())
	{
		return e_VT_Exclusive;
	}
	else if (val.index() == 3)
	{
		return e_VT_Bool;
	}
	else
	{
		return e_VT_Data;
	}
}

bool VariableNode::getVis() const
{
	bool result = false;
	struct VisitValue
	{
		VisitValue(bool& result) :resref(result)
		{

		}
		void operator()(bool arg)
		{
			resref = arg;
		}
		void operator()(int arg)
		{
			resref = arg;
		}
		void operator()(float arg)
		{
			resref = arg;
		}
		void operator()(const std::string& val)
		{
			resref = false;
		}
		bool& resref;
	};
	VisitValue visitor(result);
	std::visit(visitor, _value);
	return result;
}

void VariableNode::setVis(bool newVis)
{
	_isCSV = false;
	_csvIndex = 0;
	_value = newVis;

	auto* p = dynamic_cast<VariableFolder*>(getParent());
	if (p && p->getExclusive())
	{
		if (newVis)
		{
			for (auto& c : *p)
			{
				auto* v = dynamic_cast<VariableNode*>(c.get());
				if (v && v != this)
				{
					v->setVis(false);
				}
			}
		}
		else
		{
			bool elseVis = false;
			for (auto& c : *p)
			{
				auto* v = dynamic_cast<VariableNode*>(c.get());
				if (v && v != this && v->getVis())
				{
					elseVis = true;
					break;
				}
			}
			if (!elseVis)
			{// in an exclusive folder one option must always be selected
				_value = true;
			}
		}
	}

	VariableFolder::updateRuntime(this);
}

bool VariableNode::rename(const std::string& name)
{
	if (!name.size())
		return false;
	auto uniqueName = make_name_unique(this, variables::sanitize_for_lua(name), true);
	VariableFolder::removeFromRuntime(this);
	_name = uniqueName;
	VariableFolder::updateRuntime(this);
	return true;
}

void VariableNode::parentChanged()
{
	if (!_name.size())
		return;
	_name = make_name_unique(this, _name, true);
}

std::unique_ptr<Node> VariableNode::allocateCopy() const
{
	return std::make_unique<VariableNode>(*dynamic_cast<const VariableNode*>(this));
}

/*static std::string variant_to_string(variables::Value val)
{
	struct Visitor
	{
		Visitor(std::string& result) :_result(result)
		{

		}
		void operator()(bool val)
		{
			if (val)
				_result = "true";
			else
				_result = "false";
		}
		void operator()(const std::string& val)
		{
			auto escaped = val;
			stlstring::escapeJson(escaped);
			_result = "\"" + escaped + "\"";
		}
		void operator()(int val)
		{
			_result = std::to_string(val);
		}
		void operator()(float val)
		{
			_result = fsutils::stlstring::minimizeFloat(val);
		}
		std::string& _result;
	};
	std::string result;
	std::visit(Visitor(result), val);
	return result;
}*/

std::string VariableNode::getJson(bool addReturns) const
{
	std::string result;

	/// TODO write using rapid json writer function
	/*if (addReturns)
		result += "\n";

	result += "{\"typeof\":\"variable\"";

	if (_name.size())
	{
		auto escaped = _name;
		stlstring::escapeJson(escaped);
		result += ",\"name\":\""s + escaped + "\""s;
	}

	result += ",\"val\":"s + variant_to_string(_value);

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

void VariableNode::setJson(const rapidjson::Value& o)
{
	auto name = o.FindMember("name");
	if (name != o.MemberEnd())
	{
		rename(name->value.GetString());
	}

	auto value = o.FindMember("val");
	if (value != o.MemberEnd())
	{
		_isCSV = false;
		_csvIndex = 0;
		if (value->value.IsFloat())
			_value = value->value.GetFloat();
		else if (value->value.IsInt())
			_value = value->value.GetInt();
		else if (value->value.IsBool())
			_value = value->value.GetBool();
		else if (value->value.IsString())
		{
			_value = std::string(value->value.GetString());
			auto& s(std::get<std::string>(_value));
			if (std::find_if(s.begin(), s.end(), [](auto ch) {return ch == ','; }) != s.end())
				_isCSV = true;
		}
	}
}

std::string VariableNode::getDescription() const
{
	return getName();
}