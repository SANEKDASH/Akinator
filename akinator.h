#ifndef AKINATOR_HEADER
#define AKINATOR_HEADER

#include "trees.h"

typedef enum
{
    kQuit,
    kSaveTree,
    kReviveTree,
    kStartGuessing,
    kDefinition,
    kDifference,
    kYesAnswer,
    kNoAnswer,
    kAkinatorSuccess,
    kUnknownCommand,
    kRightAnswer,
    kWrongAnswer,
    kUnknownCommandInThisScope,
} AkinatorCmd_t;

struct AkinatorCommand
{
    AkinatorCmd_t cmd_code;
    const char *cmd_string;
};

static const size_t kCmdCount = 8;

static AkinatorCommand AkinatorCommandsArray[kCmdCount] =
{
    kQuit,          "quit",
    kSaveTree,      "save",
    kReviveTree,    "revive",
    kStartGuessing, "start",
    kYesAnswer,     "yes",
    kNoAnswer,      "no",
    kDefinition,    "def",
    kDifference,    "diff",
};

AkinatorCmd_t CallGuesser(Tree *tree, TreeNode *node);

AkinatorCmd_t CallInterface(Tree *tree);

AkinatorCmd_t GetCommand();

AkinatorCmd_t AddMember(Tree *tree, TreeNode *node);

AkinatorCmd_t GiveDefinition(Tree *tree);

#endif
