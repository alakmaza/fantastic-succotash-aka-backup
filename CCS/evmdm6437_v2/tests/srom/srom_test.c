/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  SPI ROM test
 *
 */

#include "stdio.h"
#include "srom.h"

static Uint8 tx[64];
static Uint8 rx[64];

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  srom_test( )                                                            *
 *      SPI ROM test, write then verify the contents of the first 4 pages   *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 srom_test( )
{
    Int16 i;

    Uint16 page;
    Uint16 page_size = 64;

    Uint8* p8;

    srom_init( );

    /* Write to SROM */
    for ( page = 0 ; page < 4 ; page++ )
    {
        /* Create the test pattern */    
        p8 = ( Uint8* )tx;
        for ( i = 0 ; i < page_size ; i++ )
            *p8++ = ( Uint8 )( i + page );

        /* Write first 64 bytes */
        srom_write( ( Uint32 )tx, page * page_size, page_size );
    }

    for ( i = 0 ; i < page_size ; i++ )
        rx[i] = 0;

    /* Read and Verify SROM */
    for ( page = 0 ; page < 4 ; page++ )
    {
        /* Read first 64 bytes */
        srom_read( page * page_size, ( Uint32 )rx, page_size );

        /* Create the pattern */
        p8 = ( Uint8* )rx;
        for ( i = 0 ; i < page_size ; i++ )
            if ( ( *p8++ ) != ( Uint8 )( page + i ) )
                return 1;  // Fail
    }

    return 0;
}
