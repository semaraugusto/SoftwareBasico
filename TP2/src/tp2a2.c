#include <msp430>

short time = 0;

int main(void) {
  WDTCTL = WDTPW + WDTHOLD;

  P1DIR = 0x01 + 0x40;
  CCTL0 = CCIE;
  TACTL = TASSEL_2 + MC_3 + ID_3;

  TACCR0 = 62500; // 2 * 62500 * 8us

  P1OUT = 0x08;
  P1IE = 0x08;

  _BIS_SR(CPUOFF + GIE);

  //while(1);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void ParteA_2_timer(void) {
  if(time < 8)
    P1OUT = 0x48;

  else if(time == 8)
    P1OUT = 0x49;

  else
    P1OUT = 0x09;

  time = time++%11;
}
#pragma vector = PORT1_VECTOR
__interrupt void Parte_A_2_botao(void) {
  if(time < 5)
    time = 5;

  P1IFG = 0x00;
}
