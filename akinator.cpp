#include <stdio.h>

#include "akinator.h"
#include "tree_dump.h"

const size_t kMaxCmdLen = 64;

static void PrintDefinition(const Tree *tree,
                            const Stack *stk,
                            const TreeNode *node);

static AkinatorCmd_t PrintDifference(const Tree *tree);

static void PrintDiff(const Tree *tree,
                      const TreeNode *lhs_node,
                      const TreeNode *rhs_node,
                      const Stack *lhs_stk,
                      const Stack *rhs_stk);



//==============================================================================

AkinatorCmd_t GetCommand()
{
    static char command_string[kMaxCmdLen] = {0};

    scanf("%s", command_string);

    for (size_t i = 0; i < kCmdCount; i++)
    {
        if (strcmp(command_string, AkinatorCommandsArray[i].cmd_string) == 0)
        {
            return AkinatorCommandsArray[i].cmd_code;
        }
    }

    return kUnknownCommand;
}

//==============================================================================

AkinatorCmd_t CallInterface(Tree *tree)
{
    AkinatorCmd_t status = kUnknownCommand;

    while ((status = GetCommand()) != kQuit)
    {
        switch (status)
        {
            case kStartGuessing:
            {
                CallGuesser(tree, tree->root);

                break;
            }

            case kDefinition:
            {
                GiveDefinition(tree);

                break;
            }

            case kDifference:
            {
                PrintDifference(tree);

                break;
            }

            case kQuit:
            case kSaveTree:
            case kReviveTree:
            case kYesAnswer:
            case kAkinatorSuccess:
            case kUnknownCommand:
            case kRightAnswer:
            case kWrongAnswer:
            case kUnknownCommandInThisScope:
            case kNoAnswer:

            default:
            {
                printf("kavo?");

                return kUnknownCommand;

                break;
            }
        }
    }

    printf(">>GoodBye");

    PrintTreeInFile(tree);

    return kAkinatorSuccess;
}

//==============================================================================

AkinatorCmd_t CallGuesser(Tree *tree, TreeNode *node)
{

    AkinatorCmd_t status = kUnknownCommand;

    if (node->left == nullptr && node->right == nullptr)
    {
        printf(">>The answer is %s?\n>>", node->data);

        status = GetCommand();

        if (status == kYesAnswer)
        {
            printf(">>You can:\n"
                   "\t-Print definition of this object (use command \"def\") or\n"
                   "\t-Quit (use command \"quit\")"
                   "\t-Suck dick\n>>");

            return kAkinatorSuccess;
        }
        else if (status == kNoAnswer)
        {
            AddMember(tree, node);

            return kNoAnswer;
        }
        else
        {
            return kUnknownCommandInThisScope;
        }
    }

    printf(">>%s?\n>>", node->data);

    status = GetCommand();

    if (status == kYesAnswer)
    {
        return CallGuesser(tree, node->right);
    }
    else if (status == kNoAnswer)
    {
        return CallGuesser(tree, node->left);
    }
    else
    {
        printf(">>Wrong command\n");

        return kUnknownCommandInThisScope;
    }
}

//==============================================================================

AkinatorCmd_t AddMember(Tree *tree, TreeNode *node)
{
    printf(">>Do you want to add an answer?\n>>");

    AkinatorCmd_t status = GetCommand();

    if (status == kYesAnswer)
    {
        char new_member_str[kMaxCmdLen] = {0};

        printf(">>Write the answer you wanted to see\n>>");

        scanf("%s", new_member_str);

        NodeCtor(tree,
                 node,
                 &node->right, new_member_str);

        printf(">>What is the difference between this answers?\n");

        scanf("%s", new_member_str);

        NodeCtor(tree,
                 node,
                 &node->left, new_member_str);

        SwapNodesData(node, node->left);

        GraphDumpList(tree);
    }
    else
    {
        return kAkinatorSuccess;
    }
}

//==============================================================================

AkinatorCmd_t GiveDefinition(Tree *tree)
{
    printf(">>Enter the name of the object whose definition you are interested in\n>>");

    static char obj_name[kMaxCmdLen] = {0};

    scanf("%s", obj_name);

    Stack stk = {0};

    StackInit(&stk);

    TreeNode *node = FindNode(&stk, tree->root, obj_name);

    PrintDefinition(tree, &stk, node);

    StackDtor(&stk);

}

//==============================================================================

static void PrintDefinition(const Tree *tree, const Stack *stk, const TreeNode *node)
{
    TreeNode *curr_node = tree->root;

    printf(">>Definition of %s:\n", node->data);

    for (size_t i = 0; i < stk->stack_data.size; i++)
    {
        if (stk->stack_data.data[i] == kGoLeft)
        {
            printf("\t-Not %s\n", curr_node->data);

            curr_node = curr_node->left;
        }
        else
        {
            printf("\t-%s\n", curr_node->data);

            curr_node = curr_node->right;
        }
    }
}

//==============================================================================

static AkinatorCmd_t PrintDifference(const Tree *tree)
{
    static char lhs_obj_name[kMaxCmdLen] = {0};
    static char rhs_obj_name[kMaxCmdLen] = {0};

    printf(">>Enter the names of the objects whose difference you are interested in\n>>");

    scanf("%s %s", lhs_obj_name, rhs_obj_name);

    Stack lhs_stk = {0};
    Stack rhs_stk = {0};

    StackInit(&lhs_stk);
    StackInit(&rhs_stk);

    TreeNode *lhs_node = FindNode(&lhs_stk, tree->root, lhs_obj_name);
    TreeNode *rhs_node = FindNode(&rhs_stk, tree->root, rhs_obj_name);

    PrintDiff(tree, lhs_node, rhs_node, &lhs_stk, &rhs_stk);

    StackDtor(&lhs_stk);
    StackDtor(&rhs_stk);

    return kAkinatorSuccess;
}

//==============================================================================

static void PrintDiff(const Tree *tree,
                      const TreeNode *lhs_node,
                      const TreeNode *rhs_node,
                      const Stack *lhs_stk,
                      const Stack *rhs_stk)
{
    size_t i = 0;
    TreeNode *curr_node = tree->root;

    printf(">>\"%s\" and \"%s\" are similar at points:\n", lhs_node->data, rhs_node->data);

    for ( ; i < lhs_stk->stack_data.size &&
            i < lhs_stk->stack_data.size &&
            lhs_stk->stack_data.data[i] == rhs_stk->stack_data.data[i];
            i++)
    {
            printf("\t-%s\n", curr_node->data);

            if (lhs_stk->stack_data.data[i] == kGoLeft)
            {
                curr_node = curr_node->left;
            }
            else
            {
                curr_node = curr_node->right;
            }
    }

    if (lhs_stk->stack_data.data[i] == kGoLeft)
    {
        printf(">>\"%s\" is not %s\n", lhs_node->data, curr_node->data);
    }

    if (rhs_stk->stack_data.data[i] == kGoLeft)
    {
        printf(">>\"%s\" is not %s\n", rhs_node->data, curr_node->data);
    }

    printf(">>");
}
//==============================================================================
