/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  PWM implementation
 */

#include "pwm.h"

PWM_Handle pwm_table[3] = {
    ( PWM_Handle )&PWM_MODULE_0,
    ( PWM_Handle )&PWM_MODULE_1,
    ( PWM_Handle )&PWM_MODULE_2
};

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _setup_pwm( pwm_id, active, inactive )                                  *
 *                                                                          *
 *      Setup the PWM #pwm_id in Continuous mode w/ an active & an inactive *
 *      Period.                                                             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 _setup_pwm( Uint32 pwm_id, Uint32 active, Uint32 inactive )
{
    Uint32 total_period = active + inactive;

    PWM_Handle pwm_handle = pwm_table[pwm_id];

    if ( pwm_id == 0 )
    {
        EVMDM6437_clrPinMux( 0, ( 3 << 10 ) );
        EVMDM6437_setPinMux( 0, ( 2 << 10 ) );
    }
    if ( pwm_id == 1 )
    {
        EVMDM6437_clrPinMux( 0, ( 1 << 12 ) );
        EVMDM6437_setPinMux( 0, ( 1 << 12 ) );
    }
    if ( pwm_id == 2 )
    {
        EVMDM6437_clrPinMux( 0, ( 3 << 14 ) );
        EVMDM6437_setPinMux( 0, ( 2 << 14 ) );
    }

    pwm_handle->regs->START = 0;
    pwm_handle->regs->PCR   = 0x0001;       // Free emulation bit
    pwm_handle->regs->CFG   = 0
        | ( 0 << 6 )                        // Disable Interrupt
        | ( 1 << 5 )                        // Inactive output level    [HIGH]
        | ( 0 << 4 )                        // 1st phase output level   [LOW]
        | ( 0 << 2 )                        // Event Trigger            [Disable]
        | ( 2 << 0 );                       // Mode                     [Continuous]

    pwm_handle->regs->RPT   = 0x0000;       // One-shot repeat count
    pwm_handle->regs->PER   = total_period; // Output Period
    pwm_handle->regs->PH1D  = active;       // First-phase duration
    pwm_handle->regs->START = 1;            // Start

    _waitusec( 10 );                        // Wait for PWM to start
    return 0;
}
