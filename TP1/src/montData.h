#ifndef MOUNTERDATA_H
#define MOUNTERDATA_H
#define TABLESIZE 256
#define INSTRUCTIONSIZE 4
#define LINESIZE 100

typedef struct {
       char label;
       char operand; // either a variable or a symbol.
       char token[INSTRUCTIONSIZE];
       int address;
} command;

typedef struct {
       char label;
       int pos;
} label;

typedef struct {
       char var;
       int pos;
       int value;
} variable;

typedef struct {
       command table[TABLESIZE];
       variable varTable[TABLESIZE];
       label labelTable[TABLESIZE];
       int numVariables;
       int numLabels;
} mounter;

void initMounter(mounter *m);

int createSymbolTable(mounter *m, char *argv[], int argc);

void runProgram(mounter *m, char *argv[], int tableSize);

#endif
