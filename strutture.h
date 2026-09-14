#include <stdio.h>
#include <stdbool.h>

typedef struct NODO{
        bool bit;
        unsigned char value;//0-1
        unsigned char occurrence;//number of occurrence, or sum of subtree occurrence
        struct NODO* right;
        struct NODO* left;

}NODO ;

typedef struct CODA_PRIORITA{
    unsigned char value;
    unsigned char occorrenze;
    struct CODA_PRIORITA* next;
    struct CODA_PRIORITA* prev;

}CODA_PRIORITA;
