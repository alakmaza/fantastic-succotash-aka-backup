/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

#include "emac.h"

volatile Int32 RxCount = 0;     // RX count
volatile Int32 TxCount = 0;     // TX count
volatile Int32 ErrCount = 0;    // Error count
volatile Int32 ErrCode = 0;     // Error Code
volatile EMAC_Desc *pDescRx;    // Next descriptors to ACK in ISR
volatile EMAC_Desc *pDescTx;    // Next descriptors to ACK in ISR

/*
 *  Interrupt service routine
 */
void EMAC_isr( )
{
    Uint32 intr_flags;
    Uint32 tmp;

    INTC_EVTCLR0 = 0xffffffff;          // Clear all interrupts flags
    INTC_EVTCLR1 = 0xffffffff;
    INTC_EVTCLR2 = 0xffffffff;
    INTC_EVTCLR3 = 0xffffffff;

    EMAC_EWCTL = 0;                     // Disable EMAC/MDIO interrupts

    intr_flags = EMAC_MACINVECTOR;      // Check interrupt flag

    if ( intr_flags & EMAC_MACINVECTOR_HOSTPEND )
    {
        ErrCode = EMAC_MACSTATUS;       // Error code
        ErrCount++;
        return;
    }

    /*
     * Look for statistics interrupt
     */
    if ( intr_flags & EMAC_MACINVECTOR_STATPEND )
    {
        ErrCode = EMAC_MACSTATUS;       // Error code
        ErrCount++;
        return;
    }

    /*
     * Look for TX interrupt ( channel 0 )
     */
    if ( intr_flags & 0x00000001 )
    {
        tmp = EMAC_TX0CP;
        EMAC_TX0CP = tmp;

        /* This while loop is only used when ACK'ing more than one desc */
        while ( tmp != ( Uint32 )pDescTx )
        {
            if ( pDescTx->PktFlgLen & EMAC_DSC_FLAG_OWNER )
            {
                ErrCount++;
                return;
            }
            pDescTx++;
            TxCount++;
        }
        if ( pDescTx->PktFlgLen & EMAC_DSC_FLAG_OWNER )
        {
            ErrCount++;
            return;
        }
        pDescTx++;
        TxCount++;
    }

    /*
     * Look for RX interrupt ( channel 0 )
     */
    if ( intr_flags & 0x00000100 )
    {
        tmp = EMAC_RX0CP;
        EMAC_RX0CP = tmp;

        /* This while loop is only used when ACK'ing more than one desc */
        while ( tmp != ( Uint32 )pDescRx )
        {
            pDescRx++;
            RxCount++;
        }
        pDescRx++;
        RxCount++;
    }

    /* Enable EMAC/MDIO interrupts in wrapper */
    EMAC_EWCTL = 1;
    return;
}

