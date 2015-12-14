/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  TIMER interface
 */

#include "timer.h"

static TIMER_Handle TIMER_table[3] = {
    ( TIMER_Handle )&TIMER_MODULE_0,
    ( TIMER_Handle )&TIMER_MODULE_1,
    ( TIMER_Handle )&TIMER_MODULE_2
};

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _timer_init( timer_id )                                                  *
 *                                                                          *
 *      Initialize timer #timer_id                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 _timer_init( Uint32 timer_id )
{
    TIMER_Handle timer_handle = TIMER_table[timer_id];

    if ( timer_id == 0 )
    {
        EVMDM6437_clrPinMux( 0 , ( 3 << 16 ) );
        EVMDM6437_setPinMux( 0 , ( 1 << 16 ) );
    }
    if ( timer_id == 1 )
    {
        EVMDM6437_clrPinMux( 0 , ( 3 << 20 ) );
        EVMDM6437_setPinMux( 0 , ( 1 << 20 ) );
    }
    if ( timer_id == 2 )
    {

    }

    timer_handle->regs->TGCR   = 0;             // Stop TIMER
    timer_handle->regs->EMUMGT_CLKSPD = 0x0003; // Free emulation bit
    timer_handle->regs->TIM12  = 0;             // Reset counter
    timer_handle->regs->TIM34  = 0;             // Reset counter
    timer_handle->regs->PRD12  = 0xFFFFFFFF;    // Set period to Max
    timer_handle->regs->PRD34  = 0xFFFFFFFF;    // Set period to Max
    timer_handle->regs->TCR    = 0
        | ( 2 << 22 )                           // Enable34      [Continuous]
        | ( 1 << 8 )                            // Clock source? [TIMERIN]
        | ( 2 << 6 );                           // Enable12      [Continuous]
    timer_handle->regs->TGCR   = 0
        | ( 0 << 12 )                           // TDDR34
        | ( 0 << 8 )                            // PSC34
        | ( 0 << 2 )                            // TIMMODE
        | ( 1 << 1 )                            // TIM34RS [Enable]
        | ( 1 << 0 );                           // TIM12RS [Enable]

    CFG_TIMERCTL |= ( 1 << 1 );                 // Timer 0 external input w/o divide

    _wait( 1000 );

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _timer_read( timer_id, usec )                                           *
 *                                                                          *
 *      Read the timer start & stop values after ### usec                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Uint32 _timer_read( Uint32 timer_id, Uint32 usec )
{
    TIMER_Handle timer_handle = TIMER_table[timer_id];

    Uint32 start;
    Uint32 stop;

    timer_handle->regs->TGCR  = 0;              // Stop TIMER
    timer_handle->regs->TIM12 = 0;              // Reset counter
    timer_handle->regs->TIM34 = 0;              // Reset counter
    timer_handle->regs->TGCR  = 0x00000003;     // Start TIMER

    start = timer_handle->regs->TIM12;          // Record start time

    _waitusec( usec );                          // Wait ###

    stop = timer_handle->regs->TIM12;           // Record stop time

    timer_handle->regs->TGCR = 0;               // Stop TIMER

    return stop - start;
}
