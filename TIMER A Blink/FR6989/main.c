#include <msp430.h> 



unsigned int timerCount = 0;
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //unlock GPIO
    P1DIR |= 0x01;
    P1OUT &= 0x00; // Set the LEDs off


    TBCTL = TBSSEL_2 + MC_1 + ID_3;           // SMCLK/8, upmode
    TB0CCTL0 = CCIE;// CCR0 interrupt enabled
    TB0CCR0 =  0xFFFF;  //Number timer counts to


    __bis_SR_register(GIE); // LPM0 with interrupts enabled
    while(1)                      //Loop
      {}
}


// Timer B0
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR (void)
{
    P1OUT ^= BIT0;                            // Toggle P1.0
}
