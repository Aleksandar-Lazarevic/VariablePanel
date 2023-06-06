/* Copyright (c) 2023 GameMart Inc. */
#pragma once

#include <optional>
#include <string>

namespace treeview
{
	class Node;
	class Listener
	{
		virtual ~Listener()=0;
		virtual void created(std::string const& newname) = 0;
		virtual void renamed(std::string const& oldname, std::string const& newname)=0;
		virtual void moved(std::string const& oldpath, std::string const& newpath)=0;
		virtual void deleted(std::string const&path) = 0;
	};
}