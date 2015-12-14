/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  VLYNQ API
 *
 */

#include "vlynq.h"
#include "stdio.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_init( )                                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_init( Int16 pins_used, Int16 scrun_used )
{
    Uint32 pinmux1 = 0;
    Uint32 pinmux1_mask = 0x00000070;

    switch ( pins_used )                            /* VLYNQWD [13:12] */
    {
        case VLYNQ_USE_4_PINS:                      // Must use all 4 pins
        case VLYNQ_USE_3_PINS:
        case VLYNQ_USE_2_PINS:
        case VLYNQ_USE_1_PINS:
            pinmux1 |= ( 2 << 4 );
            break;
        default:
            return -1;
    }

    switch ( scrun_used )                           /* VLSCREN [14] */
    {
        case VLYNQ_USE_SCRUN:
        case VLYNQ_DONT_USE_SCRUN:
            pinmux1 |= ( 3 << 4 );                  // Tough luck its on
            break;

        default:
            return -2;
    }

    EVMDM6437_clrPinMux( 0, pinmux1_mask );         // Enable internal pinmux
    EVMDM6437_setPinMux( 0, pinmux1 );
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_reset( )                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_reset( )
{
    VLYNQ_CTRL |= 1;
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_quit( )                                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_quit( )
{
    vlynq_reset( );                                 // Reset Vlynq

    EVMDM6437_clrPinMux( 0, 0x00000071 );           // Disable internal pinmux
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_rx_pins_used( )                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_rx_pins_used( )
{
    Int16 rx_pins = ( VLYNQ_STAT >> 24 ) & 0xF;

    printf( "    VLYNQ using %d RX pins.\n", rx_pins );

    switch ( rx_pins )
    {
        case 1:
            return VLYNQ_USE_1_PINS;

        case 2:
            return VLYNQ_USE_2_PINS;

        case 3:
            return VLYNQ_USE_3_PINS;

        case 4:
            return VLYNQ_USE_4_PINS;

        default:
            return VLYNQ_USE_NO_PINS;
    }
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_tx_pins_used( )                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_tx_pins_used( )
{
    Int16 tx_pins = ( VLYNQ_STAT >> 20 ) & 0xF;

    printf( "    VLYNQ using %d TX pins.\n", tx_pins );

    switch ( tx_pins )
    {
        case 1:
            return VLYNQ_USE_1_PINS;

        case 2:
            return VLYNQ_USE_2_PINS;

        case 3:
            return VLYNQ_USE_3_PINS;

        case 4:
            return VLYNQ_USE_4_PINS;

        default:
            return VLYNQ_USE_NO_PINS;
    }
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_wait_for_link( )                                                  *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_wait_for_link( Int32 timeout )
{
    /* Use a timeout, no infinite loops */
    while ( timeout-- > 0 )
    {
        if ( VLYNQ_STAT & 1 )                       // Check for link
        {
            printf( "    VLYNQ link found\n" );
            return VLYNQ_LINK_FOUND;                // VLYNQ link found
        }
    }

    printf( "    Cannot find VLYNQ link.\n" );      // Timeout
    return VLYNQ_NO_LINK_FOUND;                     // No link found
}
