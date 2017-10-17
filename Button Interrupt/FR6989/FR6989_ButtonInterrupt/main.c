#include <msp430.h> 
void main (void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;                            // Set P1.0 to output direction
    P1OUT &= ~BIT0;                           // Set LED1 low
    P9DIR |= BIT7;                            // Set P9.7 to output direction
    P9OUT |= BIT7;                            // Set LED2 High

    P1REN |= BIT1;                            // Enable P1.1 internal resistance
    P1OUT |= BIT1;                            // Set P1.1 as pull-Up resistance
    P1IES &= ~BIT1;                           // P1.1 Lo/Hi edge
    P1IFG &= ~BIT1;                           // P1.1 IFG cleared
    P1IE |= BIT1;                             // P1.1 interrupt enabled

    __bis_SR_register( GIE );                 // Enable interrupts globally
}

// Port 1 interrupt
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
    case P1IV_P1IFG1:                                       // Pin 1 (button 2)
        P1OUT^=BIT0;                        //Flip LED1
        P9OUT^=BIT7;                        //Flip LED2
        __delay_cycles(1000);                  // delay cycles to allow for button bounce without reinterrupt
        P1IFG &= ~BIT1;                        // reset flag for interrupt
        break;
    default:   _never_executed();

    }

}
