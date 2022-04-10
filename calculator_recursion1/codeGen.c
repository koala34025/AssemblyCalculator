#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGen.h"
#include "lex.h"

int VarInRight(BTNode* root) {
    if (!root)
        return 0;

    if (root->data == ID)
        return 1;

    return (VarInRight(root->left) || VarInRight(root->right));
}

int evaluateTree(BTNode *root) {
    int retval = 0, lv = 0, rv = 0;

    if (root != NULL) {
        switch (root->data) {
            case ID:
                if (!existVar(root->lexeme)) {              // first appears
                    if (match(ASSIGN))                      // followed by a (=)
                        retval = getval(root->lexeme);
                    else
                        error(NOTFOUND);
                }
                else
                    retval = getval(root->lexeme);
                break;
            case INT:
                retval = atoi(root->lexeme);
                break;
            case ASSIGN:
                rv = evaluateTree(root->right);
                retval = setval(root->left->lexeme, rv);
                break;
            case ADDSUB:
            case MULDIV:
                lv = evaluateTree(root->left);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+") == 0) {
                    retval = lv + rv;
                } else if (strcmp(root->lexeme, "-") == 0) {
                    retval = lv - rv;
                } else if (strcmp(root->lexeme, "*") == 0) {
                    retval = lv * rv;
                } else if (strcmp(root->lexeme, "/") == 0) {
                    if (rv == 0) {
                        if (!VarInRight(root->right))
                            error(DIVZERO)
                        else
                            retval = 0;
                    }
                    else {
                        retval = lv / rv;
                    }
                }
                break;

            case INCDEC:
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "++") == 0) {
                    retval = setval(root->right->lexeme, 1 + rv);
                }
                else if (strcmp(root->lexeme, "--") == 0) {
                    retval = setval(root->right->lexeme, -1 + rv);
                }
                break;
            case AND: case OR: case XOR:
                lv = evaluateTree(root->left);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "&") == 0) {
                    retval = lv & rv;
                }
                else if (strcmp(root->lexeme, "|") == 0) {
                    retval = lv | rv;
                }
                else if (strcmp(root->lexeme, "^") == 0) {
                    retval = lv ^ rv;
                }
                break;

            default:
                retval = 0;
        }
    }
    return retval;
}

void printPrefix(BTNode *root) {
    if (root != NULL) {
        printf("%s ", root->lexeme);
        printPrefix(root->left);
        printPrefix(root->right);
    }
}
void printInfix(BTNode* root) {
    if (root != NULL) {
        printInfix(root->left);
        printf("%s ", root->lexeme);
        printInfix(root->right);
    }
}

void printPostfix(BTNode* root) {
    if (root != NULL) {
        printPostfix(root->left);
        printPostfix(root->right);
        printf("%s ", root->lexeme);
    }
}

