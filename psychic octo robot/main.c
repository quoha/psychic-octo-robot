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
    PORBUK *s[27];
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
int POR_Execute(PORBUK *root, PORSTK *stack) {
    return 0;
}

//--------------------------------------------------------------
//
PORSTR *POR_String(const char *string) {
    string = string ? string : "";
    PORSTR *p = malloc(sizeof(*p) + strlen(string));
    if (p) {
        p->curr = p->data;
        strcpy(p->data, string);
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

