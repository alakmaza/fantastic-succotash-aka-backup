/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  NOR Flash Test
 *
 */

#include "stdio.h"
#include "evmdm6437_flash.h"

static Uint32 buf_len = 1024;
static Uint8 rx[1024];
static Uint8 tx[1024];

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  norflash_test( )                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 norflash_test( )
{
    Int32 i = 0, j = 0;
    Int16 errors = 0;

    Uint32 npages;
    Uint32 nbytes;

    Uint32 *p32;
    Uint32 addr;

    /* Initialize Flash */
    EVMDM6437_FLASH_init();

    /* Determine the total # number of pages of Flash */
    npages = EVMDM6437_FLASH_getTotalPages( 1 );

    if ( npages == 0 )
    {
        printf( "     Error: NO pages found\n" );
        printf( "     Verify that JP2 is set to FLASH.\n" );
        return 1;
    }
    else
    {
        nbytes = npages * FLASH_PAGESIZE;
        printf( "     %d Pages\n", npages );
        printf( "     %d Mbytes\n", nbytes >> 20 );
    }

    /* ---------------------------------------------------------------- *
     *  Erase                                                           *
     * ---------------------------------------------------------------- */
    printf( "     Erasing Flash [%d-%d] pages\n", 0, npages );

    /* Erase all bytes of Flash */
    for ( i = 0 ; i < npages ; i += 8 )
    {
        printf( "     Erasing ( Pg: %d-%d of %d )\n", i, i + 7, npages );
        errors = EVMDM6437_FLASH_erase( FLASH_BASE + ( i * FLASH_PAGESIZE ), 8 * FLASH_PAGESIZE );

        /* Check errors */
        if ( errors )
        {
            printf( "     Error: Erasing code %d\n", errors );
            return ( errors | 0x8000 );
        }
    }

    /* ---------------------------------------------------------------- *
     *  Write                                                           *
     * ---------------------------------------------------------------- */
    printf( "     Writing Flash [%d-%d] bytes\n", 0, nbytes );

    i = 0;

    /* Iterate through all bytes */
    for ( addr = FLASH_BASE ; addr < FLASH_BASE + nbytes ; addr += buf_len )
    {
        /* Create write pattern */
        p32 = ( Uint32* )tx;
        for ( j = 0 ; j < buf_len ; j += 4 )
            *p32++ = ( addr + j + i );

        /* Write the pattern to Flash */
        errors = EVMDM6437_FLASH_write( ( Uint32 )tx, addr, buf_len );

        /* Check errors */
        if ( errors )
        {
            printf( "     Error: Writing code %d\n", errors );
            return ( errors | 0x8000 );
        }

        /* Write first page, then the beginning of each other page */
        if ( ( addr - FLASH_BASE ) >= FLASH_PAGESIZE )
        {
            addr += ( FLASH_PAGESIZE - buf_len );
            i = ( i + 1 ) & 3;
        }
    }

    /* ---------------------------------------------------------------- *
     *  Read                                                            *
     * ---------------------------------------------------------------- */
    printf( "     Reading Flash [%d-%d] bytes\n", 0, nbytes );

    i = 0;

    /* Iterate through all bytes */
    for ( addr = FLASH_BASE ; addr < FLASH_BASE + nbytes ; addr += buf_len )
    {
        /* Read from Flash */
        errors = EVMDM6437_FLASH_read( addr, ( Uint32 )rx, buf_len );

        /* Check pattern */
        p32 = ( Uint32* )rx;
        for ( j = 0 ; j < buf_len ; j += 4 )
            if ( *p32++ != ( addr + j + i ) )
                return ( j | 0x8000 );

        /* Check errors */
        if ( errors )
        {
            printf( "     Error: Reading code %d\n", errors );
            return ( errors | 0x8000 );
        }

        /* Read first page, then the beginning of each other page */
        if ( ( addr - FLASH_BASE ) >= FLASH_PAGESIZE )
        {
            addr += ( FLASH_PAGESIZE - buf_len );
            i = ( i + 1 ) & 3;
        }
    }

    printf( "     Erasing 1st Flash page\n" );

    /* Erase first page of Flash before exiting */
    EVMDM6437_FLASH_erase( FLASH_BASE, FLASH_PAGESIZE );

    /* Test Passed */
    return 0;
}
