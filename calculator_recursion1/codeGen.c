#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGen.h"

int r[8];
int top;

int push(int data) {
    if (top >= 8)
        printf("r[] is full, push nothing\n");
    else
        r[++top] = data;
}

int peek() {
    if (isEmpty()) 
        printf("r[] is empty, peek nothing\n");
    else
        return r[top];
}

int pop() {
    if (isEmpty()) 
        printf("r[] is empty, pop nothing\n");
    else 
        r[top--] = 0;
}

int isEmpty() {
    if (top == -1)
        return 1;
    else
        return 0;
}


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
            case ID:                                        // called by evaluateTree(root->right)
                if (existVar(root->lexeme) == -1) {              // first appears than instantly know its wrong
                    //if (match(ASSIGN))                      
                    //    retval = getval(root->lexeme);
                    //else
                    error(NOTFOUND);
                }
                else {
                    retval = getval(root->lexeme);
                    push(retval);
                    printf("MOV r%d [%d]\n", top, existVar(root->lexeme) * 4);
                }
                break;
            case INT:
                retval = atoi(root->lexeme);
                push(retval);
                printf("MOV r%d %d\n", top, retval);
                break;
            case ASSIGN: // mem[]
                rv = evaluateTree(root->right);             // since LVAL must be variable, setval straightly
                retval = setval(root->left->lexeme, rv);    // otherwise in case ID:, it checks existVar();
                pop();
                printf("MOV [%d] r%d\n", existVar(root->left->lexeme) * 4, top + 1);
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
                pop();
                pop();
                push(retval);
                if (strcmp(root->lexeme, "+") == 0) {
                    printf("ADD r%d r%d\n", top, top + 1);
                }
                else if (strcmp(root->lexeme, "-") == 0) {
                    printf("SUB r%d r%d\n", top, top + 1);
                }
                else if (strcmp(root->lexeme, "*") == 0) {
                    printf("MUL r%d r%d\n", top, top + 1);
                }
                else if (strcmp(root->lexeme, "/") == 0) {
                    printf("DIV r%d r%d\n", top, top + 1);
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
                r[top] = retval;
                printf("%c r%d %d\n", strcmp(root->lexeme, "++") == 0 ? "ADD" : "SUB", top, 1);
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
                pop();
                pop();
                push(retval);
                if (strcmp(root->lexeme, "&") == 0) {
                    printf("AND r%d r%d\n", top, top + 1);
                }
                else if (strcmp(root->lexeme, "|") == 0) {
                    printf("OR r%d r%d\n", top, top + 1);
                }
                else if (strcmp(root->lexeme, "^") == 0) {
                    printf("XOR r%d r%d\n", top, top + 1);
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

