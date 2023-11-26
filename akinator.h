#ifndef AKINATOR_HEADER
#define AKINATOR_HEADER

#include "trees.h"

typedef enum
{
    kQuit,
    kStartGuessing,
    kDefinition,
    kDifference,
    kYesAnswer,
    kNoAnswer,
    kPrint,
    kAkinatorSuccess,
    kUnknownCommand,
    kRightAnswer,
    kWrongAnswer,
    kNullCmd,
    kFailedToFindObject,
    kUnknownCommandInThisScope,
    kSave,
} AkinatorCmd_t;

struct AkinatorCommand
{
    AkinatorCmd_t cmd_code;
    const char *cmd_string;
};

static const size_t kCmdCount = 8;

static AkinatorCommand AkinatorCommandsArray[kCmdCount] =
{
    kQuit,          "�����",
    kPrint,         "������ ����",
    kStartGuessing, "������",
    kYesAnswer,     "��",
    kNoAnswer,      "���",
    kDefinition,    "���� �����������",
    kDifference,    "��������",
    kSave,          "���������",
};

AkinatorCmd_t CallGuesser(Tree *tree, TreeNode *node);

AkinatorCmd_t CallInterface(Tree *tree);

AkinatorCmd_t GetCommand();

AkinatorCmd_t AddMember(Tree *tree, TreeNode *node);

AkinatorCmd_t GiveDefinition(Tree *tree);

#endif
