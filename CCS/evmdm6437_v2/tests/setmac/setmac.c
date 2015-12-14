/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Set MAC address & Version info
 *
 */

#include "stdio.h"
#include "evmdm6437_eeprom.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *      Set MAC Address & Version to EEPROM                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main( )
{
    Int16 i;
    Uint8 MAC[8]     = { 0, 0, 0, 0, 0, 0 };
    Uint8 VERSION[1] = { 0 };
    Uint8 verify[8]  = { 0, 0, 0, 0, 0, 0 };
    Uint32 input[6]  = { 0x00, 0x0E, 0x99, 0, 0, 0 };

    /* BSL EEPROM init */
    EVMDM6437_init( );
    EVMDM6437_EEPROM_init( );

    /* Input MAC address */
    fflush( stdin );
    printf( "\nEnter MAC address:   ( 00-0E-99-xx-xx-xx ) ( Last 3 numbers only )\n");
    scanf( "%x-%x-%x", &input[3], &input[4], &input[5] );
    printf( "\n" );

    /* Check Input */
    for ( i = 0 ; i < 6 ; i++ )
    {
        if ( input[i] > 255 )
        {
            printf( " ***Failed. Bad input: %x-%x-%x*** \n", input[3], input[4], input[5] );
            return;
        }
    }

    /* Store input into MAC[] */
    for ( i = 0 ; i < 6 ; i++ )
        MAC[i] = ( Uint8 )input[i];


    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  Set MAC address to the [first six bytes] of the [last page]     *
     *                                                                  *
     * ---------------------------------------------------------------- */
    printf( "Writing MAC address: ( 00-0E-99-%02X-%02X-%02X )\n", MAC[3], MAC[4], MAC[5] );

    /* Write to EEPROM */
    EVMDM6437_EEPROM_write( ( Uint32 )MAC, 0x7F00, 8 );

    /* Wait for EEPROM to write */
    _waitusec( 250000 );

    /* Read Back */
    EVMDM6437_EEPROM_read( 0x7F00, ( Uint32 )verify, 8 );

    /* Verify the MAC address in EEPROM */
    for ( i = 0 ; i < 6 ; i++ )
        if ( verify[i] != MAC[i] )
        {
            printf( "\n ***Failed Setting MAC address*** \n" );
            return;
        }

    printf( "New MAC address set: ( %02X-%02X-%02X-%02X-%02X-%02X ) - Good!\n",
        MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5] );


    /* ---------------------------------------------------------------- *
     *                                                                  *
     *  Set VERSION ID# to the [last bytes] of the [last page]          *
     *                                                                  *
     * ---------------------------------------------------------------- */
    VERSION[0] = 1;

    /* Write to EEPROM */
    EVMDM6437_EEPROM_write( ( Uint32 )VERSION, 0x7FFF, 1 );

    /* Wait for EEPROM to write */
    _waitusec( 250000 );

    /* Read Back */
    EVMDM6437_EEPROM_read( 0x7FFF, ( Uint32 )verify, 1 );

    /* Verify the VERSION ID# in EEPROM */
    if ( verify[0] != VERSION[0] )
    {
        printf( "\n ***Failed Setting Version ID#*** \n" );
        return;
    }

    printf( "Setting Version ID#: ( %02X ) - Good!\n", VERSION[0] );
    printf( "\nPASS.\n" );

    SW_BREAKPOINT;
    return;
}
