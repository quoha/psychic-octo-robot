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
typedef struct PORNOD PORNOD;
typedef struct PORSTK PORSTK;
typedef struct PORSTR PORSTR;
struct PORBUK {
    int   (*c)(PORBUK *root, PORSTK *stack, PORSTR *string);
    PORBUK *s[256];
};
struct PORNOD {
    PORSTK *prev;
    PORSTK *tail;
    enum {pSTR} kind;
    union {
        PORSTR *string;
        void   *v;
    } u;
};
struct PORSTK {
    PORNOD *top;
    PORNOD *bottom;
};
struct PORSTR {
    char *curr;
    char *eod;
    char  data[1];
};
int     POR_Eval(PORBUK *root, PORSTK *stack, PORSTR *prog);
PORSTK *POR_Stack(void);
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
            b = root;
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
int POR_Register(PORBUK *root, const char *name, int (*c)(PORBUK *root, PORSTK *stack, PORSTR *string)) {
    PORBUK *b = root;
    const unsigned char *s = (const unsigned char *)name;
    while (*s) {
        if (!b->s[*s]) {
            b->s[*s] = POR_Bucket();
            if (!b->s[*s]) {
                perror("register function");
                return 0;
            }
        }
        b = b->s[*s];
        s++;
    }
    if (b->c && b->c != c) {
        printf("error:\tfunction '%s' is already defined\n", name);
        return 0;
    }
    b->c = c;
    return 1;
}

//--------------------------------------------------------------
//
PORSTK *POR_Stack(void) {
    PORSTK *s = malloc(sizeof(*s));
    if (s) {
        memset(s, 0, sizeof(*s));
    }
    return s;
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
int F_Hi(PORBUK *root, PORSTK *stack, PORSTR *prog) {
    printf("  por:\thi <<%s>>\n", prog->curr);
    return 1;
}

//--------------------------------------------------------------
//
int F_World(PORBUK *root, PORSTK *stack, PORSTR *prog) {
    printf("  por:\tthere!\n");
    return 1;
}

//--------------------------------------------------------------
//
int main(int argc, const char * argv[])
{
    PORBUK *root  = POR_Bucket();
    PORSTR *prog  = POR_String("Hello, World ", -1);
    PORSTK *stack = POR_Stack();

    POR_Register(root, "Hello,", F_Hi);
    POR_Register(root, "World" , F_World);

    POR_Eval(root, stack, prog);

    return 0;
}

