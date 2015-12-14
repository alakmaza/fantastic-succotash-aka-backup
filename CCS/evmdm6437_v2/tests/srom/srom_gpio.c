/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  SPI ROM interface
 *
 */

#include "srom.h"
#include "evmdm6437_gpio.h"

Uint8 srombuf[ SROM_PAGESIZE + 4 ];
Uint8 statusbuf[2];


#define CLKX0_LOW  EVMDM6437_GPIO_setOutput( GPIO99, 0 );
#define CLKX0_HIGH EVMDM6437_GPIO_setOutput( GPIO99, 1 );

#define FSX0_LOW   EVMDM6437_GPIO_setOutput( GPIO103, 0 );
#define FSX0_HIGH  EVMDM6437_GPIO_setOutput( GPIO103, 1 );

#define DX0_LOW    EVMDM6437_GPIO_setOutput( GPIO104, 0 );
#define DX0_HIGH   EVMDM6437_GPIO_setOutput( GPIO104, 1 );

#define DR0        EVMDM6437_GPIO_getInput( GPIO100 );

void _srom_cycle( Uint8* buf, Int16 len )
{
    Int16 i;
    Int16 bit;
    Uint8 rx_byte;
    Uint8 tx_byte;

    /* SROM Enable */
    FSX0_LOW;

    for ( i = 0 ; i < len ; i++ )
    {
        tx_byte = buf[i];
        rx_byte = 0;
        for ( bit = 7 ; bit >= 0 ; bit-- )
        {
            CLKX0_LOW;

            if ( ( ( tx_byte >> bit ) & 1 ) == 0 )
            {
                DX0_LOW;
            }
            else
            {
                DX0_HIGH;
            }

            _wait( 3 );

            CLKX0_HIGH;

          //_wait( 0 );

            rx_byte = ( rx_byte << 1 ) | DR0;
        }

        if ( i > 0 )
            buf[i] = rx_byte;
    }

    CLKX0_LOW;
    _wait( 4 );

    /* SROM Disable */
    FSX0_HIGH;

    _waitusec( 1 );
}

Uint8 srom_status( )
{
    /* Issue read status command */
    statusbuf[0] = SROM_CMD_RDSR;
    statusbuf[1] = 0;
    _srom_cycle( statusbuf, 2 );
    return statusbuf[1];
}

/*
 *  ======== srom_init ========
 *  init srom
 */
void srom_init( )
{
    /*
     *  Pin Mux for MCBSP0
     */
    EVMDM6437_clrPinMux( 0, ( 3 << 22 ) );
    EVMDM6437_setPinMux( 0, ( 0 << 22 ) );

    /*
     *  Configure MCBSP0
     *      CLKX0   -> GPIO99  * -> SCK
     *      FSX0    -> GPIO103 * -> CSn
     *      DX0     -> GPIO104 * -> SI
     *      DR0     -> GPIO100 * -> SO
     *      FSR0    -> GPIO102   -> N/A
     *      CLKR0   -> GPIO101   -> N/A
     */
    EVMDM6437_GPIO_init( );

    EVMDM6437_GPIO_setOutput( GPIO99, 1 );
  //EVMDM6437_GPIO_setOutput( GPIO100, 1 );
    EVMDM6437_GPIO_setOutput( GPIO101, 1 );
    EVMDM6437_GPIO_setOutput( GPIO102, 1 );
    EVMDM6437_GPIO_setOutput( GPIO103, 1 );
    EVMDM6437_GPIO_setOutput( GPIO104, 1 );

    EVMDM6437_GPIO_setDir( GPIO99, GPIO_OUT );  // CLKX
    EVMDM6437_GPIO_setDir( GPIO100, GPIO_IN );  // DR
    EVMDM6437_GPIO_setDir( GPIO101, GPIO_OUT ); // CLKR
    EVMDM6437_GPIO_setDir( GPIO102, GPIO_OUT ); // FSR
    EVMDM6437_GPIO_setDir( GPIO103, GPIO_OUT ); // FSX
    EVMDM6437_GPIO_setDir( GPIO104, GPIO_OUT ); // DX

    _waitusec( 100 );
}

/*
 *  ======== srom_read ========
 *  Read data from srom
 */
void srom_read( Uint32 src, Uint32 dst, Uint32 length )
{
    Int32 i;
    Uint8 *psrc, *pdst;

   /* Create command block for program operation */
    srombuf[0] = SROM_CMD_READ;
    srombuf[1] = ( Uint8 )( src >> 8 );
    srombuf[2] = ( Uint8 )( src >> 0 );

    /* Read */
    _srom_cycle( srombuf, 3 + length );

    /* Establish src & dest */
    pdst = ( Uint8 * )dst;
    psrc = srombuf + 3;
    for ( i = 0 ; i < length ; i++ )
        *pdst++ = *psrc++;
}

/*
 *  ======== srom_write ========
 *  Write data to srom
 */
void srom_write( Uint32 src, Uint32 dst, Uint32 length )
{
    Int32 i;
    Int32 bytes_left;
    Int32 bytes_to_program;
    Uint8 *psrc;

    /* Establish source */
    psrc = ( Uint8 * )src;
    bytes_left = length;

    while ( bytes_left > 0 )
    {
        bytes_to_program = bytes_left;

        /* Most to program is SROM_CMD_BLOCKSIZE */
        if ( bytes_to_program > SROM_PAGESIZE )
             bytes_to_program = SROM_PAGESIZE;

        /* Make sure you don't run off the end of a block */
        if ( ( dst & SROM_PAGEMASK ) != ( ( dst + bytes_to_program ) & SROM_PAGEMASK ) )
            bytes_to_program -= ( dst + bytes_to_program ) - ( ( dst + bytes_to_program ) & SROM_PAGEMASK );

        /* Issue WPEN */
        srombuf[0] = SROM_CMD_WREN;

        _srom_cycle( srombuf, 1 );

        /* Create command block for program operation */
        srombuf[0] = SROM_CMD_WRITE;
        srombuf[1] = ( Uint8 )( dst >> 8 );
        srombuf[2] = ( Uint8 )( dst );

        for ( i = 0 ; i < bytes_to_program ; i++ )
            srombuf[3 + i] = *psrc++;

        /* Execute program command */
        _srom_cycle( srombuf, 3 + bytes_to_program );

        /* Wait while busy */
        while( ( srom_status( ) & 0x01 ) );

        /* Get ready for next iteration */
        bytes_left -= bytes_to_program;
        dst += bytes_to_program;
    }
}
