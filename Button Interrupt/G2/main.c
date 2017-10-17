#include <msp430.h> 
void main (void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer


    P1DIR |= BIT0;                            // Set P1.0 to output direction
    P1OUT &= ~BIT0;                           // Set LED1 low
    P1DIR |= BIT6;                            // Set P1.6 to output direction
    P1OUT |= BIT6;                            // Set LED2 High

    P1REN |= BIT3;                            // Enable P1.3 internal resistance
    P1OUT |= BIT3;                            // Set P1.3 as pull-Up resistance
    P1IES &= ~BIT3;                           // P1.3 Lo/Hi edge
    P1IFG &= ~BIT3;                           // P1.3 IFG cleared
    P1IE |= BIT3;                             // P1.3 interrupt enabled

    __bis_SR_register( GIE );                 // Enable interrupts globally
}

// Port 1 interrupt
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    switch( __even_in_range(P1IV_P1IFG1 )) {
    case P1IV_P1IFG1:                                       // Pin 1 (button 2)
        P1OUT^=BIT0;                        //Flip LED1
        P1OUT^=BIT6;                        //Flip LED2
        __delay_cycles(1000);                  // delay cycles to allow for button bounce without reinterrupt
        P1IFG &= ~BIT3;                        // reset flag for interrupt
        break;
    default:   _never_executed();

    }

}
