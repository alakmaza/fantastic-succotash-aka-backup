/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  Boot Mode Reader
 *
 */

#include "stdio.h"
#include "evmdm6437.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main( void )
{
    Uint16 endian           = ( CFG_BOOTCFG >> 20 ) & 1;
    Uint16 fast_boot        = ( CFG_BOOTCFG >> 19 ) & 1;
    Uint16 pci_enable       = ( CFG_BOOTCFG >> 17 ) & 1;
    Uint16 emifa_bus_width  = ( CFG_BOOTCFG >> 16 ) & 1;
    Uint16 aeaw             = ( CFG_BOOTCFG >> 12 ) & 7;
    Uint16 aem              = ( CFG_BOOTCFG >> 8  ) & 7;
    Uint16 boot_mode        = ( CFG_BOOTCFG >> 0  ) & 15;

    /*
     *  Read & Print the Boot Mode options
     */
    printf( "\nBoot Mode Reader:\n" );

    switch( boot_mode )
    {
        case 0:
            printf( "  > [Boot Mode]: No Boot\n" );
            break;
        case 1:
            if ( pci_enable == 0 )
                printf( "  > [Boot Mode]: HPI Boot\n" );
            if ( pci_enable == 1 )
                printf( "  > [Boot Mode]: PCI Boot w/o auto init\n" );
            break;
        case 2:
            if ( pci_enable == 0 )
                printf( "  > [Boot Mode]: HPI Boot\n" );
            if ( pci_enable == 1 )
                printf( "  > [Boot Mode]: PCI Boot w/ auto init\n" );
            break;
        case 4:
            if ( fast_boot == 0 )
                printf( "  > [Boot Mode]: EMIFA ROM Direct Boot\n" );
            if ( fast_boot == 1 )
                printf( "  > [Boot Mode]: EMIFA ROM Fast Boot\n" );
            break;
        case 5:
            printf( "  > [Boot Mode]: I2C Boot\n" );
            break;
        case 6:
            printf( "  > [Boot Mode]: SPI Boot\n" );
            break;
        case 7:
            printf( "  > [Boot Mode]: NAND Boot\n" );
            break;
        case 8:
            printf( "  > [Boot Mode]: UART Boot\n" );
            break;
        case 11:
            printf( "  > [Boot Mode]: EMAC Boot\n" );
            break;
        default:
            printf( "  >>>>>> ERROR boot option not supported <<<<<<\n" );
            break;
    }

    if ( fast_boot == 0 )
        printf( "  > [Fast Boot]: No\n" );
    if ( fast_boot == 1 )
        printf( "  > [Fast Boot]: Yes\n" );

    if ( emifa_bus_width == 0 )
        printf( "  > [Bus Width]: 8-bit\n" );
    if ( emifa_bus_width == 1 )
        printf( "  > [Bus Width]: 16-bit\n" );

    if ( pci_enable == 0 )
        printf( "  > [PCI]      : OFF\n" );
    if ( pci_enable == 1 )
        printf( "  > [PCI]      : ON\n" );

    if ( endian == 0 )
        printf( "  > [Endianess]: Big Endian\n" );
    if ( endian == 1 )
        printf( "  > [Endianess]: Little Endian\n" );

    switch( aeaw )
    {
        case 0:
            printf( "  > [AEAW][%d]  : EMIFA ADDR[12:0]\n", aeaw );
            break;
        case 4:
            printf( "  > [AEAW][%d]  : EMIFA ADDR[20:0]\n", aeaw );
            break;
        default:
            printf( "  >>>>>> ERROR AEAW not supported <<<<<<\n" );
            break;
    }

    switch( aem )
    {
        case 0:
            printf( "  > [AEM] [%d]  : No boot\n", aem );
            break;
        case 1:
            printf( "  > [AEM] [%d]  : [EMIFA 8-bit][CCDC 16-bit][VENC 8-bit]\n", aem );
            break;
        case 2:
            printf( "  > [AEM] [%d]  : [EMIFA 16-bit]\n", aem );
            break;
        case 3:
            printf( "  > [AEM] [%d]  : [EMIFA 8-bit][CCDC 8-bit][VENC 16-bit]\n", aem );
            break;
        case 4:
            printf( "  > [AEM] [%d]  : [NAND 8-bit][CCDC 8-bit][VENC 16-bit]\n", aem );
            break;
        case 5:
            printf( "  > [AEM] [%d]  : [NAND 8-bit][CCDC 16-bit][VENC 8-bit]\n", aem );
            break;
        default:
            printf( "  >>>>>> ERROR AEM not supported <<<<<<\n" );
            break;
    }

    SW_BREAKPOINT;
    return;
}
