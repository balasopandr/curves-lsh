#ifndef __COMPLETE_BINARY_TREE__
#define __COMPLETE_BINARY_TREE__
#include "list.h"

typedef struct complete_bin_tree *CompleteBinaryTree;

CompleteBinaryTree CompleteBinaryTree_create(unsigned int height, List leaves);
void CompleteBinaryTree_build(CompleteBinaryTree node, unsigned int height, Node *cur_node);
void CompleteBinaryTree_delete(CompleteBinaryTree node);
void * CompleteBinaryTree_get_data(CompleteBinaryTree node);
CompleteBinaryTree CompleteBinaryTree_get_left_child(CompleteBinaryTree node);
CompleteBinaryTree CompleteBinaryTree_get_right_child(CompleteBinaryTree node);
bool CompleteBinaryTree_isLeaf(CompleteBinaryTree node);

#endif
