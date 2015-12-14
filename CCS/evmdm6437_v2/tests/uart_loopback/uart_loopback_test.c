/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  UART Test
 *
 */

#include "evmdm6437_uart.h"

static Uint8 rx[256];
static Uint8 tx[256];

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  uart_loopback_test( )                                                   *
 *      Simple UART loopback test.                                          *
 *      Will write then read back the byte sent to the RS232 connector.     *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 uart_loopback_test( )
{
    Int16 i, errors = 0;
    UART_Handle uart0;

    Int32 timeout = 0;
    Int32 test_timeout = 0x100000;

    /* Open Uart Handle */
    uart0 = EVMDM6437_UART_open( 0, 115200 );

    if ( uart0 == ( UART_Handle )-1 )
        return 1;

    /* Setup buffers */
    for ( i = 0 ; i < 0x100 ; i++ )
    {
        tx[i] = i;
        rx[i] = 0;
    }

    /* UART Test */
    for ( i = 0 ; i < 0x100 ; i++ )
    {
        /* TX */
        timeout = test_timeout;
        while( EVMDM6437_UART_xmtReady( uart0 ) )   // Wait for TX ready
        {
            if ( timeout-- < 0 )
                return -1;
        }

        EVMDM6437_UART_putChar( uart0, tx[i] );     // Write 1 byte

        /* RX */
        timeout = test_timeout;
        while( EVMDM6437_UART_rcvReady( uart0 ) )   // Wait for Rx ready
        {
            if ( timeout-- < 0 )
                return -1;
        }

        EVMDM6437_UART_getChar( uart0, &rx[i] );    // Read 1 byte
    }

    /* Compare TX & RX */
    for ( i = 0 ; i < 0x100 ; i++ )
        if ( tx[i] != rx[i] )
            errors++;

    return errors;
}
