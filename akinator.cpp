#include <stdio.h>

#include "TextParse/text_parse.h"
#include "akinator.h"
#include "tree_dump.h"
#include "trees.h"
#include "debug/debug.h"

static const char  *kSaveFileName = "tree_save.txt";

static const size_t kMaxCmdLen = 64;

static void PrintDefinition(const Tree *tree,
                            const Stack *stk,
                            const TreeNode *node);

static AkinatorCmd_t PrintDifference(const Tree *tree);

static void PrintDiff(const Tree *tree,
                      const TreeNode *lhs_node,
                      const TreeNode *rhs_node,
                      const Stack *lhs_stk,
                      const Stack *rhs_stk);

static TreeErrs_t ShowTree(Tree *tree);

static void PrintNodes(TreeNode *node, FILE *output_file);

//==============================================================================

AkinatorCmd_t GetCommand()
{
    static char command_string[kMaxCmdLen] = {0};

    *command_string = 0;

    GetStr(command_string, kMaxCmdLen);

    if (*command_string == '\0')
    {
        return kNullCmd;
    }

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
    CHECK(tree);

    printf(">> Вас приветствует Акинатор. Напишите команду \"помощь\" чтобы увидеть список команд.\n>> ");

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

            case kNullCmd:
            {
                printf(">> ");

                break;
            }

            case kPrint:
            {
                ShowTree(tree);

                break;
            }

            case kUnknownCommand:
            {
                printf(">> Неизвесная команда, используйте команду \"помощь\" для того, чтобы увидеть список команд.\n >>");

                break;
            }

            case kSave:
            {
                PrintTreeInFile(tree, kSaveFileName);

                break;
            }

            case kFailedToFindObject:
            case kQuit:
            case kYesAnswer:
            case kAkinatorSuccess:
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
        printf(">> ");
    }

    printf(">> Досвидания.");

    return kAkinatorSuccess;
}

//==============================================================================

