#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

// inicializa o interpretador
void initInterpreter(int argc, char *argv[], machine *machine) {
       int count = argc;
       machine->pc = atoi(argv[2]);
       machine->m = atoi(argv[2]);
       machine->finalm = machine->m;
       machine->zero = 0;

       for(count = 0; count < MEMSIZE; count++) {
              machine->mem[count] = 0;
       }
       return;
}

void readFile(char *argv[], machine *machine) {
       int aux;
       instruction inst;
       char line[10];

       FILE *file;
       file = fopen(argv[1], "r");

       aux = machine->m;
       fgets(line, 10, file);
       while(!feof(file) && aux < MEMSIZE) {
              inst.opcode = 0; inst.deslc = 0;
              sscanf(line, "%d %d", &inst.opcode, &inst.deslc);

              if(inst.deslc == 0) {
                     // não há operação de deslocamento
                     machine->mem[aux] = inst.opcode;
                     aux++;
              }
              else {
                     // há operação de deslocamento
                     machine->mem[aux] = inst.opcode;
                     aux++;
                     machine->mem[aux] = inst.deslc;
                     aux++;
              }

              fgets(line, 10, file);
       }


       machine->finalm = aux;
       fclose(file);
       return;
}

int getInstruction(machine *machine) {
       // for(int i = machine->pc; i < machine->finalm; i++) {
       //        printf("%d\n", machine->mem[i]);
       // }
       int inst = 0;
       if(machine->pc == machine->finalm) {
              return -1;
       }
       else {

              inst = machine->mem[machine->pc];
              // printf("get instruction = %d\n", inst);
                     machine->pc++;
       }
       return inst;
}

void exec(machine *machine) {
       instruction instruction;
       int ret = 1;
       int cont = 0;
       instruction.opcode = 0;
       while(instruction.opcode >= 0 && ret >= 0) {
              instruction.opcode = 0; instruction.deslc = 0;
              instruction.opcode = getInstruction(machine);
              if(instruction.opcode != 0 && instruction.opcode != 11 && instruction.opcode != 19 && instruction.opcode != 20 && instruction.opcode != 21) {
                     instruction.deslc = getInstruction(machine);
              }
              ret = decode(machine, instruction);
       }

       // if(instruction.opcode != 0 && instruction.opcode != 11)

       // while

}

int decode(machine *machine, instruction inst) {
       int temp, inteiroLido;
       int ret = 1;
              if(inst.opcode == 1) // LAD -> carga AC direto
              {
                     temp = machine->pc + inst.deslc;
                     machine->ac = machine->mem[temp];
              }

              else if(inst.opcode == 2) // SAD -> armazena AC direto
              {
                     temp = machine->pc + inst.deslc;
                     machine->mem[temp] = machine->ac;
              }
              else if(inst.opcode == 3) // ADD -> soma
              {
                     temp = machine->pc + inst.deslc;
                     machine->ac += machine->mem[temp];
              }
              else if(inst.opcode == 4)// SUB -> subtrai
              {
                     temp = machine->pc + inst.deslc;
                     machine->ac -= machine->mem[temp];
              }
              else if(inst.opcode == 5) // INP ->entrada
              {
                     scanf("%d\n", &inteiroLido);
                     temp = machine->pc + inst.deslc;
                     machine->mem[temp] = inteiroLido;
              }
              else if(inst.opcode == 6)// OUT -> imprime
              {
                     temp = machine->pc + inst.deslc;
                     printf("%d\n", machine->mem[temp]);
              }
              else if(inst.opcode == 7)// JMP -> desvio
              {
                     machine->pc += inst.deslc;
              }
              else if(inst.opcode == 8)// JGZ -> desvio se maior
              {
                     if(machine->ac > 0) {
                            machine->pc += inst.deslc;
                     }
              }
              else if(inst.opcode == 9) // JLZ -> desvio se menor
              {
                     if(machine->ac < 0) {
                            machine->pc += inst.deslc;
                     }
              }
              else if(inst.opcode == 10)// JZE -> desvio se igual
              {
                     if(machine->ac == 0) {
                            machine->pc = inst.deslc;
                     }
              }
              else if(inst.opcode == 11)// HLT -> pare
              {
                     ret  = -1;
              }
              else if(inst.opcode == 12) // LXD -> carrega RX
              {
                     temp = machine->pc + inst.deslc;
                     machine->rx = machine->mem[temp];
              }
              else if(inst.opcode == 13) // SXD -> armazena RX
              {
                     temp = machine->pc + inst.deslc;
                     machine->mem[temp] = machine->rx;
              }
              else if(inst.opcode == 14) // LAX -> carrega AC indexado
              {
                     machine->ac = machine->mem[machine->rx];
                     machine->rx++;
              }
              else if(inst.opcode == 15) // SAX -> armazena AC indexado
              {
                     machine->mem[machine->rx] = machine->ac;
                     machine->rx++;
              }
              else if(inst.opcode == 16) // LCD -> carrega RC
              {
                     temp = machine->pc + inst.deslc;
                     machine->rc = machine->mem[temp];
              }
              else if(inst.opcode == 17) // JCC -> conta e desvia
              {
                     machine->rc--;
                     if(machine->rc > 0) {
                            machine->pc = machine->pc + inst.deslc;
                     }
              }
              else if(inst.opcode == 18) // CAL -> chamada de funçao
              {
                     machine->rx = machine->pc;
                     machine->pc = machine->pc + inst.deslc;
              }
              else if(inst.opcode == 19) // RET -> retorno da funçao
              {
                     machine->pc = machine->rx;
              }
              else if(inst.opcode == 20) // LAI -> carrega AC indireto
              {
                     machine->ac = machine->mem[machine->mem[machine->rx]];
                     machine->rx++;
                     // int i = getInstruction(machine);
                     // printf("GET = %d\n\n", i);
              }
              else if(inst.opcode == 21) // SAI -> armazena AC indireto
              {
                     machine->mem[machine->mem[machine->rx]] = machine->ac;
                     machine->rx++;
              }
              else
              {
                     ret = 0;
              }
       return ret;
}
