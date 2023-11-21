#include <stdio.h>
#include <stdlib.h>

#define DEBUG

#include "debug/color_print.h"
#include "debug/debug.h"
#include "TextParse/text_parse.h"
#include "trees.h"
#include "tree_dump.h"

static const char *kTreeSaveFileName = "tree_save.txt";

static const int kPoisonVal = 0xBADBABA;

//==============================================================================

TreeErrs_t TreeCtor(Tree *tree, TreeDataType_t root_val)
{
    CHECK(tree);

    tree->root = (TreeNode *) calloc(1, sizeof(TreeNode));

    if (tree->root == nullptr)
    {
        perror("huy");

        return kFailedAllocation;
    }

    tree->root->data = root_val;
    tree->root->left  = nullptr;
    tree->root->right = nullptr;

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t TreeDtor(TreeNode *root)
{
    if (root == nullptr)
    {
        return kTreeSuccess;
    }

    root->data = 0;

    TreeDtor(root->left);
    TreeDtor(root->right);

    free(root);

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t NodeCtor(Tree *tree, TreeNode **node, TreeDataType_t node_val)
{
    CHECK(node);

    *node = (TreeNode *) calloc(1, sizeof(TreeNode));

    if (node == nullptr)
    {
        return kFailedAllocation;
    }

    (*node)->data = (TreeDataType_t) calloc(strlen(node_val), sizeof(char));
    strcpy((*node)->data, node_val);

    (*node)->left = (*node)->right = nullptr;

    GraphDumpList(tree);

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t PrintTree(TreeNode *root, FILE *output_file)
{
    CHECK(root);
    CHECK(output_file);

    if (!root)
    {
        fprintf(output_file,"null ");

        return kTreeSuccess;
    }

    fprintf(output_file, "( ");

    fprintf(output_file, "%s ", root->data);

    PrintTree(root->left, output_file);

    PrintTree(root->right, output_file);

    fprintf(output_file, ") ");

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t PrintTreeInFile(Tree *tree)
{
    CHECK(tree);

    FILE *output_file = fopen(kTreeSaveFileName, "wb");

    if (output_file == nullptr)
    {
        return kFailedToOpenFile;
    }

    PrintTree(tree->root, output_file);

    fclose(output_file);

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t ReadTreeFromFile(Tree *tree)
{
    CHECK(tree);

    Text tree_text = {0};

    ReadTextFromFile(&tree_text, kTreeSaveFileName);

    size_t iterator = 1;

    CreateNodeFromText(tree, &tree->root, &tree_text, &iterator);

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t CreateNodeFromText(Tree *tree, TreeNode **curr_node, Text *text, size_t *iterator)
{
    CHECK(tree);
    CHECK(curr_node);
    CHECK(iterator);

    NodeCtor(tree, curr_node, text->lines_ptr[*iterator]);

    ++(*iterator);

    if (*text->lines_ptr[*iterator] == '(')
    {
        ++(*iterator);

        CreateNodeFromText(tree, &((*curr_node)->left), text, iterator);
    }
    else if (*text->lines_ptr[*iterator] != ')')
    {
        if (strcmp(text->lines_ptr[*iterator], "null") == 0)
        {
            (*curr_node)->left = nullptr;
        }
        else
        {
            NodeCtor(tree, &(*curr_node)->left, text->lines_ptr[*iterator]);
        }

        ++(*iterator);
    }

    if (*text->lines_ptr[*iterator] == '(')
    {
        ++(*iterator);

        CreateNodeFromText(tree, &((*curr_node)->right), text, iterator);
    }
    else if (*text->lines_ptr[*iterator] != ')')
    {
        if (strcmp(text->lines_ptr[*iterator], "null") == 0)
        {
           (*curr_node)->right = nullptr;
        }
        else
        {
            NodeCtor(tree, &(*curr_node)->right, text->lines_ptr[*iterator]);
        }

        ++(*iterator);
    }

    if (*text->lines_ptr[*iterator] == ')')
    {
        ++(*iterator);

        return kTreeSuccess;
    }

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t SwapNodesData(TreeNode *node_lhs, TreeNode *node_rhs)
{
    TreeDataType_t tmp = nullptr;

    CHECK(node_lhs);
    CHECK(node_rhs);

    tmp = node_lhs->data;

    node_lhs->data = node_rhs->data;

    node_rhs->data = tmp;

    return kTreeSuccess;
}

//==============================================================================
