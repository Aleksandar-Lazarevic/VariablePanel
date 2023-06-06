/* Copyright (c) 2023 GameMart Inc. */
#pragma once

#include <variant>
#include <string>
#include <vector>

namespace variables
{
	std::string sanitize_for_lua(std::string_view idealname);
	using Value = std::variant<std::string, int, float, bool>;

	struct Values
	{
		Values()
		{
		}
		Values(const std::string& name)
		{
			_name = name;
		}
		Values(const std::string& name, const Value& value)
		{
			_value = value;
			_name = name;
		}
		std::string _name;
		Value _value;
		std::vector<Values> _children;
	};

	Value string_to_value(const std::string_view newvalue);
}