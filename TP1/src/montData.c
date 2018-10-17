#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "montData.h"


void initMounter(mounter *m) {
       int i, j;
       for(i = 0; i < TABLESIZE; i++) {
              m->table[i].address = -1;
              m->table[i].label = '\0';
              m->varTable[i].var = '\0';
              for(j = 0; j < INSTRUCTIONSIZE; j++) {
                     m->table[i].token[j] = '\0';
              }
       }


       m->numVariables = 0;
       m->numLabels = 0;

       return;
}

// primeira passada no arquivo para salvar a tabela de simbolos.
// retorna o tamanho da tabela.
int createSymbolTable(mounter *m, char *argv[], int argc) {
       FILE *file;
       argc++; // silenciando warning de argumento não usado.
       argc--;
       file = fopen(argv[1], "r");

       char line[LINESIZE];

       int pointer = 0, pc = 0;

              fgets(line, 100, file);
              while(!feof(file)) {
                     // não tem label
                     if(line[0] == ' ') {
                            m->table[pointer].label = '\0';
                            m->table[pointer].token[0] = line[2];
                            m->table[pointer].token[1] = line[3];

                            if(line[4] == '\n') {// é uma operação de não depende de operando;
                                   m->table[pointer].address = pc;
                                   pc++;
                                   m->table[pointer].operand = '\0';
                            }

                            else if(line[4] == ' ') { // é uma pseudoinstrução do tipo DA DS ou DC
                                   m->table[pointer].address = pc;
                                   pc++;

                                   m->table[pointer].token[2] = '\0';
                                   m->table[pointer].token[3] = '\0';
                                   // será uma constante. trato disso depois.
                                   m->table[pointer].operand = line[6];
                            }
                            else {
                                   m->table[pointer].address = pc;
                                   pc += 2;

                                   m->table[pointer].token[2] = line[4];
                                   m->table[pointer].token[3] = '\0';
                                   // pode ser uma variável ou uma constante.
                                   m->table[pointer].operand = line[6];
                            }

                     }
                     // tem label
                     else {
                            m->table[pointer].label = line[0];

                            m->table[pointer].token[0] = line[2];
                            m->table[pointer].token[1] = line[3];

                            if(line[4] == '\n') {
                                   m->table[pointer].address = pc;
                                   pc++;

                                   m->table[pointer].operand = '\0';
                            }

                            else {
                                   m->table[pointer].operand = line[6];
                                   if(line[4] == ' ') {// é uma pseudo instrução do tipo DA DS ou DC
                                          m->table[pointer].address = pc;
                                          pc++;

                                          m->table[pointer].token[2] = '\0';
                                          m->table[pointer].token[3] = '\0';

                                          if(m->table[pointer].token[0] == 'D' && m->table[pointer].token[1] == 'C') {
                                                 m->varTable[m->numVariables].var = m->table[pointer].label;
                                                 m->varTable[m->numVariables].pos = m->table[pointer].address;
                                                 m->varTable[m->numVariables].value = atoi(&m->table[pointer].operand);
                                                 m->numVariables++;
                                          }

                                   }
                                   // não é uma pseudoinstrução
                                   else {
                                          m->table[pointer].address = pc;
                                          pc += 2;

                                          m->table[pointer].token[2] = line[4];
                                          m->table[pointer].token[3] = '\0';

                                          // como tem label e não é uma pseudoinstrução de declaraçao de variavel
                                          // colocando a label na tabela de labels.
                                          m->labelTable[m->numLabels].label = m->table[pointer].label;
                                          m->labelTable[m->numLabels].pos = m->table[pointer].address;
                                          m->numLabels++;
                                   }
                            }
                     }

                     // if(m->table[pointer].operand == '\0') {
                     //        pc++;
                     //        m->table[pointer].address = pc;
                     // }
                     // else {
                     //        pc += 2;
                     //        m->table[pointer].address = pc;
                     // }
                     // // if(m->table[pointer].label != '\0') {
                     // //        m->labelTable[m->numLabels].label = m->table[pointer].label;
                     // //        m->labelTable[m->numLabels].pos = m->table[pointer].address;
                     // //        m->numLabels++;
                     // // }
                     // // declarando as variáveis
                     // if(m->table[pointer].token[0] == 'D' && m->table[pointer].token[1] == 'C') {
                     //        m->varTable[m->numVariables].var = m->table[pointer].label;
                     //        m->varTable[m->numVariables].pos = m->table[pointer].address;
                     //        m->varTable[m->numVariables].value = atoi(&m->table[pointer].operand);
                     //        m->numVariables++;
                     // }

                     // zprintf("%d\t%c %s %c\n", pc, m->table[pointer].label, m->table[pointer].token, m->table[pointer].operand);

                     pointer++;
                     fgets(line, 100, file);


              }

       pointer++;

       fclose(file);
       return pointer;

}

