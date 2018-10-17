#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"



int main (int argc, char *argv[]) {
       machine machine;

       initInterpreter(argc, argv, &machine);
       readFile(argv, &machine);
       exec(&machine);
}
