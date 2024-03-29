#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"
#include "parser.h"

static TokenSet getToken(void);
static TokenSet curToken = UNKNOWN;
static char lexeme[MAXLEN];

TokenSet getToken(void)
{
    int i = 0;
    char c = '\0';

    while ((c = fgetc(stdin)) == ' ' || c == '\t');

    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i < MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        // TODO
        // handle invalid variable names, e.g., 1_Var
        if (isalpha(c) || c == '_') {
            error(INVANAME);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    } 
    /*
    else if (c == '+' || c == '-') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return ADDSUB;
    }
    */

    // determine ADDSUB or INCDEC
    else if (c == '+') {
        lexeme[0] = c;
        c = fgetc(stdin);
        if (c == '+') {
            lexeme[1] = c;
            lexeme[2] = '\0';
            return INCDEC;
        }
        else {
            lexeme[1] = '\0';
            ungetc(c, stdin);
            return ADDSUB;
        }
    }
    else if (c == '-') {
        lexeme[0] = c;
        c = fgetc(stdin);
        if (c == '-') {
            lexeme[1] = c;
            lexeme[2] = '\0';
            return INCDEC;
        }
        else {
            lexeme[1] = '\0';
            ungetc(c, stdin);
            return ADDSUB;
        }
    }

    else if (c == '*' || c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULDIV;
    } else if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    } else if (isalpha(c) || c == '_') {
        
        // handle arbitrary variable names, e.g., Var_1
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while ((isalpha(c) || isdigit(c) || c == '_') && i < MAXLEN) {
            lexeme[i] = c;
            i++;
            c = fgetc(stdin);
        }
        ungetc(c, stdin);
        //lexeme[1] = '\0';
        lexeme[i] = '\0';
        return ID;

    } else if (c == EOF) {
        return ENDFILE;
    }

    else if (c == '&') {
        strcpy(lexeme, "&");
        return AND;
    }
    else if (c == '|') {
        strcpy(lexeme, "|");
        return OR;
    }
    else if (c == '^') {
        strcpy(lexeme, "^");
        return XOR;
    }

    else {
        return UNKNOWN;
    }
}

void advance(void) {
    curToken = getToken();
}

int match(TokenSet token) {
    if (curToken == UNKNOWN)
        advance();
    return token == curToken;
}

char *getLexeme(void) {
    return lexeme;
}
