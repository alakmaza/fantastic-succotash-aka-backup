/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  DIP Switches / LED Test
 *
 */

#include "stdio.h"
#include "evmdm6437_dip.h"
#include "evmdm6437_led.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  dip_switch_test( )                                                      *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 dip_switch_test( )
{
    Int16 dip0, dip1, dip2, dip3;
    Int16 jp1, sw7;

    Int16 last_jp1 = -1;
    Int16 last_sw7 = -1;

    /* Initialize the DIP Switches & LEDs */
    EVMDM6437_DIP_init( );
    EVMDM6437_LED_init( );

    while ( 1 )
    {
        /* Will return DIP_DOWN or DIP_UP */
        dip0 = EVMDM6437_DIP_get( DIP_0 );
        dip1 = EVMDM6437_DIP_get( DIP_1 );
        dip2 = EVMDM6437_DIP_get( DIP_2 );
        dip3 = EVMDM6437_DIP_get( DIP_3 );

        /* Will return JP1_JUMPER_NTSC or JP1_JUMPER_PAL */
        jp1 = EVMDM6437_DIP_get( JP1_JUMPER );

        /* Will return SW7_SWITCH_LEFT or SW7_SWITCH_RIGHT */
        sw7 = EVMDM6437_DIP_get( SW7_SWITCH );

        /* Synchronize the DIP switches & LEDs */
        if ( dip0 == DIP_DOWN )
            EVMDM6437_LED_on( 0 );
        else
            EVMDM6437_LED_off( 0 );

        if ( dip1 == DIP_DOWN )
            EVMDM6437_LED_on( 1 );
        else
            EVMDM6437_LED_off( 1 );

        if ( dip2 == DIP_DOWN )
            EVMDM6437_LED_on( 2 );
        else
            EVMDM6437_LED_off( 2 );

        if ( dip3 == DIP_DOWN )
            EVMDM6437_LED_on( 3 );
        else
            EVMDM6437_LED_off( 3 );

        /* Print only if there was a change to JP1 */
        if ( jp1 != last_jp1 )
        {
            if ( jp1 == JP1_JUMPER_NTSC )
            {
                printf( "  > Jumper JP1 set to NTSC (HIGH)\n" );
                last_jp1 = JP1_JUMPER_NTSC;
            }
            if ( jp1 == JP1_JUMPER_PAL )
            {
                printf( "  > Jumper JP1 set to PAL  (LOW)\n" );
                last_jp1 = JP1_JUMPER_PAL;
            }
        }

        /* Print only if there was a change to SW7 */
        if ( sw7 != last_sw7 )
        {
            if ( sw7 == SW7_SWITCH_RIGHT )
            {
                printf( "  > Switch SW7 set to RIGHT (HIGH)\n" );
                last_sw7 = SW7_SWITCH_RIGHT;
            }
            if ( sw7 == SW7_SWITCH_LEFT )
            {
                printf( "  > Switch SW7 set to LEFT  (LOW)\n" );
                last_sw7 = SW7_SWITCH_LEFT;
            }
        }

        /* Wait for the I2C GPIO Expander to change */
        EVMDM6437_DIP_wait( 1000000 );
    }

    return 0;
}
