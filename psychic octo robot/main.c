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
    PORNOD *prev;
    PORNOD *next;
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
PORNOD *POR_Pop(PORSTK *stack);
PORNOD *POR_Push(PORSTK *stack, int kind, void *item);
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
PORNOD *POR_Node(int kind, void *item) {
    PORNOD *n = malloc(sizeof(*n));
    if (n) {
        n->prev = n->next = 0;
        n->kind = kind;
        n->u.v  = item;
    }
    return n;
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
PORNOD *POR_Pop(PORSTK *stack) {
    PORNOD *node = (stack && stack->top) ? stack->top : 0;
    if (node) {
        stack->top = stack->top->prev;
        if (!stack->top) {
            stack->bottom = 0;
        } else {
            stack->top->next = 0;
        }
    }
    return node;
}

//--------------------------------------------------------------
//
PORNOD *POR_Push(PORSTK *stack, int kind, void *item) {
    if (stack) {
        PORNOD *node = POR_Node(kind, item);
        if (node) {
            if (!stack->top) {
                stack->top = stack->bottom = node;
            } else {
                stack->top->next = node;
                stack->top->next->prev = stack->top;
                stack->top = stack->top->next;
            }
        }
    }
    return stack ? stack->top : 0;
}

//--------------------------------------------------------------
//
PORSTK *POR_Stack(void) {
    PORSTK *s = malloc(sizeof(*s));
    if (s) {
        s->top = s->bottom = 0;
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
int F_ABC(PORBUK *root, PORSTK *stack, PORSTR *prog) {
    POR_Push(stack, pSTR, POR_String("abc", -1));
    return 1;
}

//--------------------------------------------------------------
//
int F_Print(PORBUK *root, PORSTK *stack, PORSTR *prog) {
    PORNOD *node = POR_Pop(stack);
    if (node && node->kind == pSTR) {
        printf("    p:\t%s\n", node->u.string->data);
    } else {
        printf("stack:\t** don't know how to print %d\n", node->kind);
        return 0;
    }
    return 1;
}

//--------------------------------------------------------------
//
int F_StackDump(PORBUK *root, PORSTK *stack, PORSTR *prog) {
    PORNOD *node = (stack && stack->bottom) ? stack->bottom : 0;
    int idx = 0;
    printf("stack:\tdumping stack...\n");
    while (node) {
        if (node->kind == pSTR) {
            printf("%5d:\t%s\n", idx++, node->u.string->data);
        } else {
            printf("%5d:\t** unknown node type %d\n", idx++, node->kind);
        }
        node = node->next;
    }
    return 1;
}

//--------------------------------------------------------------
//
int F_StackDup(PORBUK *root, PORSTK *stack, PORSTR *prog) {
    PORNOD *node = (stack && stack->top) ? stack->top : 0;
    if (node) {
        if (node->kind == pSTR) {
            POR_Push(stack, pSTR, POR_String(node->u.string->data, -1));
        } else {
            printf("stack:\t** don't know how to duplicate %d\n", node->kind);
            return 0;
        }
    } else {
        printf("stack:\t** can't dup an empty stack\n");
        return 0;
    }
    return 1;
}

//--------------------------------------------------------------
//
int main(int argc, const char * argv[])
{
    PORBUK *root  = POR_Bucket();
    PORSTR *prog  = POR_String("abc .s dup .s print .s ", -1);
    PORSTK *stack = POR_Stack();

    POR_Register(root, "dup"   , F_StackDup);
    POR_Register(root, ".s"    , F_StackDump);
    POR_Register(root, "abc"   , F_ABC);
    POR_Register(root, "print" , F_Print);

    POR_Eval(root, stack, prog);

    return 0;
}

