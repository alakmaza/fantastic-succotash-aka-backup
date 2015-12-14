/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  DDR2 Test
 *
 */

#include "stdio.h"
#include "evmdm6437.h"

extern memfill32( Uint32 start, Uint32 len, Uint32 val );
extern memaddr32( Uint32 start, Uint32 len );
extern meminvaddr32( Uint32 start, Uint32 len );

Uint32 ddr_base;
Uint32 ddr_size;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  ddr_test( )                                                             *
 *      Test DDR memory, checks the data and address lines are good.        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 ddr_test( )
{
    ddr_base = 0x80000000;       // DDR memory
    ddr_size = 0x00100000;       // 1 MB

    if ( memfill32( ddr_base, ddr_size, 0xFFFFFFFF ) )
        return 1;

    if ( memfill32( ddr_base, ddr_size, 0xAAAAAAAA ) )
        return 2;

    if ( memfill32( ddr_base, ddr_size, 0x55555555 ) )
        return 4;

    if ( memfill32( ddr_base, ddr_size, 0x00000000 ) )
        return 8;

    ddr_base = 0x80000000;       // DDR memory
    ddr_size = 0x08000000;       // 128 MB

    if ( memaddr32( ddr_base, ddr_size ) )
        return 10;

    if ( meminvaddr32( ddr_base, ddr_size ) )
        return 20;

    return 0;
}
