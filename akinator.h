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
    kUnknownCommand,
    kRightAnswer,
    kWrongAnswer,
    kNullCmd,
    kSave,
    kHelp,
} AkinatorCmd_t;

typedef enum
{
    kAkinatorSuccess,
    kFailedToFindObject,
    kReadenUnknownCommand,
} AkinatorErrs_t;

struct AkinatorCommand
{
    AkinatorCmd_t cmd_code;
    const char *cmd_string;
    const char *help_message;
};


static AkinatorCommand AkinatorCommandsArray[] =
{
    kQuit          ,"�����"  ,"����� ��������� ������ ���������",
    kPrint         ,"����"   ,"����� �������� ����",
    kStartGuessing ,"������" ,"����� ������ ������ ���������",
    kYesAnswer     ,"��"     ,"�������...",
    kNoAnswer      ,"���"    ,"�������...",
    kDefinition    ,"���"    ,"����� �������� ��� ����������� �������",
    kDifference    ,"����"   ,"����� �������� ������� �������� ����� ����� ���������",
    kSave          ,"����"   ,"��������� ����� ��������� ����",
    kHelp          ,"���"    ,"������ - ���������� ������",
};

static const size_t kCmdCount = sizeof(AkinatorCommandsArray) / sizeof(AkinatorCommand);

AkinatorErrs_t CallGuesser(Tree     *tree,
                          TreeNode *node);

AkinatorErrs_t CallInterface(Tree *tree);

AkinatorCmd_t GetCommand();

AkinatorErrs_t AddMember(Tree     *tree,
                        TreeNode *node);

AkinatorErrs_t GiveDefinition(Tree *tree);

#endif
