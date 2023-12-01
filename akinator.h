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
    kQuit          ,"выйти"  ,"чтобы закончить работу Акинатора",
    kPrint         ,"база"   ,"чтобы показать базу",
    kStartGuessing ,"начать" ,"чтобы начать работу Акинатора",
    kYesAnswer     ,"да"     ,"думайте...",
    kNoAnswer      ,"нет"    ,"думайте...",
    kDefinition    ,"опр"    ,"чтобы Акинатор дал определение объекту",
    kDifference    ,"разл"   ,"чтобы Акинатор показал различия между двумя объектами",
    kSave          ,"сохр"   ,"сохранить чтобы сохранить базу",
    kHelp          ,"пом"    ,"помощь - собственно помощь",
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
