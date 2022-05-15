/*
 * File:   prelab11.c
 * Author: Carlos
 *
 * Created on 11 de mayo de 2022, 04:30 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <stdint.h>
//CONSTANTES
#define _XTAL_FREQ 1000000
#define FLAG_SPI 0xFF
//VARIABLES
uint8_t dato = 0x15;
char cont_master = 0;
char cont_slace = 0xFF;
char val_temporal = 0;
//PROTOTIPO DE FUNCIONES
void setup (void);
//INTERRUPCIONES
void __interrupt() isr(void){
    if (PIR1bits.SSPIF){
        val_temporal = SSPBUF;
        SSPBUF = dato;   
        PIR1bits.SSPIF = 0;
    }  
    if (RBIF){
        if(PORTBbits.RB0 == 0){
            dato++;
        }
        if(PORTBbits.RB1 == 0){
            dato--;
        }
        RBIF = 0;
    }
    return;
}
//MAIN
void main(void) {
    setup();
    while(1){
    }
    return;
}
    

//CONFIGURACIÓN
void setup(void){
    ANSEL = 0b00000001;
    ANSELH = 0;
    
    OSCCONbits.IRCF = 0b100;
    OSCCONbits.SCS = 1;
    TRISA = 0b00100001;
    PORTA = 0;
    TRISB = 0b00000011;
    //CONFIGURACION SPI ESCLAVO
    TRISC = 0b00011000;
    PORTC = 0;
    
    OPTION_REGbits.nRBPU = 0;
    SSPCONbits.SSPM = 0b0100;
    SSPCONbits.CKP = 0;
    SSPCONbits.SSPEN = 1;
    
    SSPSTATbits.CKE = 1;
    SSPSTATbits.SMP = 0;
    
    
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
}
