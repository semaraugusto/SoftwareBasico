#include <stdio.h>
#include <stdlib.h>
#include "montData.h"

int main(int argc, char *argv[]) {
       mounter mounter;
       int tableSize;

       initMounter(&mounter);

       tableSize = createSymbolTable(&mounter, argv, argc);

       runProgram(&mounter, argv, tableSize);

       return 0;
}
