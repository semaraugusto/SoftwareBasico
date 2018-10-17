CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -std=c99 -pedantic

TARGET_MONTADOR = S2
TARGET_SIMULADOR = M2
OBJECTS_MONTADOR = mainMontador.o montData.o
OBJECTS_SIMULADOR = mainSimulador.o interpreter.o

VFLAGS =
SIM = ./M2 restoDivisaoParteB3.m2 0 < input
MONT = ./S2 restoDivisaoParteB3.s2 restoDivisaoParteB3.m2

default: all

all: $(OBJECTS_SIMULADOR) $(OBJECTS_MONTADOR)
	$(CC) $(CFLAGS) $(OBJECTS_MONTADOR) -o $(TARGET_MONTADOR)
	$(CC) $(CFLAGS) $(OBJECTS_SIMULADOR) -o $(TARGET_SIMULADOR)
	rm -f *.o

mainMounter.o: mainMontador.c montData.h
	$(CC) $(CFLAGS) -c mainMounter.c

mounterData.o: montData.c montData.h
	$(CC) $(CFLAGS) -c montData.c

mainAssembler.o: mainSimulador.c interpreter.h
	$(CC) $(CFLAGS) -c mainSimulador.c

interpreter.o: interpreter.c interpreter.h
	$(CC) $(CFLAGS) -c interpreter.c

# execute o montador
mont:
	$(MONT)
#execute o assembler
sim:
	$(SIM)
valgrind:
	valgrind $(VFLAGS) $(EXEC)
time:
	time $(EXEC)
