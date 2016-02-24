/* 
 * File:   main_IR_Receptor.c
 * Author: micka
 *
 * Created on 9 mars 2015, 17:03
 * 
 * License CC-BY-NC-SA
 * https://creativecommons.org/licenses/by-nc-sa/3.0/fr/
 * 
 */

/* CONFIGURATION */
#pragma config CP    = OFF      // Flash Program Memory Code Protection bit (Code protection off)
#pragma config CCPMX = RB3      // CCP1 Pin Selection bit
#pragma config DEBUG = OFF      // In-Circuit Debugger Mode bit 
#pragma config WRT   = OFF      // Flash Program Memory Write Enable bits
#pragma config CPD   = OFF      // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config LVP   = OFF      // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE  = OFF      // Watchdog Timer Enable bit (WDT disabled)
#pragma config FOSC  = INTOSCIO

#define _XTAL_FREQ 8000000      // set your internal(or)external oscillator speed

#include <htc.h>
#include <xc.h>
#include <pic16f87.h>

/* Alias */
#define RECEP  PORTAbits.RA1
#define OUT1   PORTAbits.RA2
#define OUT2   PORTAbits.RA3
#define RED1   PORTBbits.RB2
#define GREEN1 PORTBbits.RB3
#define RED2   PORTBbits.RB5
#define GREEN2 PORTBbits.RB4

/* FONCTIONS */
void     init_oscillator (void);
unsigned change_out      (unsigned OUT);
void     init_code       (unsigned *code);

/*
 * MAIN
 */
void main(void) {

    init_oscillator();
    
    TRISAbits.TRISA1 = 1; //input of IR recepter
    TRISAbits.TRISA2 = 0; //output led
    TRISAbits.TRISA3 = 0; //output led
    
    TRISB = 0;
    TRISAbits.TRISA4 = 0; //output led
        
    unsigned signal_precedent = 1;
    
    unsigned code[8];
    
    /* set code to 0 */
    init_code(code);
    
    /* set outpu to 0 */
    OUT1 = 0;
    OUT2 = 0;
    GREEN1 = 0;RED1 = 1;
    GREEN2 = 0;RED2 = 1;
        
    while(1)
    { 
        
        /* code: 01010101 -> reception: 10101010 */
        if( code[0]==1 && code[1]==0 && code[2]==1 && code[3]==0 && code[4]==1 && code[5]==0 && code[6]==1 && code[7]==0 ){
            OUT1 = change_out(OUT1);
            if( OUT1==1 ){GREEN1=1;RED1=0;}else{GREEN1=0;RED1=1;}
            init_code(code);
        }
        /* code: 00011000 -> reception: 11100111 */
        else if( code[0]==1 && code[1]==1 && code[2]==1 && code[3]==0 && code[4]==0 && code[5]==1 && code[6]==1 && code[7]==1 ){
            OUT2 = change_out(OUT2);
            if( OUT2==1 ){ GREEN2=1;RED2=0; }else{ GREEN2=0;RED2=1; }
            init_code(code);
        }
        
        
        if( RECEP==0 ){
            __delay_ms(10);
            if( RECEP==0 && signal_precedent==1 ){
                __delay_ms(1);
                __delay_us(500); // delay to be at the midle
                
                code[0] = RECEP;
                __delay_ms(1);
                code[1] = RECEP;
                __delay_ms(1);
                code[2] = RECEP;
                __delay_ms(1);
                code[3] = RECEP;
                __delay_ms(1);
                code[4] = RECEP;
                __delay_ms(1);
                code[5] = RECEP;
                __delay_ms(1);
                code[6] = RECEP;
                __delay_ms(1);
                code[7] = RECEP;

                signal_precedent = 0;
                
                PORTAbits.RA4 = change_out(PORTAbits.RA4);
            }
            else{}
        }
        else{ signal_precedent=1; }
        
       
    }
}

/*
 * Init frenquency
 */
void init_oscillator(void){
    OSCCONbits.IRCF = 0b111; //8MHz
    OSCCONbits.SCS = 0b00; //Oscillator mode defined by FOSC
    OSCCONbits.IOFS = 1; //Frequency is stable
}

/*
 * Change out signal 
 */
unsigned change_out(unsigned OUT){
    if( OUT==1 ) return 0;
    else return 1;
}

/*
 * Set code to default: 00000000
 */
void init_code(unsigned *code){
    code[0] = 0;
    code[1] = 0;
    code[2] = 0;
    code[3] = 0;
    code[4] = 0;
    code[5] = 0;
    code[6] = 0;
    code[7] = 0;
}
