#include <stdio.h>
#include <TXlib.h>
#define DEBUG

#include "akinator.h"
#include "TextParse/text_parse.h"
#include "trees.h"
#include "tree_dump.h"

int main()
{
    txSetLocale();
    txDisableAutoPause();

    Tree tree;

    InitTreeGraphDump();

    TreeCtor(&tree, "blyadina");

    ReadTreeOutOfFile(&tree);

    CallInterface(&tree);

    EndTreeGraphDump();
    TreeDtor(tree.root);

    return 0;
}
