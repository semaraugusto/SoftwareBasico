#ifndef INTERPRETER_H
#define INTERPRETER_H
#define MEMSIZE 256

typedef struct {
       int mem[MEMSIZE]; //memória
       int m, finalm; // memória atual, memória final
       int pc, ac, rc, rx; // registradores: contador de programa e acumulador.
       short unsigned int zero; // flag p/ se o resultado da operacao foi 0
} machine;

typedef struct {
       int opcode;
       int deslc;
} instruction;

void initInterpreter(int argc, char *argv[], machine *machine);

void readFile(char *argv[], machine *machine);

int getInstruction(machine *machine);

void exec(machine *machine);

int decode(machine *machine, instruction instruction);

#endif
