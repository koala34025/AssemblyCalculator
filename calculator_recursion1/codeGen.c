#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGen.h"

int evaluateTree(BTNode *root) {
    int retval = 0, lv = 0, rv = 0;

    if (root != NULL) {
        switch (root->data) {
            case ID:
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
                    if (rv == 0)
                        error(DIVZERO);
                    retval = lv / rv;
                }
                break;

            case INCDEC:

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

