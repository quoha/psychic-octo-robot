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
    char  data[1];
};
struct PORSTK {
    void *p;
};

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
int POR_Execute(PORBUK *root, PORSTK *stack) {
    return 0;
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
            p->data[length] = 0;
        }
        p->curr = p->data;
    }
    return p;
}

//--------------------------------------------------------------
//
int main(int argc, const char * argv[])
{

    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

