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
#define _XTAL_FREQ 500000
#define FLAG_SPI 0xFF
#define IN_MIN 0
#define IN_MAX 255
#define OUT_MIN 125
#define OUT_MAX 250
//VARIABLES
unsigned short CCPR_UNO = 0;
uint8_t dato = 0x15;
char cont_master = 0;
char cont_slace = 0xFF;
char val_temporal = 0;
//PROTOTIPO DE FUNCIONES
void setup (void);
unsigned short map(uint8_t val, uint8_t in_min, uint8_t in_max, unsigned short out_min, unsigned short out_max);
//INTERRUPCIONES
void __interrupt() isr(void){
    if (PIR1bits.SSPIF){
        val_temporal = SSPBUF;   
        CCPR_UNO = map(val_temporal, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
        CCPR1L = (uint8_t)(CCPR_UNO>>2);
        CCP1CONbits.DC1B = CCPR_UNO & 0b11;
        
        PIR1bits.SSPIF = 0;
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
    TRISD = 0;
    PORTD = 0;
    //CONFIGURACION SPI ESCLAVO
    TRISC = 0b00011000;
    PORTC = 0;
    
    
    SSPCONbits.SSPM = 0b0100;
    SSPCONbits.CKP = 0;
    SSPCONbits.SSPEN = 1;
    
    SSPSTATbits.CKE = 1;
    SSPSTATbits.SMP = 0;
    
    
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //CONFIGURACIÓN DE PWM
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC1 = 1;
    PR2 = 156;
    
    //CONFIGURACIÓN DE CCP
    CCP1CON = 0;
    CCP1CONbits.P1M = 0;
    CCP1CONbits.CCP1M = 0b1100;
    CCPR1L = 125 >> 2;
    CCP1CONbits.DC1B = 125 & 0b11;
    
    PIR1bits.TMR2IF = 0;
    T2CONbits.T2CKPS = 0b01;
    T2CONbits.TMR2ON = 1;
    while (!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    __delay_us(40);
}
unsigned short map(uint8_t x, uint8_t x0, uint8_t x1, unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}

