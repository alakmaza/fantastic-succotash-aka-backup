/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  VLYNQ API
 *
 */

#include "stdio.h"
#include "vlynq.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_init( )                                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_init( Int16 pins_used, Int16 scrun_used )
{
    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  EVM VLYNQ - Board Setup                                         *
     *                                                                  *
     * ---------------------------------------------------------------- */
    Uint32 pinmux1 = 0;

    /* Must use all 4 pins */
    switch ( pins_used )
    {
        case VLYNQ_USE_4_PINS:
        case VLYNQ_USE_3_PINS:
        case VLYNQ_USE_2_PINS:
        case VLYNQ_USE_1_PINS:
            /* VLYNQWD [13:12] */
            pinmux1 |= ( 2 << 4 );
            break;
        default:
            return -1;
    }

    /* Tough luck SCRUN is on */
    switch ( scrun_used )
    {
        case VLYNQ_USE_SCRUN:
        case VLYNQ_DONT_USE_SCRUN:
            /* VLSCREN [14] */
            pinmux1 |= ( 2 << 4 );
            break;

        default:
            return -2;
    }

    /* Set PinMux */
    EVMDM6437_clrPinMux( 0, ( 3 << 4 ) );
    EVMDM6437_setPinMux( 0, pinmux1 );

    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  EVM VLYNQ - Module Setup                                        *
     *                                                                  *
     * ---------------------------------------------------------------- */
    VLYNQ_CTRL |= 1;            // Software Reset

    VLYNQ_CTRL = 0
        | ( 0 << 31 )           // Power ON
        | ( 0 << 30 )           // Serial clock pull-up enable
        | ( 7 << 24 )           // [26:24] Rx sample value
        | ( 0 << 23 )           // RTM valid
        | ( 0 << 22 )           // RTM enable
        | ( 1 << 21 )           // Transmit fast path enabled
        | ( 0 << 16 )           // [18:16] Clock divider
        | ( 1 << 15 )           // Internal clock
        | ( 1 << 14 )           // Interrupt local
        | ( 0 << 13 )           // Interrupt ignored
        | ( 0 << 8 )            // [12:8] Interrupt vector
        | ( 1 << 7 )            // Interrupt to config register
        | ( 1 << 2 )            // Address optimization enabled
        | ( 0 << 1 )            // Normal operation [no Internal loopback]
        | ( 0 << 0 );           // Normal operation [no Software Reset]

    VLYNQ_INTPTR = ( Uint32 )&VLYNQ_INTPTR;
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
    /* Reset VLYNQ */
    vlynq_reset( );

    /* Disable VLYNQ PinMux */
    EVMDM6437_clrPinMux( 0, 0x00000070 );
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
