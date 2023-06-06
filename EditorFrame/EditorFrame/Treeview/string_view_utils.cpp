/* Copyright (c) 2023 GameMart Inc. */
#include "string_view_utils.hpp"

std::vector<std::string_view> svutils::explode(std::string_view delimiter, std::string_view str)
{
	std::vector<std::string_view> arr;

	auto strleng = str.length();
	auto delleng = delimiter.length();
	if (delleng == 0)
		return arr; //no change

	size_t i = 0;
	size_t k = 0;
	while (i < strleng)
	{
		unsigned j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng) //found delimiter
		{
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}