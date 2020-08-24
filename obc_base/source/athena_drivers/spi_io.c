#include "spi_io.h"
#include "HL_spi.h"
#include "HL_gio.h"
#include "HL_het.h"

/******************************************************************************
 Module Public Functions - Low level SPI control functions
******************************************************************************/



void SPI_Init (void) {

}

BYTE SPI_RW (BYTE d) {
    while ((spiREG1->FLG & 0x0200) == 0); // Wait until TXINTFLG is set for previous transmission
    spiREG1->DAT1 = d | 0x100D0000;    // transmit register address

    while ((spiREG1->FLG & 0x0100) == 0); // Wait until RXINTFLG is set when new value is received
    return((unsigned char)spiREG1->BUF);  // Return received value
}

void SPI_Release (void) {
    WORD idx;
    for (idx=512; idx && (SPI_RW(0xFF)!=0xFF); idx--);
}

inline void SPI_CS_Low (void) {
    gioSetBit(hetPORT2, 6, 0); //CS LOW
}

inline void SPI_CS_High (void){
    gioSetBit(hetPORT2, 6, 1); //CS HIGH
}

inline void SPI_Freq_High (void) {

}

inline void SPI_Freq_Low (void) {

}

void SPI_Timer_On (WORD ms) {
//    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;  // Make sure clock is enabled
//    LPTMR0_CSR = 0;                     // Reset LPTMR settings
//    LPTMR0_CMR = ms;                    // Set compare value (in ms)
//    // Use 1kHz LPO with no prescaler
//    LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;
//    // Start the timer and wait for it to reach the compare value
//    LPTMR0_CSR = LPTMR_CSR_TEN_MASK;
}

inline BOOL SPI_Timer_Status (void) {
    return TRUE;
}

inline void SPI_Timer_Off (void) {
//    LPTMR0_CSR = 0;                     // Turn off timer
}

