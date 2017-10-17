#include <msp430.h>



int press = 0;
int count = 0;
int overflows = 0;
int n = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //unlocl GPIO


    P1DIR |= 0x01; // LED Output

    P5DIR &= 0xAF; // Button input
    P5REN |= 0x40;//Enable resistor
    P5OUT |= 0x40;//Set resistor as a pull up

    P5IES |= 0x40; // start interrupt
    P5IE |= 0x40; // Enable interrupt for button

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
        if (P5IN & 0x40) // if button isnt pressed clear variables
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
#pragma vector = PORT5_VECTOR
__interrupt void PORT_5(void)
{
    press = 1;
    overflows = 0;
    P5IFG &= 0xAF; // Clear interrupt flag
}