AkinatorCmd_t CallGuesser(Tree *tree, TreeNode *node)
{
    AkinatorCmd_t status = kUnknownCommand;

    printf(">> Акинатор начинает работу.\n");

    while (node->left != nullptr && node->right != nullptr)
    {
        printf(">> %s?\n>> ", node->data);

        status = GetCommand();

        if (status == kYesAnswer)
        {
            node = node->right;
        }
        else if (status == kNoAnswer)
        {
            node = node->left;
        }
        else
        {
            printf(">> Неизвестная команда\n");

            return kUnknownCommandInThisScope;
        }
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        printf(">> %s это т(а)от, кого вы искали?\n>> ", node->data);

        status = GetCommand();

        if (status == kYesAnswer)
        {
            printf(">> Замечательно!\n"
                   ">> Вы можете продолжить работу.\n");

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

    return kAkinatorSuccess;
}

//==============================================================================

AkinatorCmd_t AddMember(Tree *tree, TreeNode *node)
{
    printf(">> Хотите добавить объект?\n>> ");

    AkinatorCmd_t status = GetCommand();

    if (status == kYesAnswer)
    {
        static char new_member_str[kMaxCmdLen] = {0};
        *new_member_str = 0;

        printf(">> Напишите имя объекта, который хотите добавить\n>> ");

        GetStr(new_member_str, kMaxCmdLen);

        NodeCtor(tree, node, &node->right, new_member_str);

        printf(">> Чем отличается \"%s\" от \"%s\"?\n>> ",
               node->data,
               new_member_str);

        GetStr(new_member_str, kMaxCmdLen);

        NodeCtor(tree, node, &node->left, new_member_str);

        SwapNodesData(node, node->left);

        printf(">> Новый объект успешно добавлен!\n");

        GRAPH_DUMP_TREE(tree);
    }

    return kAkinatorSuccess;
}

//==============================================================================

AkinatorCmd_t GiveDefinition(Tree *tree)
{
    CHECK(tree);

    printf(">> Введите имя объекта, определение которого хотите увидеть\n>> ");

    static char obj_name[kMaxCmdLen] = {0};

    scanf("%s", obj_name);

    Stack stk = {0};

    StackInit(&stk);

    TreeNode *node = FindNode(&stk, tree->root, obj_name);

    if (node == nullptr)
    {
        StackDtor(&stk);

        printf(">> Не удалось найти \"%s\"в базе.\n", obj_name);

        return kFailedToFindObject;
    }

    if (node != nullptr)
    {
        PrintDefinition(tree, &stk, node);
    }

    StackDtor(&stk);

    return kAkinatorSuccess;
}

//==============================================================================

static void PrintDefinition(const Tree *tree, const Stack *stk, const TreeNode *node)
{
    CHECK(tree);
    CHECK(stk);
    CHECK(node);

    TreeNode *curr_node = tree->root;

    printf("> %s: ", node->data);

    for (size_t i = 0; i < stk->stack_data.size; i++)
    {
        if (stk->stack_data.data[i] == kGoLeft)
        {
            printf("не %s, ", curr_node->data);

            curr_node = curr_node->left;
        }
        else
        {
            printf("%s, ", curr_node->data);

            curr_node = curr_node->right;
        }
    }

    printf("\b\b.\n");
}

//==============================================================================

static AkinatorCmd_t PrintDifference(const Tree *tree)
{
    CHECK(tree);

    static char lhs_obj_name[kMaxCmdLen] = {0};
    static char rhs_obj_name[kMaxCmdLen] = {0};

    printf(">> Введите названия двух объектов, различия которых вы хотите увидеть.\n>> ");

    scanf("%s %s", lhs_obj_name, rhs_obj_name);

    Stack lhs_stk = {0};
    Stack rhs_stk = {0};

    StackInit(&lhs_stk);
    StackInit(&rhs_stk);

    TreeNode *lhs_node = FindNode(&lhs_stk, tree->root, lhs_obj_name);
    TreeNode *rhs_node = FindNode(&rhs_stk, tree->root, rhs_obj_name);

    if (lhs_node == nullptr)
    {
        printf(">> Не удалось найти \"%s\" в базе.\n", lhs_obj_name);
    }
    if (rhs_node == nullptr)
    {
        printf(">> Не удалось найти \"%s\" в базе.\n", rhs_obj_name);
    }

    if (lhs_node != nullptr && rhs_node != nullptr)
    {
        PrintDiff(tree, lhs_node, rhs_node, &lhs_stk, &rhs_stk);
    }

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
    CHECK(tree);
    CHECK(lhs_node);
    CHECK(rhs_node);
    CHECK(lhs_stk);
    CHECK(rhs_stk);

    size_t i = 0;

    TreeNode *curr_node = tree->root;

    printf(">> \"%s\" и \"%s\" схожи тем, что ", lhs_node->data, rhs_node->data);

    for ( ; i < lhs_stk->stack_data.size &&
            i < lhs_stk->stack_data.size &&
            lhs_stk->stack_data.data[i] == rhs_stk->stack_data.data[i];
            i++)
    {
            if (lhs_stk->stack_data.data[i] == 0)
            {
                printf("не ");
            }

            printf("%s, ", curr_node->data);

            if (lhs_stk->stack_data.data[i] == kGoLeft)
            {
                curr_node = curr_node->left;
            }
            else
            {
                curr_node = curr_node->right;
            }
    }

    printf("\b\b.\n");

    printf(">> Но %s в отличии от %s ", lhs_node->data, rhs_node->data);

    for (size_t j = i; j < lhs_stk->stack_data.size; j++)
    {
        if (lhs_stk->stack_data.data[i] == 0)
        {
            printf("не ");
        }

        printf("%s, ", curr_node->data);

        if (lhs_stk->stack_data.data[i] == kGoLeft)
        {
            curr_node = curr_node->left;
        }
        else
        {
            curr_node = curr_node->right;
        }
    }

    printf("\b\b.\n");
}

//==============================================================================

static TreeErrs_t ShowTree(Tree *tree)
{
    CHECK(tree);

    FILE *dot_file = fopen("tree_demo.dot", "w");

    if (dot_file == nullptr)
    {
        perror("ShowTree() failed to open output_file");

        return kFailedToOpenFile;
    }

    #define LOG_PRINT(...) fprintf(dot_file, __VA_ARGS__)

    if (dot_file == nullptr)
    {
        perror("GraphDumpTree() failed to open dump file");

        return kFailedToOpenFile;
    }

    LOG_PRINT("digraph List\n{\n"
              "\trankdir = TB;\n"
              "\tgraph [bgcolor = \"black\"]\n"
              "\tnode[color =\"black\", fontsize=14, shape = Mrecord];\n"
              "\tedge[color = \"red\", fontcolor = \"blue\",fontsize = 12];\n\n\n");


    PrintNodes(tree->root, dot_file);
    LogPrintEdges(tree->root, dot_file);


    LOG_PRINT("\n\n}");

    fclose(dot_file);

    system("iconv -f CP1251 -t UTF-8 tree_demo.dot > rus_tree_demo.dot");

    system("dot -Tpng rus_tree_demo.dot -o tree_demo.png");

    system("tree_demo.png");

    return kTreeSuccess;
}

//==============================================================================

static void PrintNodes(TreeNode *node, FILE *output_file)
{
    CHECK(node);
    CHECK(output_file);

    fprintf(output_file,
            "node%p [style = filled, fillcolor = \"green\", shape = Mrecord, label = "
            "\"%s\"]\n",
            node,
            node->data);

    if (node->left != nullptr)
    {
        PrintNodes(node->left, output_file);
    }

    if (node->right != nullptr)
    {
        PrintNodes(node->right, output_file);
    }

}

//==============================================================================
