#include <msp430.h> 
void main (void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;                            // Set P1.0 to output direction
    P1OUT &= ~BIT0;                           // Set LED1 low
    P1DIR |= BIT1;                            // Set P1.1 to output direction
    P1OUT |= BIT1;                            // Set LED2 High

    P5REN |= BIT6;                            // Enable P5.6 internal resistance
    P5OUT |= BIT6;                            // Set P5.6 as pull-Up resistance
    P5IES &= ~BIT6;                           // P5.6 Lo/Hi edge
    P5IFG &= ~BIT6;                           // P5.6 IFG cleared
    P5IE |= BIT6;                             // P5.6 interrupt enabled

    __bis_SR_register( GIE );                 // Enable interrupts globally
}

// Port 1 interrupt
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
    case P5IV_P5IFG6:                                       // P5.6
        P1OUT^=BIT0;                        //Flip LED1
        P1OUT^=BIT1;                        //Flip LED2
        __delay_cycles(1000);                  // delay cycles to allow for button bounce without reinterrupt
        P5IFG &= ~BIT6;                        // reset flag for interrupt
        break;
    default:   _never_executed();

    }

}
