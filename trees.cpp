#include <stdio.h>
#include <stdlib.h>

#include "debug/color_print.h"
#include "debug/debug.h"
#include "TextParse/text_parse.h"
#include "trees.h"
#include "tree_dump.h"
#include "Stack/stack.h"

static const char *kTreeSaveFileName = "tree_save.txt";

static const int kPoisonVal = 0xBADBABA;

static TreeErrs_t CreateNodeFromText(Tree      *tree,
                                     TreeNode  *parent_node,
                                     TreeNode **curr_node,
                                     Text      *text,
                                     size_t    *iterator);

static void SeekNode(Stack           *stk,
                     TreeNode        *node,
                     TreeNode       **ret_node,
                     TreeDataType_t   key);

static TreeErrs_t CreateNodeFromBrackets(Tree      *tree,
                                         TreeNode  *parent_node,
                                         TreeNode **curr_node,
                                         Text      *text,
                                         size_t    *iterator);

//==============================================================================

TreeErrs_t TreeCtor(Tree           *tree,
                    TreeDataType_t  root_val)
{
    CHECK(tree);

    tree->root = (TreeNode *) calloc(1, sizeof(TreeNode));

    if (tree->root == nullptr)
    {
        perror("TreeCtor() failed to allocate memory");

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

    if (root->left != nullptr)
    {
        TreeDtor(root->left);
    }

    if (root->right != nullptr)
    {
        TreeDtor(root->right);
    }

    free(root);

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t NodeCtor(Tree            *tree,
                    TreeNode        *parent_node,
                    TreeNode       **node, //*
                    TreeDataType_t   node_val)
{
    CHECK(node);

    *node = (TreeNode *) calloc(1, sizeof(TreeNode));

    if (node == nullptr)
    {
        return kFailedAllocation;
    }

    (*node)->data = (TreeDataType_t) calloc(strlen(node_val), sizeof(char));

    if ((*node)->data == nullptr)
    {
        free(*node);

        return kFailedAllocation;
    }

    strcpy((*node)->data, node_val);

    (*node)->left   = (*node)->right = nullptr;
    (*node)->parent = parent_node;

    GRAPH_DUMP_TREE(tree);

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t PrintTree(const TreeNode *root,
                     FILE           *output_file)
{
    CHECK(output_file);

    if (root == nullptr)
    {
        fprintf(output_file,"null ");

        return kTreeSuccess;
    }

    fprintf(output_file, "( ");

    fprintf(output_file, "\"%s\" ", root->data);

    PrintTree(root->left, output_file);

    PrintTree(root->right, output_file);

    fprintf(output_file, ") ");

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t PrintTreeInFile(Tree       *tree,
                           const char *file_name)
{
    CHECK(tree);

    FILE *output_file = fopen(file_name, "wb");

    if (output_file == nullptr)
    {
        return kFailedToOpenFile;
    }

    PrintTree(tree->root, output_file);

    fclose(output_file);

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t ReadTreeOutOfFile(Tree *tree)
{
    CHECK(tree);

    Text tree_text = {0};

    if (ReadTextFromFile(&tree_text, kTreeSaveFileName) != kSuccess)
    {
        printf("\nReadTreeOutOfFile() failed to read text from file\n");

        return kFailedToReadText;

        TextDtor(&tree_text);
    }

    size_t iterator = 0;

    if (CreateNodeFromText(tree, nullptr, &tree->root, &tree_text, &iterator) != kTreeSuccess)
    {
        printf("ReatTreeOutOfFile() failed to read tree");

        return kFailedToReadTree;
    }

    return kTreeSuccess;
}

//==============================================================================

static TreeErrs_t CreateNodeFromText(Tree      *tree,
                                     TreeNode  *parent_node,
                                     TreeNode **curr_node,
                                     Text      *text,
                                     size_t    *iterator)
{
    CHECK(tree);
    CHECK(curr_node);
    CHECK(iterator);
    CHECK(text);

    TreeErrs_t status = kTreeSuccess;

    if (*text->lines_ptr[*iterator] == '(')
    {
        ++(*iterator);

        status = NodeCtor(tree, parent_node, curr_node, text->lines_ptr[*iterator]);

        if (status != kTreeSuccess)
        {
            return status;
        }

        ++(*iterator);
    }

    status = CreateNodeFromBrackets(tree, *curr_node, &(*curr_node)->left, text, iterator);

    if (status != kTreeSuccess)
    {
        return status;
    }

    status = CreateNodeFromBrackets(tree, *curr_node, &(*curr_node)->right, text, iterator);

    if (status != kTreeSuccess)
    {
        return status;
    }

    if (*text->lines_ptr[*iterator] == ')')
    {
        return kTreeSuccess;
    }

    return kTreeSuccess;
}

//==============================================================================

static TreeErrs_t CreateNodeFromBrackets(Tree      *tree,
                                         TreeNode  *parent_node,
                                         TreeNode **node,
                                         Text      *text,
                                         size_t    *iterator)
{
    if (*text->lines_ptr[*iterator] == '(')
    {
        TreeErrs_t status = CreateNodeFromText(tree, parent_node, node, text, iterator);

        if (status != kTreeSuccess)
        {
            return status;
        }

        ++(*iterator);
    }
    else if (*text->lines_ptr[*iterator] != ')')
    {
        if (strcmp(text->lines_ptr[*iterator], "null") == 0)
        {
            *node = nullptr;
        }
        else
        {
            TreeErrs_t status = NodeCtor(tree, parent_node, node, text->lines_ptr[*iterator]);

            if (status != kTreeSuccess)
            {
                return status;
            }
        }

        ++(*iterator);
    }

    return kTreeSuccess;
}

//==============================================================================

TreeErrs_t SwapNodesData(TreeNode *node_lhs,
                         TreeNode *node_rhs)
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

TreeNode *FindNode(Stack          *stk,
                   TreeNode       *node,
                   TreeDataType_t  key)
{
    TreeNode *curr_node = nullptr;

    SeekNode(stk, node, &curr_node, key);

    return curr_node;
}

//==============================================================================

static void SeekNode(Stack           *stk,
                     TreeNode        *node,
                     TreeNode       **ret_node,
                     TreeDataType_t   key)
{
    CHECK(stk);
    CHECK(node);
    CHECK(ret_node);

    int pop_value = 0;

    if (strcmp(node->data, key) == 0)
    {
        *ret_node = node;

        return;
    }

    //prichesat
    if (node->left != nullptr && *ret_node == nullptr)
    {
        Push(stk, kGoLeft);

        SeekNode(stk, node->left, ret_node, key);

        if (*ret_node == nullptr)
        {
            Pop(stk, &pop_value);
        }
    }

    if (node->right != nullptr && *ret_node == nullptr)
    {
        Push(stk, kGoRight);

        SeekNode(stk, node->right, ret_node, key);

        if (*ret_node == nullptr)
        {
            Pop(stk, &pop_value);
        }
    }
}

//==============================================================================
