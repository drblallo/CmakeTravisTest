//
// Created by massimo on 05/08/18.
//

#ifndef CULT_TREE_HPP
#define CULT_TREE_HPP

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

namespace utils
{
	template<typename T>
	class TreeItem
	{
		public:
		explicit TreeItem(const T& newData): parent(), children(), data(newData){};
		explicit TreeItem(T&& newData)
				: parent(), children(), data(std::move(newData)){};
		TreeItem& operator=(const TreeItem<T>& other) = delete;
		TreeItem& operator=(TreeItem<T>&& other) = delete;
		TreeItem(const TreeItem<T>& other) = delete;
		TreeItem(TreeItem<T>&& other) = delete;
		~TreeItem() = default;

		TreeItem<T>* getParent() const { return parent; }
		TreeItem<T>* getRoot()
		{
			if (!getParent())
				return this;
			return getParent()->getRoot();
		}

		unsigned long getChildCount() const { return children.size(); }

		std::vector<TreeItem<T>*> getChildren() const
		{
			std::vector<TreeItem<T>*> toReturn;

			for (const auto& child : children)
				toReturn.push_back(child.first);

			return toReturn;
		}

		bool hasChild(TreeItem& t) const
		{
			return children.find(&t) != children.end();
		}

		const T& getData() const { return data; }

		T& getData() { return data; }

		void setData(const T& newData) { data = newData; }

		template<typename F>
		void forAllChildren(F&& function)
		{
			function(data);
			for (const auto& child : children)
				child.first->forAllChildren(function);
		}

		template<typename F>
		void forAllAncestors(F&& function)
		{
			function(data);
			if (parent)
				parent->forAllAncestors(function);
		}

		bool isAncestor(TreeItem<T>& descendant)
		{
			bool found = false;

			descendant.forAllAncestors([this, &found](T& ancestor) {
				if (&ancestor == &data)
					found = true;
			});

			return found;
		}

		/*
		 * transfers ownership of the current tree item to the caller.
		 * Childs are not affected
		 *
		 */
		std::unique_ptr<TreeItem<T>> removeFromParent()
		{
			assert(parent != nullptr);
			auto childSet(&parent->children);
			assert(getParent()->hasChild(*this));

			std::unique_ptr<TreeItem<T>> ptr(std::move(childSet->at(this)));
			childSet->erase(this);
			parent = nullptr;
			return ptr;
		}

		TreeItem<T>& addChild(std::unique_ptr<TreeItem<T>>&& child)
		{
			assert(child.get() != nullptr);
			assert(!hasChild(*child.get()));
			assert(child.get() != this);
			assert(!child->isAncestor(*this));
			assert(child->parent == nullptr);

			TreeItem<T>* ptr(child.get());
			child->parent = this;
			children[child.get()] = std::move(child);
			return *ptr;
		}

		TreeItem<T>& addChild(T&& data)
		{
			return addChild(std::make_unique<TreeItem<T>>(std::move(data)));
		}

		TreeItem<T>& addChild(const T& data)
		{
			return addChild(std::make_unique<TreeItem<T>>(data));
		}

		private:
		TreeItem<T>* parent;
		std::map<TreeItem<T>*, std::unique_ptr<TreeItem<T>>> children;
		T data;
	};

	template<typename T>
	class Tree
	{
		public:
		explicit Tree(const T& rootData)
				: root(std::make_unique<TreeItem<T>>(rootData))
		{
		}

		explicit Tree(T&& rootData)
				: root(std::make_unique<TreeItem<T>>(std::move(rootData)))
		{
		}

		TreeItem<T>& getRoot() { return *root; }
		const TreeItem<T>& getRoot() const { return *root; }
		void setRoot(const T& data) { root.reset(new TreeItem<T>(data)); }

		private:
		std::unique_ptr<TreeItem<T>> root;
	};
}	// namespace utils

#endif	// CULT_TREE_HPP
