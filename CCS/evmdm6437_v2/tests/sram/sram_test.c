/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  SRAM Test
 *
 */

#include "evmdm6437.h"

extern memfill32( Uint32 start, Uint32 len, Uint32 val );
extern memaddr32( Uint32 start, Uint32 len );
extern meminvaddr32( Uint32 start, Uint32 len );

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  sram_init( )                                                            *
 *                                                                          *
 *      Setup SRAM                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 sram_init( )
{
    /*------------------------------------------------------------------*
     *                                                                  *
     *  SRAM timing parameters                                          *
     *                                                                  *
     *      EMIF.CLK freq   = PLL1/6 = 594/6 = 99 MHz                   *
     *      EMIF.CLK period = 1/99 MHz = 10.1 ns                        *
     *                                                                  *
     *------------------------------------------------------------------*/
    Uint32 acfg2 = 0
        | ( 0 << 31 )           // selectStrobe
        | ( 0 << 30 )           // extWait
        | ( 0 << 26 )           // writeSetup      //   0 ns
        | ( 2 << 20 )           // writeStrobe     //  20 ns
        | ( 0 << 17 )           // writeHold       //   0 ns
        | ( 0 << 13 )           // readSetup       //   0 ns
        | ( 2 << 7 )            // readStrobe      //  20 ns
        | ( 0 << 4 )            // readHold        //   0 ns
        | ( 1 << 2 )            // turnAround      //  10 ns
        | ( 0 << 0 );           // asyncSize       //  8-bit bus

    EVMDM6437_setupEMIF( EMIF_CS2, acfg2, EMIF_NORMAL_MODE );

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  sram_test( )                                                            *
 *                                                                          *
 *      Test SRAM using DATA & ADDR memory tests                            *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 sram_test( void )
{
    Uint32 errors = 0;
    Uint32 base = EMIF_CS2_BASE;
    Uint32 size = 0x00100000;  // 1 MBytes

    /* Initialize SRAM */
    sram_init( );

    /* Data tests */
    errors |= memfill32( base, size, 0xFFFFFFFF );
    errors |= memfill32( base, size, 0xAAAAAAAA );
    errors |= memfill32( base, size, 0x55555555 );
    errors |= memfill32( base, size, 0x00000000 );

    /* Addr tests */
    errors |= memaddr32( base, size );
    errors |= meminvaddr32( base, size );

    return ( errors == 0 ) ? 0 : -1;
}
