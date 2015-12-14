/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  NAND Flash Test
 *
 */

#include "stdio.h"
#include "evmdm6437_nandflash.h"

static Uint8 rx[NAND_PAGESIZE + NAND_SPARESIZE];
static Uint8 tx[NAND_PAGESIZE + NAND_SPARESIZE];

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  nandflash_test( )                                                       *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 nandflash_test( )
{
    Int32 i,j = 0;
    Int16 errors = 0;

    Uint32 nblocks;
    Uint32 npages;
    Uint32 nbytes;
    Uint32 segment_len = NAND_PAGESIZE;

    Uint32* p32;

    Uint32 page;
    Uint32 pageaddr;
    Int32 bad_pages = 0;
    Int32 allowed_bad_pages;

    /* ---------------------------------------------------------------- *
     *  Initialize NAND Flash                                           *
     * ---------------------------------------------------------------- */
    EVMDM6437_NANDFLASH_init( );

    /* Determine the total number of pages */
    npages = EVMDM6437_NANDFLASH_getTotalPages( );

    if ( npages == 0 )
    {
        printf( "     >> Error: Cannot find NAND Flash pages.\n" );
        printf( "               Verify Jumper JP2 is set to NAND.\n" );
        return 1;
    }
    else
    {
        nblocks = npages >> NAND_PAGES_PER_BLOCK_POW2;
        nbytes  = npages * NAND_PAGESIZE;
        printf( "     %d Blocks\n", nblocks );
        printf( "     %d Pages\n", npages );
        printf( "     %d MBytes\n", nbytes >> 20 );

        allowed_bad_pages = 26;
        nblocks = nblocks >> 2;     // Reduce total amount to test
        npages = npages >> 2;
        printf( "     Reducing test size to [%d blocks],[%d pages],[%d bad pages]\n",
                nblocks, npages, allowed_bad_pages );
    }


    /* ---------------------------------------------------------------- *
     *  Erase                                                           *
     * ---------------------------------------------------------------- */
    printf( "     Erasing Flash [%d-%d] blocks\n", 0, nblocks );

    /* Cycle through NAND Flash Blocks */
    for ( i = 0 ; i < nblocks ; i++ )
    {
        errors = EVMDM6437_NANDFLASH_erase( i * NAND_BLOCKSIZE, 1 );

        if ( errors == NAND_ERR_TIMEOUT )
        {
            printf( "     >> Error: Timeout occured.\n" );
            return errors;
        }
        else if ( errors != 0 )
        {
            printf( "     Found bad block: %d\n", i );
            if ( ( ++bad_pages ) > allowed_bad_pages )
            {
                printf( "     >> Error: Erasing NAND Flash %d\n", bad_pages );
                return errors;
            }
        }
    }

    bad_pages = 0;

    /* ---------------------------------------------------------------- *
     *  Write                                                           *
     * ---------------------------------------------------------------- */
    printf( "     Writing Flash [%d-%d] pages\n", 0, npages );

    /* Iterate through all pages */
    for ( page = 0 ; page < npages ; page++ )
    {
        pageaddr = ( page * NAND_PAGESIZE );

        /* Create write pattern */
        p32 = ( Uint32* )tx;
        for ( i = 0 ; i < segment_len ; i += 4 )
            *p32++ = ( pageaddr + i );

        /* Write to Flash */
        errors = EVMDM6437_NANDFLASH_writePage( ( Uint32 )tx, pageaddr, 1 );
        if ( errors )
        {
            printf( "     Found bad page: %d\n", page );
            if ( ( ++bad_pages ) > allowed_bad_pages )
            {
                printf( "     >> Error: Too many bad pages %d\n", bad_pages );
                return bad_pages;
            }
        }

        /* Skip some pages */
        if ( page >= NAND_PAGES_PER_BLOCK )
            page += ( NAND_PAGES_PER_BLOCK - 1 );
    }

    bad_pages = 0;

    /* ---------------------------------------------------------------- *
     *  Read                                                            *
     * ---------------------------------------------------------------- */
    printf( "     Reading Flash [%d-%d] pages\n", 0, npages );

    /* Iterate through all pages */
    for ( page = 0 ; page < npages ; page++ )
    {
        pageaddr = ( page * NAND_PAGESIZE );

        /* Read from Flash */
        errors = EVMDM6437_NANDFLASH_readPage( pageaddr, ( Uint32 )rx, 1 );

        /* Check pattern */
        p32 = ( Uint32* )rx;
        for ( i = 0; i < segment_len; i += 4 )
            if ( *p32++ != ( pageaddr + i ) )
                errors = 1;

        if ( errors )
        {
            printf( "     Found bad page: %d\n", page );
            if ( ( ++bad_pages ) > allowed_bad_pages )
            {
                printf( "     >> Error: Too many bad pages %d\n", bad_pages );
                return bad_pages;
            }
        }

        /* Skip some pages */
        if ( page >= NAND_PAGES_PER_BLOCK )
            page += ( NAND_PAGES_PER_BLOCK - 1 );
    }

    bad_pages = 0;

    /* ---------------------------------------------------------------- *
     *  Erase                                                           *
     * ---------------------------------------------------------------- */
    printf( "     Erasing Flash [%d-%d] blocks\n", 0, nblocks );

    /* Cycle through NAND Flash Blocks */
    for ( i = 0 ; i < nblocks ; i++ )
    {
        errors = EVMDM6437_NANDFLASH_erase( i * NAND_BLOCKSIZE, 1 );

        if ( errors == NAND_ERR_TIMEOUT )
        {
            printf( "     >> Error: Timeout occured.\n" );
            return errors;
        }
        else if ( errors != 0 )
        {
            printf( "     Found bad block: %d\n", i );
            if ( ( ++bad_pages ) > allowed_bad_pages )
            {
                printf( "     >> Error: Erasing NAND Flash %d\n", bad_pages );
                return errors;
            }
        }
    }

    return 0;
}
