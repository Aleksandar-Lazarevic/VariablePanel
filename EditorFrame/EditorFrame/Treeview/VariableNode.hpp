/* Copyright (c) 2023 GameMart Inc. */
#pragma once

#include "Node.hpp"
#include "VariableValue.hpp"

namespace treeview {
	enum ValueType
	{
		e_VT_Data,
		e_VT_Bool,
		e_VT_Exclusive,
	};
	class VariableNode : public Node
	{
	public:
		VariableNode();
		VariableNode(std::string_view name);
		VariableNode(const VariableNode& other);
		virtual std::string getName() const;
		virtual bool rename(const std::string& name);
		virtual ~VariableNode();
		virtual std::string getDescription() const;
		virtual std::string getJson(bool addReturns) const;
		virtual void setJson(const rapidjson::Value& source);
		virtual std::unique_ptr<Node> allocateCopy() const;
		void setValue(const std::string_view newvalue);
		variables::Value getValue() const;
		variables::Value getCurrentValue() const;
		std::string getString() const;
		bool getVis() const;
		void setVis(bool newVis);
		void setCSVIndex(unsigned newIndex);
		void setCSVIndex(std::string const& newIndex);
		unsigned getCSVIndex() const;
		bool hasCSV() const;
		ValueType GetValueType();
	private:
		variables::Value _value;
		std::string _name;
		unsigned _csvIndex;
		bool _isCSV;
		virtual void parentChanged();
	};
}