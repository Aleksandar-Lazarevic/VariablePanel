/* Copyright (c) 2023 GameMart Inc. */
#include "Node.hpp"
#include <boost/algorithm/string.hpp>
#include <stdexcept>
#include "string_view_utils.hpp"

using namespace treeview;

Node& Node::at(unsigned int index)
{
	return *_children[index].get();
}

const Node* Node::getRoot() const
{
	auto ptr = this;
	while (ptr->getParent())
	{
		ptr = ptr->getParent();
	}
	return ptr;
}

Node* Node::getRoot()
{
	return const_cast<Node*>(const_cast<const Node*>(this)->getRoot());
}

bool Node::isParent(const Node& possibleChild) const
{
	const Node* search = &possibleChild;
	while (search)
	{
		if (search == this)
			return true;
		search = search->getParent();
	}
	return false;
}

const Node& Node::at(unsigned int index) const
{
	return *_children[index].get();
}

Node& Node::operator[](unsigned int index)
{
	return *_children[index].get();
}

void Node::setJson(const rapidjson::Value& source)
{
}

const Node& Node::operator[](unsigned int index) const
{
	return *_children[index].get();
}

Node& Node::operator[](const std::string& index)
{
	return const_cast<Node&>(const_cast<const Node&>(*this)[index]);
}

const Node& Node::operator[](const std::string& index) const
{
	for (auto& search : _children)
	{
		if (search.get()->getName() == index)
		{
			return *search.get();
		}
	}
	throw std::out_of_range(index + " did not exist in " + getName());
}

NodeVector::iterator Node::begin()
{
	return _children.begin();
}

NodeVector::iterator Node::end()
{
	return _children.end();
}

NodeVector::const_iterator Node::begin() const
{
	return _children.begin();
}

NodeVector::const_iterator Node::end() const
{
	return _children.end();
}

NodeVector::iterator Node::erase(NodeVector::iterator position)
{
	return _children.erase(position);
}

Node* Node::findWithPaths(std::vector<std::string_view> const& path, bool caseInsensitive)
{
	return const_cast<Node*>(const_cast<const Node*>(this)->findWithPaths(path, caseInsensitive));
}

const Node* Node::findWithPaths(std::vector<std::string_view> const& path, bool caseInsensitive) const
{
	auto* parent = this;
	for (auto it : path)
	{
		auto found = parent->find(std::string(it), caseInsensitive);
		if (found >= parent->size())
			return nullptr;
		parent = &parent->at(found);
	}
	return parent;
}

Node* Node::findWithPath(std::string_view path, bool caseInsensitive)
{
	return const_cast<Node*>(const_cast<const Node*>(this)->findWithPath(path, caseInsensitive));
}

const Node* Node::findWithPath(std::string_view path, bool caseInsensitive) const
{
	auto exploded = svutils::explode("/", path);
	const Node* ptr = this;
	for (auto& index : exploded)
	{
		auto& children = ptr->_children;
		unsigned i = 0;
		const Node* found = nullptr;
		for (auto& search : children)
		{
			if ((caseInsensitive && boost::iequals(search->getName(), index)) || (!caseInsensitive && search->getName() == index))
			{
				found = children[i].get();
				break;
			}
			++i;
		}
		if (!found)
			return nullptr;
		ptr = found;
	}
	return ptr;
}

unsigned Node::find(std::string_view index, bool caseInsensitive) const
{
	unsigned i = 0;
	for (auto& search : _children)
	{
		if ((caseInsensitive && boost::iequals(search->getName(), index)) || (!caseInsensitive && search->getName() == index))
		{
			return i;
		}
		++i;
	}
	return -1;
}

unsigned Node::find(Node& child)
{
	unsigned i = 0;
	for (auto& search : _children)
	{
		if (search.get() == &child)
		{
			return i;
		}
		++i;
	}
	return -1;
}

unsigned Node::getScope() const
{
	return 0;
}

void Node::removeMatches(const std::string& removeName)
{
	_children.erase(std::remove_if(_children.begin(), _children.end(), [&removeName](auto& it) {
		if (boost::iequals(it->getName(), removeName))
		return true;
	return false;
		}),
		_children.end());
	childRemoved();
}

void Node::remove(unsigned int index)
{
	_children.erase(_children.begin() + index);
	childRemoved();
}

void Node::empty()
{
	_children.clear();
}

bool Node::move(unsigned int oldPosition, Node& newParent, unsigned int newPosition)
{
	if (&newParent == this)
	{
		oldPosition = std::min(oldPosition, (unsigned)_children.size() - 1);
		newPosition = std::min(newPosition, (unsigned)_children.size() - 1);
		if (oldPosition != newPosition)
		{
			if (newPosition < oldPosition)
				std::rotate(_children.begin() + newPosition, _children.begin() + oldPosition, _children.begin() + oldPosition + 1);
			else
				std::rotate(_children.begin() + oldPosition, _children.begin() + oldPosition + 1, _children.begin() + newPosition + 1);
			childRemoved();
			return true;
		}
	}
	else
	{
		oldPosition = std::min(oldPosition, (unsigned)_children.size() - 1);
		newPosition = std::min(newPosition, (unsigned)newParent.size());
		newParent.addChild(std::move(_children[oldPosition]));
		_children.erase(_children.begin() + oldPosition);
		if (!newParent.move(newParent.size() - 1, newParent, newPosition))
			childRemoved();
		return true;
	}
	return false;
}

unsigned int Node::dup(unsigned int index)
{
	if (index >= _children.size())
		throw std::out_of_range("dup had invalid index");
	auto ptr = _children[index]->allocateCopy();
	ptr->_parent = this;
	auto it = _children.insert(_children.begin() + index + 1, std::move(ptr));
	it->get()->parentChanged();
	return (unsigned)(it - _children.begin());
}

bool Node::operator==(const std::string& other)
{
	return getName() == other;
}

bool Node::rename(const std::string& name)
{
	return false;
}

Node* Node::replaceChild(unsigned int index, std::unique_ptr<Node> newChild)
{
	newChild->_parent = this;
	_children.at(index) = std::move(newChild);
	return _children.at(index).get();
}

/**
is this type of node allowed to hold children
*/
bool Node::canBeParent() const
{
	return false;
}

bool Node::canUserRename() const
{
	return false;
}

Node* Node::getParent() const
{
	return _parent;
}

std::string Node::getName() const
{
	return "";
}

bool Node::evaluate() const
{
	return true;
}

unsigned int Node::recursive_size() const
{
	unsigned count = 1;
	for (auto& child : _children)
	{
		count += child->recursive_size();
	}
	return count;
}

unsigned int Node::size() const
{
	return (unsigned)_children.size();
}

Node::~Node()
{
}

Node::Node()
{
	_parent = nullptr;
}

void Node::parentChanged()
{
}

void Node::childRemoved()
{
}

Node::Node(const Node& other)
{
	_parent = nullptr;
	for (auto& it : other)
	{
		addChild(it->allocateCopy());
	}
}

std::string treeview::make_name_unique(const Node* changing, std::string targetName, bool useUnderscores)
{
	auto parent = changing->getParent();
	std::replace(targetName.begin(), targetName.end(), '/', ' ');
	if (!parent)
		return targetName;
	auto isMatchedToParent = [parent, changing, &targetName]() -> bool {
		for (auto& child : *parent)
		{
			if (child.get() != changing && child->getName() == targetName)
			{
				return true;
			}
		}
		return false;
	};
	while (isMatchedToParent())
	{
		if (useUnderscores)
			targetName = targetName + "_copy";
		else
			targetName = targetName + " copy";
	}
	return targetName;
}