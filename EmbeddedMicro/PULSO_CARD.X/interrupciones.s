    .include "p30F3013.inc"
    .GLOBAL  __T3Interrupt
    .GLOBAL  __ADCInterrupt
    
__T3Interrupt:
    BTG		LATD, #LATD8
    NOP
    
    BCLR	IFS0, #T3IF
    RETFIE


__ADCInterrupt:
    MOV		ADCBUF0, W0
    
    
    BCLR	IFS0, #ADIF
    RETFIE
    
    
    

