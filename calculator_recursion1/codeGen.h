#ifndef __CODEGEN__
#define __CODEGEN__

#include "parser.h"

// Evaluate the syntax tree
extern int evaluateTree(BTNode *root);

//
extern int VarInRight(BTNode* root);

// Print the syntax tree in prefix
extern void printPrefix(BTNode *root);

extern void printInfix(BTNode* root);
extern void printPostfix(BTNode* root);

#endif // __CODEGEN__
