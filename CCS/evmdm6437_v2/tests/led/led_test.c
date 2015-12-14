/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  LED Test
 *
 */

#include "evmdm6437_led.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  led_test( )                                                             *
 *      Running Four LED test                                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 led_test( )
{
    Int16 i, j;

    /* Initialize the LED module */
    EVMDM6437_LED_init( );

    /* Running Four LED test */
    for ( j = 0 ; j < 5 ; j++ )
    {
        for ( i = 0 ; i < 4 ; i++ )
        {
            if ( EVMDM6437_LED_on( i ) )
                return 1;

            _waitusec( 125000 );
        }

        for ( i = 0 ; i < 4 ; i++ )
        {
            if ( EVMDM6437_LED_off( i ) )
                return 2;

            _waitusec( 125000 );
        }
    }

    /* Turn on all Four LEDs to indicate a PASS */
    for ( i = 0 ; i < 4 ; i++ )
        EVMDM6437_LED_on( i );

    return 0;
}

