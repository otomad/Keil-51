/*---------------------------------------------------------------------------
;	Copyright (C) 2001 CYGNAL INTEGRATED PRODUCTS, INC.
; 	All rights reserved.
;
;
; 	FILE NAME  	: C8051F300.H 
; 	TARGET MCUs	: C8051F300, 'F301, 'F302, 'F303 
; 	DESCRIPTION	: Register/bit definitions for the C8051F30x product family.  
;
; 	REVISION 1.1 	
;
;---------------------------------------------------------------------------*/

/*  BYTE Registers  */
sfr P0       = 0x80;    /* PORT 0                                                   */	
sfr SP       = 0x81;    /* STACK POINTER                                            */
sfr DPL      = 0x82;    /* DATA POINTER - LOW BYTE                                  */
sfr DPH      = 0x83;    /* DATA POINTER - HIGH BYTE                                 */
sfr PCON     = 0x87;    /* POWER CONTROL                                            */
sfr TCON     = 0x88;    /* TIMER CONTROL                                            */
sfr TMOD     = 0x89;    /* TIMER MODE                                               */
sfr TL0      = 0x8A;    /* TIMER 0 - LOW BYTE                                       */
sfr TL1      = 0x8B;    /* TIMER 1 - LOW BYTE                                       */
sfr TH0      = 0x8C;    /* TIMER 0 - HIGH BYTE                                      */	
sfr TH1      = 0x8D;    /* TIMER 1 - HIGH BYTE                                      */
sfr CKCON    = 0x8E;    /* CLOCK CONTROL                                            */
sfr PSCTL    = 0x8F;    /* PROGRAM STORE R/W CONTROL                                */
sfr SCON0    = 0x98;    /* SERIAL PORT 0 CONTROL                                    */
sfr SBUF0    = 0x99;    /* SERIAL PORT 0 BUFFER                                     */
sfr CPT0MD   = 0x9D;    /* COMPARATOR 0 MODE                                        */
sfr CPT0MX   = 0x9F;    /* COMPARATOR 0 MUX                                         */
sfr P0MDOUT  = 0xA4;    /* PORT 0 OUTPUT MODE                                       */
sfr IE       = 0xA8;    /* INTERRUPT ENABLE                                         */
sfr OSCXCN   = 0xB1;    /* EXTERNAL OSCILLATOR CONTROL                              */
sfr OSCICN   = 0xB2;    /* INTERNAL OSCILLATOR CONTROL                              */
sfr OSCICL   = 0xB3;    /* INTERNAL OSCILLATOR CALIBRATION                          */
sfr FLSCL    = 0xB6;    /* FLASH SCALE                                         */
sfr FLKEY    = 0xB7;    /* FLASH LOCK & KEY                                         */
sfr IP       = 0xB8;    /* INTERRUPT PRIORITY                                       */
sfr AMX0SL   = 0xBB;    /* ADC 0 MUX CHANNEL SELECTION                              */
sfr ADC0CF   = 0xBC;    /* ADC 0 CONFIGURATION                                      */
sfr ADC0     = 0xBE;    /* ADC 0 DATA                                               */
sfr SMB0CN   = 0xC0;    /* SMBUS CONTROL                                            */
sfr SMB0CF   = 0xC1;    /* SMBUS CONFIGURATION                                      */
sfr SMB0DAT  = 0xC2;    /* SMBUS DATA                                               */
sfr ADC0GT   = 0xC4;    /* ADC0 GREATER-THAN                                        */
sfr ADC0LT   = 0xC6;    /* ADC0 LESS-THAN                                           */
sfr TMR2CN   = 0xC8;    /* TIMER 2 CONTROL                                          */
sfr TMR2RLL  = 0xCA;    /* TIMER 2 RELOAD LOW                                       */
sfr TMR2RLH  = 0xCB;    /* TIMER 2 RELOAD HIGH                                      */
sfr TMR2L    = 0xCC;    /* TIMER 2 LOW BYTE                                         */
sfr TMR2H    = 0xCD;    /* TIMER 2 HIGH BYTE                                        */
sfr PSW      = 0xD0;    /* PROGRAM STATUS WORD                                      */
sfr REF0CN   = 0xD1;    /* VOLTAGE REFERENCE 0 CONTROL                              */
sfr PCA0CN   = 0xD8;    /* PCA0 CONTROL                                             */
sfr PCA0MD   = 0xD9;    /* PCA0 MODE                                                */
sfr PCA0CPM0 = 0xDA;    /* PCA0 MODULE 0 MODE                                       */
sfr PCA0CPM1 = 0xDB;    /* PCA0 MODULE 1 MODE                                       */
sfr PCA0CPM2 = 0xDC;    /* PCA0 MODULE 2 MODE                                       */
sfr ACC      = 0xE0;    /* ACCUMULATOR                                              */
sfr XBR0     = 0xE1;    /* DIGITAL CROSSBAR CONFIGURATION REGISTER 0                */
sfr XBR1     = 0xE2;    /* DIGITAL CROSSBAR CONFIGURATION REGISTER 1                */
sfr XBR2     = 0xE3;    /* DIGITAL CROSSBAR CONFIGURATION REGISTER 2                */
sfr IT01CF   = 0xE4;    /* INT0/INT1 CONFIGURATION                                  */
sfr EIE1     = 0xE6;    /* EXTERNDED INTERRUPT ENABLE 1                              */
sfr ADC0CN   = 0xE8;    /* ADC 0 CONTROL                                            */	
sfr PCA0CPL1 = 0xE9;    /* PCA0 MODULE 1 CAPTURE/COMPARE REGISTER LOW BYTE          */
sfr PCA0CPH1 = 0xEA;    /* PCA0 MODULE 1 CAPTURE/COMPARE REGISTER HIGH BYTE         */
sfr PCA0CPL2 = 0xEB;    /* PCA0 MODULE 2 CAPTURE/COMPARE REGISTER LOW BYTE          */
sfr PCA0CPH2 = 0xEC;    /* PCA0 MODULE 2 CAPTURE/COMPARE REGISTER HIGH BYTE         */
sfr RSTSRC   = 0xEF;    /* RESET SOURCE                                             */
sfr B        = 0xF0;    /* B REGISTER                                               */
sfr P0MDIN   = 0xF1;    /* PORT 0 INPUT MODE REGISTER                               */
sfr EIP1     = 0xF6;    /* EXTERNAL INTERRUPT PRIORITY 1                              */
sfr CPT0CN   = 0xF8;    /* COMPARATOR 0 CONTROL                                     */
sfr PCA0L    = 0xF9;    /* PCA0 COUNTER REGISTER LOW BYTE                           */
sfr PCA0H    = 0xFA;    /* PCA0 COUNTER REGISTER HIGH BYTE                          */
sfr PCA0CPL0 = 0xFB;    /* PCA MODULE 0 CAPTURE/COMPARE REGISTER LOW BYTE           */
sfr PCA0CPH0 = 0xFC;    /* PCA MODULE 0 CAPTURE/COMPARE REGISTER HIGH BYTE          */

/*  BIT Registers  */

/*  TCON  0x88 */
sbit IT0     = TCON ^ 0;   /* EXT INTERRUPT 0 TYPE                                  */
sbit IE0     = TCON ^ 1;   /* EXT INTERRUPT 0 EDGE FLAG                             */
sbit IT1     = TCON ^ 2;   /* EXT INTERRUPT 1 TYPE                                  */
sbit IE1     = TCON ^ 3;   /* EXT INTERRUPT 1 EDGE FLAG                             */
sbit TR0     = TCON ^ 4;   /* TIMER 0 ON/OFF CONTROL                                */
sbit TF0     = TCON ^ 5;   /* TIMER 0 OVERFLOW FLAG                                 */
