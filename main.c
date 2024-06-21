#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// stop watchdog timer
	
    P6DIR = 0x0;   // motor direction control from light
    P2DIR |= BIT2; // motor drive CCW
    P2DIR |= BIT4; // motor drive CW
	P1DIR |= BIT0;  // light for testing
	P4DIR |= BIT0; // turn on motor

	int state = 1;  // is 1 if lights off, is 0 if lights on
	// set up adc12
    ADC12CTL0 = ADC12SHT02 + ADC12ON;   // sampling time and ADC12 on
    ADC12CTL1 = ADC12SHP;               // sampling signal sourced from the sampling timer instead of sampling input signal
    ADC12CTL0 |= ADC12ENC;              // ADC enable
    P6SEL |= 0x01;                      // P6.0 allow ADC on pin 6.0
    ADC12MCTL0 = ADC12INCH_0 + ADC12REFON; // ADC12INCH_0 selects A0 panel and ADC12REF0 sets ref voltage to 5V


  while (1)
  {

    ADC12CTL0 |= ADC12SC;         // start sampling
    while (ADC12CTL1 & ADC12BUSY);// while bit ADC12BUSY in register ADC12CTL1 is high; wait

    if (ADC12MEM0 > 2457){         // 2457 is 3V
        P1OUT = BIT0;
        state = open(state);      // lights on - open the blinds
        __delay_cycles(1500000); // delay

    } else {
        P1OUT = ~BIT0;
        state = close(state);      // lights off - close the blinds
        __delay_cycles(1500000); // delay
    }

  }

}

int close(int state) { // CW
   if (state == 0){
        P4OUT = BIT0;  // activate relay for power
        P2OUT = ~BIT2; // drive the motor one way
        P2OUT = BIT4;
        __delay_cycles(6000000);
        P4OUT = ~BIT0; // deactivate relay for power
   }
   return 1;
}

int open(int state) { // CCW
    if (state == 1) {
        P4OUT = BIT0; // activate relay for power
        P2OUT = BIT2; // drive the motor one way
        P2OUT = ~BIT4;
        __delay_cycles(6200000);
        P4OUT = ~BIT0; // deactivate relay for power
    }
    return 0;
}
