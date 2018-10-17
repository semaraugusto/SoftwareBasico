#include <stdio.h>

struct X2 {
       long l;
       char c;
};
struct X4 {
       char c1;
       char c2;
       char c3;
} x;

struct X6 {
       short s1;
       int i;
       char c[3];
       short s2;
};

void dump (void *p, int n) {
       unsigned char *p1 = p;
       while (n--) {
              printf("%d ", *p1);
              p1++;
       }
       printf("\n");
}

union U1 {
       short s;
       char c[5];
};

int main (void) {
       int i,j;
       short a[2][3];
       int b[2];

       union U1 teste;

       struct X6 stru;

       for (i=0;i<2;i++) {
              b[i] = i;
              for (j=0;j<3;j++)
                     a[i][j] = 3*i+j;
       }
       printf ("b: %lu\n", sizeof(struct X2));
       dump (b, sizeof(b));
       printf ("a: %lu\n", sizeof(struct X4));
       dump (a, sizeof(a));
       printf("c: %lu\n", sizeof(teste));
       dump(&teste, sizeof(teste));
       return 0;
}
// basicamente a função dump acessa cada byte da memória separadamente.
// no caso o i = 10.000, em binário i =     00100111 00010000. como 00010000, o primeiro byte é 16 em decimal,
// e 00100111 em decimal é 39, então ele imprime 16, 39, 0, 0, já que os últimos 2 bytes são somente 0's.

// a variável c é guardada em somente um byte, i é guardada em 4 bytes diferentes e o vetor v é guardado em 5
// bytes. Um para cada char.

// 1 - o vetor b possui 8 bytes e estão alocados em múltiplos de 4 bytes.
// o vetor a possui 12 bytes e também está alocado em múltiplos de 4 bytes.


//  2 - há um buraco entre o parâmetro b e o c porque a memória sempre aloca em múltiplos de 4 bytes.
// como o short somente possui 2 bytes de tamanho, o compilador usa uma técnica chamada
// padding que preenche os 2 bytes faltantes do short.

// 3 -
