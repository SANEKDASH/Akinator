#include <stdio.h>

#define DEBUG

#include "akinator.h"
#include "TextParse/text_parse.h"
#include "trees.h"
#include "tree_dump.h"


int main()
{
    Tree tree;

    BeginTreeGraphDump();

    TreeCtor(&tree, "blyadina");

    ReadTreeFromFile(&tree);
    CallInterface(&tree);

    EndTreeGraphDump();
    TreeDtor(tree.root);



    return 0;
}
