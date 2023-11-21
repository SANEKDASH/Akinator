#ifndef TREES_HEADER
#define TREES_HEADER

#include "TextParse/text_parse.h"

typedef char* TreeDataType_t;

typedef enum
{
    kTreeSuccess,
    kFailedAllocation,
    kFailedToOpenFile,
} TreeErrs_t;

struct TreeNode
{
    TreeDataType_t data;

    TreeNode *left;
    TreeNode *right;

    //TreeNode *parent;
};

struct Tree
{
    TreeNode *root;
};

TreeErrs_t TreeCtor(Tree *tree, TreeDataType_t root_val);

TreeErrs_t NodeCtor(Tree *tree, TreeNode **node, TreeDataType_t node_val);

TreeErrs_t TreeDtor(TreeNode *root);

TreeErrs_t PrintTree(TreeNode *root, FILE *output_file);

TreeErrs_t PrintTreeInFile(Tree *tree);

TreeErrs_t ReadTreeFromFile(Tree *tree);

TreeErrs_t CreateNodeFromText(Tree *tree, TreeNode **curr_node, Text *text, size_t *iterator);

TreeErrs_t SwapNodesData(TreeNode *node_lhs, TreeNode *node_rhs);

#endif
