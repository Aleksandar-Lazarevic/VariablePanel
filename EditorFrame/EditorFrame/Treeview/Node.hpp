/* Copyright (c) 2023 GameMart Inc. */
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>

namespace rapidjson
{
	class CrtAllocator;
	template <typename BaseAllocator>
	class MemoryPoolAllocator;
	template <typename CharType>
	struct UTF8;
	template <typename Encoding, typename Allocator>
	class GenericValue;
	typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator>> Value;
}

namespace treeview
{
	class Node;

	std::string make_name_unique(const Node* changing, std::string targetName, bool useUnderscores);

	template <typename Pred>
	bool ConstIterateNodes(const Node& node, Pred predicate)
	{
		for (auto& it : node)
		{
			auto& child = *it.get();
			if (!predicate(child) || !ConstIterateNodes(child, predicate))
				return false;
		}
		return true;
	}

	template <typename Pred>
	static bool IterateNodes(Node& node, Pred predicate)
	{
		for (auto& it : node)
		{
			auto& child = *it.get();
			if (!predicate(child) || !IterateNodes(child, predicate))
				return false;
		}
		return true;
	}

	typedef std::vector<std::unique_ptr<Node>> NodeVector;

	class Node
	{
	public:
		virtual std::string getDescription() const = 0;
		const Node& at(unsigned int index) const;
		Node& at(unsigned int index);
		const Node* getRoot() const;
		Node* getRoot();
		bool isParent(const Node& possibleChild) const;
		virtual void setJson(const rapidjson::Value& source);
		virtual std::string getName() const;
		virtual bool evaluate() const;
		virtual std::string getJson(bool addReturns) const = 0;
		virtual Node& operator[](unsigned int index);
		virtual const Node& operator[](unsigned int index) const;
		virtual Node& operator[](const std::string& index);
		virtual const Node& operator[](const std::string& index) const;
		virtual std::unique_ptr<Node> allocateCopy() const = 0;
		virtual NodeVector::iterator begin();
		virtual NodeVector::iterator end();
		virtual NodeVector::const_iterator begin() const;
		virtual NodeVector::const_iterator end() const;
		virtual NodeVector::iterator erase(NodeVector::iterator position);
		unsigned find(Node& child);
		const Node* findWithPaths(std::vector<std::string_view> const& path, bool caseInsensitive) const;
		Node* findWithPaths(std::vector<std::string_view> const& path, bool caseInsensitive);
		const Node* findWithPath(std::string_view path, bool caseInsensitive = false) const;
		Node* findWithPath(std::string_view path, bool caseInsensitive = false);
		unsigned find(std::string_view index, bool caseInsensitive = false) const;
		virtual unsigned getScope() const;
		void removeMatches(const std::string& removeName);
		virtual void remove(unsigned int index);
		virtual void empty();
		bool move(unsigned int oldPosition, Node& newParent, unsigned int newPosition);
		unsigned int dup(unsigned int index);
		template <typename T>
		T* addChild(std::unique_ptr<T> newChild)
		{
			_children.push_back(std::move(newChild));
			auto newptr = dynamic_cast<T*>(_children.back().get());
			newptr->_parent = this;
			_children.back()->parentChanged();
			return newptr;
		}
		template <typename T>
		T* insertChild(std::unique_ptr<T> newChild)
		{
			NodeVector::iterator pos = _children.begin() + nIdx;
			auto it = _children.insert(pos, std::move(newChild));
			auto newptr = dynamic_cast<T*>((*it).get());
			newptr->_parent = this;
			(*it)->parentChanged();
			return newptr;
		}
		template <typename T, typename... Args>
		T* addChild(Args&&... args)
		{
			_children.push_back(std::make_unique<T>(std::forward<Args>(args)...));
			auto newptr = dynamic_cast<T*>(_children.back().get());
			newptr->_parent = this;
			_children.back()->parentChanged();
			return newptr;
		}

		template <typename T, typename... Args>
		T* insertChild(int nIdx, Args&&... args)
		{
			NodeVector::iterator pos = _children.begin() + nIdx;
			auto it = _children.insert(pos, std::make_unique<T>(std::forward<Args>(args)...));
			auto newptr = dynamic_cast<T*>((*it).get());
			newptr->_parent = this;
			(*it)->parentChanged();
			return newptr;
		}

		bool operator==(const std::string& other);
		virtual bool canBeParent() const;
		virtual bool canUserRename() const;
		Node* getParent() const;
		virtual bool rename(const std::string& name);
		Node* replaceChild(unsigned int index, std::unique_ptr<Node> newChild);
		unsigned int recursive_size() const;
		virtual unsigned int size() const;
		virtual ~Node();
		Node();
		Node(const Node& other);

	private:
		virtual void parentChanged();
		virtual void childRemoved();
		Node& operator=(const Node& other) = delete;
		Node* _parent;
		NodeVector _children;
	};
}