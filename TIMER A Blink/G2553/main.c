#include <msp430.h> 



unsigned int timerCount = 0;
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    P1DIR |= 0x41;
    P1OUT &= 0x00; // Set the LEDs off

                        // CCR0 interrupt enabled
    TACTL = TASSEL_2 + MC_1 + ID_3;           // SMCLK/8, upmode
    TA0CCTL0 = CCIE;
    TA0CCR0 =  0x0FFF;  //Number timer counts to


    __bis_SR_register(GIE);         // LPM0 with interrupts enabled
    while(1)                      //infinite loop
      {}
}


// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    P1OUT ^= BIT0;                            // Toggle LED1
}
