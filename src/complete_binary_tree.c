#include <stdio.h>
#include <stdlib.h>
#include "complete_binary_tree.h"
#include "curve.h"
#include "global_defs.h"

struct complete_bin_tree
{
	void *data;
	bool is_leaf;
	CompleteBinaryTree left_child, right_child;
};

CompleteBinaryTree CompleteBinaryTree_create(unsigned int height, List leaves)
{
	CompleteBinaryTree root = malloc(sizeof(struct complete_bin_tree));
	if(height == 0)
	{
		root->is_leaf = true;
	}
	else
	{
		root->is_leaf = false;
		Node cur_node = List_get_first_node(leaves);
		CompleteBinaryTree_build(root, height-1, &cur_node);
	}
	return root;
}

void CompleteBinaryTree_build(CompleteBinaryTree node, unsigned int height, Node *cur_node)
{
	if(height == 0)
	{
		node->is_leaf = true;
		node->left_child = NULL;
		node->right_child = NULL;
		if(*cur_node == NULL)
		{
			node->data = NULL;
		}
		else
		{
			node->data = Node_get_data(*cur_node);
			*cur_node = Node_get_next(*cur_node);
		}
	}
	else
	{
		node->is_leaf = false;
		node->left_child = malloc(sizeof(struct complete_bin_tree));
		node->right_child = malloc(sizeof(struct complete_bin_tree));
		node->data = NULL;
		CompleteBinaryTree_build(node->left_child, height-1, cur_node);
		CompleteBinaryTree_build(node->right_child, height-1, cur_node);
	}
}

void CompleteBinaryTree_delete(CompleteBinaryTree node)
{
	if(node->is_leaf)
	{
		free(node);
		node = NULL;
	}
	else
	{
		CompleteBinaryTree_delete(node->left_child);
		CompleteBinaryTree_delete(node->right_child);
		free(node);
		node = NULL;
	}
}

void * CompleteBinaryTree_get_data(CompleteBinaryTree node)
{
	return node->data;
}

CompleteBinaryTree CompleteBinaryTree_get_left_child(CompleteBinaryTree node)
{
	return node->left_child;
}

CompleteBinaryTree CompleteBinaryTree_get_right_child(CompleteBinaryTree node)
{
	return node->right_child;
}

bool CompleteBinaryTree_isLeaf(CompleteBinaryTree node)
{
	return node->is_leaf;
}
