#include <msp430>

short time = 0;

int main (void) {
  WDTCTL = WDTPW + WDTHOLD;

  P1DIR = 0x01; // na parte 2 é + 0x40.
  CCTL0 = CCIE;
  TACTL = TASSEL_2 + MC_3 + ID_3;

  TACCR0 = 62500; // 2* 62500 * 8micros

  _BIS_SR(CPUOFF + GIE);

// while(1);
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ParteA_1 (void) { // ativa o led no tempo 0 e desativa nos tempos 1 e 2.

  // P1OUT = 0x48 -> led verde ligado
  // P1OUT = 0x49 -> os 2 leds ligados
  // P1OUT = 0x09 -> só led vermelho ligado
  // P1OUT = 0x08 -> nenhum ligado.

  if(time == 0) {
    P1OUT = 0x01; // ativa o led
  }
  else {
    P1OUT = 0x00; // desativa led.
  }

  time = time++%3;
}

// para a parte 2.
// enable do pino - P1REM = 0x08;
// P1OUT = 0x08 // pra ativar o botão.
// P1IE = 0x08 // habilitando interrupcao do botao
// P1IFG = 0x00 // colocar isso no fim da interrupcao.
