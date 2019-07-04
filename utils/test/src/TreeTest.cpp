//
// Created by massimo on 05/08/18.
//

#include "gtest/gtest.h"
#include <functional>

#include "TreeItem.hpp"

using namespace utils;

template<typename T>
bool isTreeWellMade(TreeItem<T>* tree)
{
	if (!tree)
		return true;

	for (TreeItem<T>* child : tree->getChildren())
		if (!isTreeWellMade<T>(child))
			return false;

	if (!tree->getParent())
		return true;

	return (tree->getParent()->hasChild(*tree));
}

class TreeTest: public testing::Test
{
	public:
	TreeTest(): tree(4) {}
	Tree<int> tree;
	virtual void SetUp() {}
	virtual void TearDown() { EXPECT_TRUE(isTreeWellMade<int>(&tree.getRoot())); }
};

TEST_F(TreeTest, defaultRootHasNoChildren)
{
	EXPECT_EQ(tree.getRoot().getChildCount(), 0);
	EXPECT_EQ(tree.getRoot().getChildren().size(), 0);
}

TEST_F(TreeTest, rootCanBeSet)
{
	tree.setRoot(5);
	EXPECT_EQ(tree.getRoot().getData(), 5);
}

TEST_F(TreeTest, canAddChild)
{
	tree.setRoot(5);
	TreeItem<int>* t = &tree.getRoot().addChild(1);

	EXPECT_EQ(t->getData(), 1);
	EXPECT_EQ(tree.getRoot().hasChild(*t), 1);
}

TEST_F(TreeTest, canAddMultipleChild)
{
	for (int a = 0; a < 10; a++)
	{
		EXPECT_TRUE(tree.getRoot().hasChild(tree.getRoot().addChild(a)));
	}
}

TEST_F(TreeTest, recursiveAddChild)
{
	TreeItem<int>* node(&tree.getRoot());
	for (int a = 0; a < 20; a++)
	{
		node = &node->addChild(a);
		EXPECT_EQ(node->getData(), a);
	}

	for (int a = 19; a >= 0; a--)
	{
		EXPECT_EQ(node->getData(), a);
		node = node->getParent();
	}
}

TEST_F(TreeTest, childrenCanBeRemoved)
{
	for (int a = 0; a < 10; a++)
		tree.getRoot().addChild(a);

	for (TreeItem<int>* child : tree.getRoot().getChildren())
	{
		EXPECT_EQ(child->removeFromParent().get(), child);
		EXPECT_FALSE(tree.getRoot().hasChild(*child));
	}
}

#ifdef CULT_DEATH_TEST
TEST_F(TreeTest, orphanChildCantBeRemoved)
{
	EXPECT_DEBUG_DEATH(tree.getRoot().removeFromParent(), ".*");
}
#endif

TEST_F(TreeTest, removedNodesCanBeReadded)
{
	TreeItem<int>& node = tree.getRoot().addChild(1);
	TreeItem<int>* ptr(&node);
	EXPECT_EQ(&tree.getRoot().addChild(node.removeFromParent()), ptr);
}

TEST_F(TreeTest, dataCanBeSet)
{
	tree.getRoot().setData(2);
	EXPECT_EQ(tree.getRoot().getData(), 2);
}

TEST_F(TreeTest, ancestrorsShouldSayTheyAre)
{
	std::unique_ptr<TreeItem<int>> ptr(std::make_unique<TreeItem<int>>(2));
	TreeItem<int>* reference(ptr.get());
	tree.getRoot().addChild(std::move(ptr));

	EXPECT_TRUE(tree.getRoot().isAncestor(*reference));
	EXPECT_FALSE(reference->isAncestor(tree.getRoot()));
}

#ifdef CULT_DEATH_TEST
TEST_F(TreeTest, nodeCantBeAddedToHisOwnAncestor)
{
	std::unique_ptr<TreeItem<int>> ptr(std::make_unique<TreeItem<int>>(2));
	std::unique_ptr<TreeItem<int>> ptr2(std::make_unique<TreeItem<int>>(3));
	ptr->addChild(std::move(ptr2));
	EXPECT_DEBUG_DEATH(ptr2->addChild(std::move(ptr)), ".*");
}
#endif

TEST_F(TreeTest, treeForAllShouldBeAppliedToEveryOne)
{
	TreeItem<int>* element = &tree.getRoot();
	for (int a = 0; a < 5; a++)
	{
		for (int a = 0; a < 5; a++)
			element->addChild(5);
		element = element->getChildren()[0];
	}
	int count = 0;

	std::function<void(int&)> fun([&count](int&) { count++; });
	tree.getRoot().forAllChildren(fun);
	EXPECT_EQ(count, 26);
}

TEST_F(TreeTest, treeForAllAncestorShouldBeAppliedToEveryOne)
{
	TreeItem<int>* element(&tree.getRoot());
	for (int a = 0; a < 5; a++)
	{
		element->addChild(5);
		element = element->getChildren()[0];
	}

	int count = 0;

	std::function<void(int&)> fun([&count](int&) { count++; });
	element->forAllAncestors(fun);
	EXPECT_EQ(count, 6);
}
