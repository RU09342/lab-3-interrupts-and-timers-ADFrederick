#include <msp430.h>



int press = 0;
int count = 0;
int overflows = 0;
int n = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer


    P1DIR |= 0x01; // LED Output

    P1DIR &= 0xFD; // Button input
    P1REN |= 0x02;//Enable resistor
    P1OUT |= 0x02;//Set resistor as a pull up

    P1IES |= 0x02; // start interrupt
    P1IE |= 0x02; // Enable interrupt for button

    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_1; // Divides SMclk by 2 set in up mode
    n = 250000 / 10;
    TA0CCR0 = n; // ~10Hz

    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// TIMER interrupt
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    if (press > 0)
    {
        if (P1IN & 0x02) // if button isnt pressed clear variables
        {
            press = 0;
            count = 0;
        }

        else    // count cycles for button held down
        {
            P1OUT |= 0x01;
            overflows++;
        }


    }

    else //Start new counter when button released
    {
        if (count == overflows) // when the two counts equal each other flip the LED
        {
            P1OUT ^= 0x01;
            count = 0;
        }

        else
            count++;
    }
}

// Button interrupt
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
    press = 1;
    overflows = 0;
    P1IFG &= 0xFD; // Clear interrupt flag
}
