/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Read MAC address & Version info
 *
 */

#include "stdio.h"
#include "evmdm6437_eeprom.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *      Read MAC Address & Version from EEPROM                              *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main( void )
{
    Uint8 MAC[6] = { 0, 0, 0, 0, 0, 0 };
    Uint8 VERSION[1] = { 0 };

    /* BSL EEPROM init */
    EVMDM6437_init( );
    EVMDM6437_EEPROM_init( );

    fflush( stdin );
    printf( "\n" );

    /* Read MAC address */
    EVMDM6437_EEPROM_read( 0x7F00, ( Uint32 )MAC, 6 );
    printf( "MAC Address: ( %02X-%02X-%02X-%02X-%02X-%02X )\n",
            MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5] );

    /* Wait for EEPROM to write */
    _waitusec( 250000 );

    /* Read Version# */
    EVMDM6437_EEPROM_read( 0x7FFF, ( Uint32 )VERSION, 1 );
    printf( "Version ID#: ( %02X )\n", VERSION[0] );

    /* Wait for EEPROM to write */
    _waitusec( 250000 );

    SW_BREAKPOINT;
    return;
}
