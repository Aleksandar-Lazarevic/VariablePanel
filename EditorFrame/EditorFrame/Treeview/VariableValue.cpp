/* Copyright (c) 2023 GameMart Inc. */
#include "VariableValue.hpp"
#include <unordered_set>

using namespace variables;

std::string variables::sanitize_for_lua(std::string_view idealname)
{
	static std::unordered_set<std::string_view> reserved = {
		"and",
		"break",
		"do",
		"else",
		"elseif",
		"end",
		"false",
		"for",
		"function",
		"if",
		"in",
		"local",
		"nil",
		"not",
		"or",
		"repeat",
		"return",
		"then",
		"true",
		"until",
		"while"
	};
	std::string result;
	result.reserve(idealname.size());
	for (auto& c : idealname)
	{
		if (c == ' ')
			result.push_back('_');
		else if (isalpha(c) || c == '_')
			result.push_back(c);
		else if (isalnum(c))
		{
			if (!result.size())
				result.push_back('X');
			result.push_back(c);
		}
	}
	if (reserved.find(result) != reserved.end())
		return "invalid_name";
	return result;
}


variables::Value variables::string_to_value(const std::string_view newvalue)
{
	variables::Value result;
	if (newvalue == "true")
		result = true;
	else if (newvalue == "false")
		result = false;
	else if (newvalue == "yes")
		result = true;
	else if (newvalue == "no")
		result = false;
	else
	{
		bool hasPeriod = false;
		bool hasAlpha = false;
		bool hasNumbers = false;
		for (auto ch : newvalue)
		{
			if (ch == '.')
				hasPeriod = true;
			else if ((ch >= '0' && ch <= '9') || ch == '-')
				hasNumbers = true;
			else
				hasAlpha = true;
		}
		if (hasAlpha)
			result = std::string(newvalue);
		else if (hasPeriod)
			result = (float)atof(newvalue.data());
		else
			result = atoi(newvalue.data());
	}
	return result;
}