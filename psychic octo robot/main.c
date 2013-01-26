//
//  main.c
//  psychic octo robot
//
//  Created by Michael Henderson on 1/25/13.
//  Copyright (c) 2013 Michael Henderson. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//--------------------------------------------------------------
//
typedef struct PORBUK PORBUK;
typedef struct PORSTK PORSTK;
typedef struct PORSTR PORSTR;
struct PORBUK {
    int   (*c)(PORBUK *root, PORSTK *stack, PORSTR *string);
    PORBUK *s[256];
};
struct PORSTR {
    char *curr;
    char *eod;
    char  data[1];
};
struct PORSTK {
    void *p;
};
PORSTR *POR_String(const char *string, int length);

//--------------------------------------------------------------
//
PORBUK *POR_Bucket(void) {
    PORBUK *b = malloc(sizeof(*b));
    if (b) {
        memset(b, 0, sizeof(*b));
    }
    return b;
}

//--------------------------------------------------------------
//
int POR_Eval(PORBUK *root, PORSTK *stack, PORSTR *prog) {
    PORBUK *b = root;
    prog->curr = prog->data;
    char *startOfWord = prog->curr;
    while (prog->curr < prog->eod) {
        if (b->s[*(prog->curr)]) {
            b = b->s[*(prog->curr++)];
        } else if (isspace(*(prog->curr)) && b->c) {
            // execute
            prog->curr++;
            int rv = b->c(root, stack, prog);
            if (!rv) {
                return rv;
            }
            startOfWord = prog->curr;
        } else {
            // invalid token
            PORSTR *word = POR_String(startOfWord, (int)(prog->curr + 1 - startOfWord));
            if (word) {
                printf("error:\tinvalid word '%s'\n", word->data);
                free(word);
            }
            return 0;
        }
    }
    return 1;
}

//--------------------------------------------------------------
//
PORSTR *POR_String(const char *string, int length) {
    if (length < 0) {
        length = (int)strlen(string ? string : "");
    }
    PORSTR *p = malloc(sizeof(*p) + length);
    if (p) {
        if (!length || !string) {
            memset(p, 0, sizeof(*p) + length);
        } else {
            memcpy(p->data, string, length);
        }
        p->curr = p->data;
        p->eod  = p->data + length;
        *(p->eod) = 0;
    }
    return p;
}

//--------------------------------------------------------------
//
int main(int argc, const char * argv[])
{
    PORBUK *root  = POR_Bucket();
    PORSTR *prog  = POR_String("Hello, World\n", 12);
    PORSTK *stack = 0;

    POR_Eval(root, stack, prog);

    return 0;
}