void runProgram(mounter *m, char *argv[], int tableSize) {
       FILE *out;
       out = fopen(argv[2], "w");
       int aux, i, j;

       for(i = 0; i < tableSize; i++) {
              printf("%c %s %c\n", m->table[i].label, m->table[i].token, m->table[i].operand);
              if(!strcmp(m->table[i].token,"END")) {
                     break;
              }

              if(strcmp(m->table[i].token,"LAD") == 0) {
                     // achando a variável a ser colocada no acumulador
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 01, aux);

              }

              if(!strcmp(m->table[i].token,"SAD")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 02, aux);
              }

              if(!strcmp(m->table[i].token,"ADD")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 03, aux);
              }

              if(!strcmp(m->table[i].token,"SUB")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 04, aux);
              }

              if(!strcmp(m->table[i].token,"INP")) {
                     // achando a variável a ser colocada no acumulador
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 05, aux);

              }

              if(!strcmp(m->table[i].token,"OUT")) {
                     // achando a variável a ser colocada no acumulador
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 06, aux);

              }

              if(!strcmp(m->table[i].token,"JMP")) {
                     for(j = 0; j < m->numLabels; j++) {
                            if(m->labelTable[j].label == m->table[i].operand)
                                   break;
                     }
                     aux = m->labelTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 07, aux);

              }

              if(!strcmp(m->table[i].token,"JGZ")) {
                     for(j = 0; j < m->numLabels; j++) {
                            if(m->labelTable[j].label == m->table[i].operand)
                                   break;
                     }

                     aux = m->labelTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 8, aux);

              }

              if(!strcmp(m->table[i].token,"JLZ")) {
                     for(j = 0; j < m->numLabels; j++) {
                            if(m->labelTable[j].label == m->table[i].operand)
                                   break;
                     }

                     aux = m->labelTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 9, aux);

              }

              if(!strcmp(m->table[i].token,"JZE")) {
                     for(j = 0; j < m->numLabels; j++) {
                            if(m->labelTable[j].label == m->table[i].operand)
                                   break;
                     }

                     aux = m->labelTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 10, aux);

              }

              if(!strcmp(m->table[i].token,"HLT")) {
                     fprintf(out, "%02d %02d\n", 11, 00);
              }

              if(!strcmp(m->table[i].token,"LXD")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 12, aux);
              }

              if(!strcmp(m->table[i].token,"SXD")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 13, aux);
              }

              // HELP HELP HELP HELP HELP como achar o valor de M pra ser colocado após 14?
              if(!strcmp(m->table[i].token,"LAX")) {

                     fprintf(out, "%02d %02d\n", 14, 00);
              }

              // HELP HELP HELP HELP HELP como achar o valor de M pra ser colocado após 15?
              if(!strcmp(m->table[i].token,"SAX")) {

                     fprintf(out, "%02d %02d\n", 15, 00);
              }

              if(!strcmp(m->table[i].token,"LCD")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     aux = m->varTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 16, 00);
              }

              if(!strcmp(m->table[i].token,"JCC")) {

                     // achando a label para onde o programa vai
                     for(j = 0; j < m->numLabels; j++) {
                            if(m->labelTable[j].label == m->table[i].operand)
                                   break;
                     }
                     aux = m->labelTable[j].pos - m->table[i].address - 2;

                     fprintf(out, "%02d %02d\n", 17, aux);
              }

              if(!strcmp(m->table[i].token,"CAL")) {

                     // achando a label para onde o programa vai
                     for(j = 0; j < m->numLabels; j++) {
                            if(m->labelTable[j].label == m->table[i].operand)
                                   break;
                     }
                     aux = m->labelTable[j].pos - m->table[i].address - 2;
                     fprintf(out, "%02d %02d\n", 18, aux);
              }

              if(!strcmp(m->table[i].token,"RET")) {

                     fprintf(out, "%02d %02d\n", 19, 00);
              }

              if(!strcmp(m->table[i].token,"LAI")) {

                     fprintf(out, "%02d %02d\n", 20, 00);
              }

              if(!strcmp(m->table[i].token,"SAI")) {

                     fprintf(out, "%02d %02d\n", 21, 00);
              }

              if(!strcmp(m->table[i].token, "DC")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].label)
                                   break;
                     }
                     fprintf(out, "%c\n", m->table[i].operand);
              }

              if(!strcmp(m->table[i].token, "DA")) {
                     for(j = 0; j < m->numVariables; j++) {
                            if(m->varTable[j].var == m->table[i].operand)
                                   break;
                     }
                     fprintf(out, "%d\n", m->varTable[j].pos);
              }
       }

       fclose(out);
       return;
}
