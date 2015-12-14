/*
 *  Copyright 2007 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  VLYNQ Test
 *
 */

#include "stdio.h"
#include "vlynq.h"

static Int32 vlynq_timeout = 20000;

static Uint32 tx[0x400];
static Uint32 rx[0x400];

static Uint32 tx_length = 1024;
static Uint32 rx_length = 1024;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  vlynq_loopback_test( )                                                  *
 *      VLYNQ test that requires the TX & RX pins to be connected.          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 vlynq_loopback_test( )
{
    Int16 i;
    Int16 errors = 0;
    Uint32* ptx;
    
    Int16 pins_used  = VLYNQ_USE_4_PINS;
    Int16 scrun_used = VLYNQ_DONT_USE_SCRUN;

    if ( vlynq_init( pins_used, scrun_used ) )
        return -1;

    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  Setup VLYNQ Memory Map                                          *
     *                                                                  *
     * ---------------------------------------------------------------- */
    VLYNQ_XAM = VLYNQ_REMOTE_LEN - 0x1000;  // Store at end of range
    VLYNQ_RAMS1 = 0x1000;
    VLYNQ_RAMO1 = ( Uint32 )rx;

    /* Init tx pointers */
    ptx = ( Uint32* )( VLYNQ_REMOTE_BASE + VLYNQ_REMOTE_LEN - 0x1000 );

    /* Setup buffers */
    for ( i = 0 ; i < tx_length ; i++ )
    {
        tx[i] = ( i << 16 ) | ( Uint16 )( ~i );
        rx[i] = 0;
    }

    /* Wait for VLYNQ link */
    if ( vlynq_wait_for_link( vlynq_timeout ) == VLYNQ_NO_LINK_FOUND )
    {
        vlynq_quit( );
        return -2;
    }

    /* Make sure we are using all the pins */
    if ( vlynq_tx_pins_used( ) != pins_used )
        errors += 1000;
    if ( vlynq_rx_pins_used( ) != pins_used )
        errors += 1000;

    /* Write Data to VLYNQ memory */
    for ( i = 0 ; i < tx_length ; i++ )
        *ptx++ = tx[i];

    /* Verify data */
    for ( i = 0 ; i < rx_length ; i++ )
        if ( rx[i] != tx[i] )
            errors++;

    if ( errors )
        printf( "Error code %d\n", errors );

    vlynq_quit( );
    return errors;
}
