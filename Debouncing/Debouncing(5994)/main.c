#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Debouncing 5994
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog 

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            
    P1DIR |=BIT0; //set Port 1.0 output LED
    P1OUT &= ~(BIT0); // Toggle

    P5DIR &=~(BIT5); //set Button
    P5REN|=BIT5;//enable pull-up/pull-down
    P1OUT|=BIT1; //choose pull-up

    P5IE |=BIT5; //enable interrupt
    P5IES |=BIT5; //setfalling edge
    P5IFG &=~(BIT5); //clear flag

    _BIS_SR(LPM4_bits + GIE); //Low Power Mode 4
}

#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    P1OUT ^=0x01; // Toggle
    P5IE &= ~BIT5; // disable interrupt
    
    //Debounce 1
    __delay_cycles(1);

    //Debounce 2
    TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, divider value 2
    TA0CCTL0 = 0x10; // set up compare mode 
    TA0CCR0 = 1000; // interrupt is disabled; duration disabled = 1000/16kHz = 1/16 sec

    P5IFG &=~(BIT5); // Clear flag
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P5IE |= BIT5; //re-enable interrupt

}
