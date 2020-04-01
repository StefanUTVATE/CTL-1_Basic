#include <msp430.h>


/**
 * RX_Enable.c
 * All MSP430 Microcontroller Pin Addresses are found on the schematic of the CTL-1 Board
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer. This would timeout our program if it was on

    //Initial Settings
    P3OUT |= 0x02;                  // Turn Output Enable for VoltTranslator OFF, just to be safe
    P2OUT = 0x00;                  // Keep all Modules in Standby by pulling both RX and TX Low
    P6OUT = 0x00;                   // Keep all CLKs and MOSI pulled low
    P4OUT = 0xFF;                   // Keep all modules CSB and LDB pulled high, meaning they cannot be written to

    //Inputs and Outputs
    P2DIR |= 0xFF;                  // configure RX and TX enable pins to be outputs
    P6DIR |= 0xFF;                  // configure CLKs and MSOI to be outputs
    P4DIR |= 0xFF;                  // configure CSB and LDB to be outputs
    P3DIR |= 0x07;                  // configure VoltTranslator Enable Pin and Debug LEDs as output

    //Global Variables
    volatile unsigned int i;        // int is volatile to prevent optimization by compiler
    volatile unsigned int j;        // int is volatile to prevent optimization by compiler
    unsigned int  half_second = 100000000;
    unsigned int  split_second = 1000000;
    unsigned int  mili_second = 10;
    unsigned int  clk_cycles = 48*16*16+7; //48 bits, 16 elements, 16 modules, 7 for good measure

    //Set all RX enables, one by one
    P3OUT = 0x00;                  // Set Output Enable ON
    for(i=half_second; i>0; i--);     // delay for half a second
    P2OUT = 0x01;                  // Set RX direction for Connector A
    for(i=half_second; i>0; i--);     // delay for half a second
    P2OUT = 0x05;                  // Set RX direction for Connector B
    for(i=half_second; i>0; i--);     // delay for half a second
    P2OUT = 0x15;                  // Set RX direction for Connector C
    for(i=half_second; i>0; i--);     // delay for half a second
    P2OUT = 0x55;                  // Set RX direction for Connector D
    P3OUT = 0x01; //Setting first LED on to indicate RX is enabled

    //Set all Elements Register to Zeros
    P4OUT = 0xAA;               //Hold CSB Low and LDB high
    for(i=split_second; i>0; i--); // delay for a split second
    for(i=clk_cycles; i>0; i--) //Clock in a lot of zeros
    {
        P6OUT ^= 0xAA;
        for(j=mili_second; j>0; j--); // delay for a mili second, gets
    }
    P6OUT = 0x00;               // Keep all CLKs and MOSI pulled low from now on
    P3OUT ^= 0x01;              // toggle LEDs (04 and 01, so 05 to turn on both)
    for(i=split_second; i>0; i--); // delay for a split second
    P4OUT = 0xFF;               //Hold CSB High and LDB high
    P3OUT ^= 0x05;              // toggle LEDs (04 and 01, so 05 to turn on both)
    for(i=half_second; i>0; i--); // delay for half a second
    P4OUT = 0x55;               //Pull LDB low
    for(i=half_second; i>0; i--); // delay for half a second
    P4OUT = 0xFF;               //Hold CSB High and LDB high
    P3OUT = 0x05;              // toggle LEDs (04 and 01, so 05 to turn on both)

}
