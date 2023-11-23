#include <stdio.h>

#include "akinator.h"
#include "tree_dump.h"

const size_t kMaxCmdLen = 64;

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
                   "\t-Print definition of this object (use command \"definition\") or\n"
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
