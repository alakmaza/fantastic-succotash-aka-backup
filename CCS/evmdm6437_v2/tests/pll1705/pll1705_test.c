/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  PLL1705
 *
 */

#include "stdio.h"
#include "evmdm6437.h"
#include "evmdm6437_i2c_gpio.h"

/*
 *  I2C_GPIO_GROUP_2
 *
 *  PLL_CSEL = p7
 *  PLL_FS1 = p6
 *  PLL_FS2 = p5
 *  PLL_SR = p4
 */

#define PLL1705_CSEL_HIGH   EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 7, 1 );
#define PLL1705_CSEL_LOW    EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 7, 0 );

#define PLL1705_FS1_HIGH    EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 6, 1 );
#define PLL1705_FS1_LOW     EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 6, 0 );

#define PLL1705_FS2_HIGH    EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 5, 1 );
#define PLL1705_FS2_LOW     EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 5, 0 );

#define PLL1705_SR_HIGH     EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 4, 1 );
#define PLL1705_SR_LOW      EVMDM6437_I2C_GPIO_setOutput( I2C_GPIO_GROUP_2, 4, 0 );

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  pll1705_test( )                                                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 pll1705_test( )
{
    Uint16 switch_state  = 0xFF;
    Uint16 pll1705_state = 0xFF;

    Int16 csel;
    Int16 fs1;
    Int16 fs2;
    Int16 sr;

    Uint32 scko1, scko2, scko3;
    Uint32 sample_freq;
    Uint32 double_rate;

    /* Initialize the I2C_GPIO & LED modules */
    EVMDM6437_I2C_GPIO_init( );
    EVMDM6437_LED_init( );

    printf( "* DIP switch SW4.0 -> PLL.CSEL\n" );
    printf( "* DIP switch SW4.1 -> PLL.FS1\n" );
    printf( "* DIP switch SW4.2 -> PLL.FS2\n" );
    printf( "* DIP switch SW4.3 -> PLL.SR\n" );

    while ( 1 )
    {
        /*
         *  USER switches on P7-P4
         *  Bit mask 0xF0
         */
        if ( EVMDM6437_I2C_GPIO_getall( I2C_GPIO_GROUP_0, &switch_state ) )
			return 1;

        /*
         *  PLL settings on P7-P4
         *  Bit mask 0xF0
         */
        if ( EVMDM6437_I2C_GPIO_getall( I2C_GPIO_GROUP_2, &pll1705_state ) )
		    return 2;

        pll1705_state = ( pll1705_state & 0x0F ) | ( switch_state & 0xF0 );

        if ( EVMDM6437_I2C_GPIO_setall( I2C_GPIO_GROUP_2, pll1705_state ) )
		    return 3;

        csel = ( pll1705_state >> 4 ) & 1;
        fs1  = ( pll1705_state >> 5 ) & 1;
        fs2  = ( pll1705_state >> 6 ) & 1;
        sr   = ( pll1705_state >> 7 ) & 1;

        if ( csel == 0 )
            scko1 = 33868;
        else
            scko1 = 16934;

        if ( ( fs2 == 0 ) && ( fs1 == 0 ) )
            sample_freq = 48000;
        else if ( ( fs2 == 0 ) && ( fs1 == 1 ) )
            sample_freq = 44100;
        else if ( ( fs2 == 1 ) && ( fs1 == 0 ) )
            sample_freq = 32000;
        else
            sample_freq = 0;

        if ( sr == 0 )
            double_rate = 1;
        else
            double_rate = 2;

        if ( ( fs2 == 1 ) && ( fs1 == 1 ) )
            printf( "> [CSEL: %d] [FS1: n/a] [FS2: n/a] [SR: %d]\n", csel, sr );
        else
            printf( "> [CSEL: %d] [FS1: %d] [FS2: %d] [SR: %d]\n", csel, fs1, fs2, sr );

        sample_freq = sample_freq * double_rate;

        switch ( sample_freq )
        {
            case 32000:
                scko2 = 8192;
                scko3 = 12288;
                break;
            case 44100:
                scko2 = 11289;
                scko3 = 16934;
                break;
            case 48000:
                scko2 = 12288;
                scko3 = 18432;
                break;
            case 64000:
                scko2 = 16384;
                scko3 = 24576;
                break;
            case 88200:
                scko2 = 22579;
                scko3 = 33868;
                break;
            case 96000:
                scko2 = 24576;
                scko3 = 36864;
                break;
            default:
                scko2 = 0;
                scko3 = 0;
        }

        printf( "> [SCKO1: %d.%d] [SCKO2*: %d.%d] [SCKO3: %d.%d]\n",
            scko1 / 1000, scko1 % 1000, scko2 / 1000, scko2 % 1000,
            scko3 / 1000, scko3 % 1000 );

        printf( "\n" );

        EVMDM6437_I2C_GPIO_waitForIntr( 0x70000000 );

        _waitusec( 50000 );
    }

    return 0;
}
