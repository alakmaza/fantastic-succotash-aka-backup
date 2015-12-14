/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  I2C EEPROM Test
 *
 */

#include "stdio.h"
#include "evmdm6437_eeprom.h"

static Uint8 tx[4][EEPROM_PAGE_SIZE];
static Uint8 rx[4][EEPROM_PAGE_SIZE];

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  eeprom_test( )                                                          *
 *      I2C EEPROM test, write then verify the contents of the first 4      *
 *      pages.                                                              *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 eeprom_test( )
{
    Int16 i, j;
    Int16 errors = 0;

    Uint16 pages = 4;
    Uint16 page_size = EEPROM_PAGE_SIZE;

    Uint8 *p8;
    Uint32 src, dst; 

    /* Create the test pattern */
    p8 = ( Uint8* )tx;
    for ( i = 0 ; i < pages * page_size ; i++ )
        *p8++ = i;

    /* Clear RX buffer */
    p8 = ( Uint8* )rx;
    for ( i = 0 ; i < pages * page_size ; i++)
        *p8++ = 0;

    /* Write 1 page at a time */
    for ( i = 0 ; i < pages ; i++ )
    {
        src = ( Uint32 )&tx[i];
        dst = i * page_size;

        if ( EVMDM6437_EEPROM_write( src, dst, page_size ) )
            return 1;

        _wait( 250000 );
    }

    /* Read 1 page at a time */
    for ( i = 0 ; i < pages ; i++ )
    {
        src = i * page_size;
        dst = ( Uint32 )&rx[i];

        if ( EVMDM6437_EEPROM_read( src, dst, page_size ) )
            return 2;

        _wait( 250000 );
    }

    /* Verify tx & rx */
    for ( i = 0 ; i < pages ; i++ )
    {
        errors = 0;

        for ( j = 0 ; j < page_size ; j++ )
        {
            if ( tx[i][j] != rx[i][j] )
                errors++;
        }

        if ( errors == 0 )
            printf( "    Page %d: PASS\n", i );
        else
            printf( "    Page %d: FAIL\n", i );
    }

    return errors;
}
