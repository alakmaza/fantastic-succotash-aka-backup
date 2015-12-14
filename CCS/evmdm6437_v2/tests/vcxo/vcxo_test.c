/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  VCXO Test
 *
 */

#include "stdio.h"
#include "evmdm6437.h"
#include "pwm.h"
#include "timer.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vcxo_test( )                                                            *
 *      Voltage Controlled Oscillator test, use the PWM to affect the       *
 *      volatage level going into the oscillator.  Read back the results.   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vcxo_test( )
{
    /* Timer module */
    Uint32 timer_id = 0;
    Uint32 timer_count = 500000;

    /* PWM module */
    Uint32 pwm_id = 2;
    Uint32 pwm_low;
    Uint32 pwm_high;

    /* Count results */
    Uint32 count_low;
    Uint32 count_mid;
    Uint32 count_high;

    _timer_init( timer_id );

    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  PWM  ._.              ._.               ._.     1/256           *
     *      _| |______________| |_______________| |_  255/256           *
     *                                                                  *
     * ---------------------------------------------------------------- */
    pwm_high = 0x01;
    pwm_low  = 0xf0;
    _setup_pwm( pwm_id, pwm_high, pwm_low );
    count_low = _timer_read( timer_id, timer_count );
    printf( "    [%d] timer count -> [%d] count\n", timer_count, count_low );

    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  PWM  .________.        .________.        ._   128/256           *
     *      _|        |________|        |________|    128/256           *
     *                                                                  *
     * ---------------------------------------------------------------- */
    pwm_high = 0x80;
    pwm_low  = 0x80;
    _setup_pwm( pwm_id, pwm_high, pwm_low );
    count_mid = _timer_read( timer_id, timer_count );
    printf( "    [%d] timer count -> [%d] count\n", timer_count, count_mid );

    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  PWM _. .______________. ._______________. ._  255/256           *
     *       |_|              |_|               |_|     1/256           *
     *                                                                  *
     * ---------------------------------------------------------------- */
    pwm_high = 0xff;
    pwm_low  = 0x01;
    _setup_pwm( pwm_id, pwm_high, pwm_low );
    count_high = _timer_read( timer_id, timer_count );
    printf( "    [%d] timer count -> [%d] count\n", timer_count, count_high );

    printf( "    high: [%d]\n", count_high );
    printf( "     mid: [%d]\n", count_mid );
    printf( "     low: [%d]\n", count_low );

    if ( ( count_low < count_mid ) && ( count_mid < count_high ) )
        return 0;
    else
        return -1;
}
