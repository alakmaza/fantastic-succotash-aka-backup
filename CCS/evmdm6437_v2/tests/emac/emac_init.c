/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

#include "emac.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  EMAC/MDIO init                                                          *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void emac_init( )
{
    Int16 i;
    Int16 rmii_mode = 0;
    Uint16 phy_ctrl_reg;

    volatile Uint32 *pReg;

    if ( rmii_mode )
    {
	    // RMII Not supported
        return;
    }
    else
    {
        /* ---------------------------------------------------------------- *
         *                                                                  *
         *  Init PHY / MDIO                                                 *
         *                                                                  *
         * ---------------------------------------------------------------- */
		MDIO_CONTROL = 0
			| ( 0 << 30 )                       // Disable MDIO
			| ( 0 << 20 )                       // Standard MDIO preamble
			| ( 1 << 19 )                       // Clear Fault
			| ( 0 << 18 )                       // Disable Fault
			| ( 1 << 0 );                       // Clock Divider

		MDIO_CONTROL |= ( 1 << 30 );            // Enable MDIO

		MDIO_USERPHYSEL0 = 0
		    | ( 0 << 7 )
			| ( 0 << 6 )
			| ( 0 << 0 );

        MDIO_USERACCESS0 = 0                    // Read Phy Id 1
            | ( 1 << 31 )                       // [31] Go
            | ( 0 << 30 )                       // [30] Read
            | ( 0 << 29 )                       // [29] Ack
            | ( 2 << 21 )                       // [25-21] PHY register address
            | ( 1 << 16 )                       // [20-16] PHY address
            | ( 0 << 0 );                       // [15-0] Data

        while( MDIO_USERACCESS0 & 0x80000000 ); // Wait for Results

        phy_ctrl_reg = 0
            | ( 0 << 15 )                       // Reset
            | ( 0 << 14 )                       // Loopback
            | ( 0 << 13 )                       // Speed Selection
            | ( 0 << 12 )                       // Auto-Negotiation Enable
            | ( 0 << 11 )                       // Power-Down
            | ( 0 << 10 )                       // Isolate
            | ( 0 << 9 )                        // Restart Auto-Negotiation
            | ( 0 << 8 )                        // Duplex Mode
            | ( 0 << 7 )                        // Collision Test
            | ( 0 << 6 );                       // Speed Selection

        MDIO_USERACCESS0 = 0                    // Set Control Reg
            | ( 1 << 31 )                       // [31] Go
            | ( 1 << 30 )                       // [30] Write
            | ( 0 << 29 )                       // [29] Ack
            | ( 0 << 21 )                       // [25-21] PHY register address
            | ( 1 << 16 )                       // [20-16] PHY address
            | ( phy_ctrl_reg << 0 );            // [15-0] Data

        while( MDIO_USERACCESS0 & 0x80000000 ); // Wait for Results
    }

    /*
     *  0. Reset Ethernet
     */
    EMAC_SOFTRESET = 1;
    while( EMAC_SOFTRESET != 0 );

    /*
     *  1. Clear interrupt enable
     */
    EMAC_EWCTL = 0;

    /*
     *  2. Clear the MAC control, receive control, & transmit control.
     */
    EMAC_MACCONTROL = 0;
    EMAC_RXCONTROL = 0;
    EMAC_TXCONTROL = 0;

    /*
     *  3. Initialize all 16 header descriptor pointers RXnHDP & TXnHDP to zero
     */
    pReg = &EMAC_RX0HDP;
    for ( i = 0 ; i < 8 ; i++ )
        *pReg++ = 0;

    pReg = &EMAC_TX0HDP;
    for ( i = 0 ; i < 8 ; i++ )
        *pReg++ = 0;

    /*
     *  4. Clear all 36 statistics registers by writing 0
     */
    pReg = &EMAC_RXGOODFRAMES;
    for ( i = 0 ; i < 36 ; i++ )
        *pReg++ = 0;

    /*
     *  5. Setup the local Ethernet MAC address.
     *     Be sure to program all 8 MAC addresses
     */
    EMAC_MACINDEX  = 0x00;
    EMAC_MACADDRHI = 0x03020100;    // Needs to be written only the first time
    EMAC_MACADDRLO = 0x0504;

    EMAC_MACINDEX  = 0x01;
    EMAC_MACADDRLO = 0x1504;

    EMAC_MACINDEX  = 0x02;
    EMAC_MACADDRLO = 0x2504;

    EMAC_MACINDEX  = 0x03;
    EMAC_MACADDRLO = 0x3504;

    EMAC_MACINDEX  = 0x04;
    EMAC_MACADDRLO = 0x4504;

    EMAC_MACINDEX  = 0x05;
    EMAC_MACADDRLO = 0x5504;

    EMAC_MACINDEX  = 0x06;
    EMAC_MACADDRLO = 0x6504;

    EMAC_MACINDEX  = 0x07;
    EMAC_MACADDRLO = 0x7504;

    /*
     *  6. Initialize the receive channel N
     */

    /*
     *  7. No multicast addressing
     */
    EMAC_MACHASH1 = 0;
    EMAC_MACHASH2 = 0;

    /*
     *  8. Set RX buffer offset to 0
     *     Valid data always begins on the 1st byte
     */
    EMAC_RXBUFFEROFFSET = 0;

    /*
     *  9. Enable Unicast RX on channel 0-7
     */
    EMAC_RXUNICASTSET = 0xFF;


    /*
     *  10. Setup the RX( M )ulticast ( B )roadcast ( P )romiscuous channel
     */
    EMAC_RXMBPENABLE = 0;

    /*
     *  11. Set the appropriate configuration bits in MACCONTROL (do not set the GMIIEN bit yet).
     */
    EMAC_MACCONTROL = 0
        | ( rmii_mode << 15 )   // 100MHz RMII
        | ( 0 << 9 )            // Round robin
        | ( 0 << 6 )            // TX pacing disabled
        | ( 0 << 5 )            // GMII RX & TX
        | ( 0 << 4 )            // TX flow disabled
        | ( 0 << 3 )            // RX flow disabled
        | ( 0 << 1 )            // Loopback disabled
        | ( 1 << 0 );           // full duplex

    /*
     *  12. Clear all unused channel interrupt bits
     */
    EMAC_RXINTMASKCLEAR = 0xFF;
    EMAC_TXINTMASKCLEAR = 0xFF;

    /*
     *  13. Enable the RX & TX channel interrupt bits.
     */
    EMAC_RXINTMASKSET = 0xFF;
    EMAC_TXINTMASKSET = 0xFF;

    /* Enable Host Error and Statistics interrupts */
    EMAC_MACINTMASKSET = 0
        | ( 1 << 1 )        // Host Error interrupt mask
        | ( 1 << 0 );       // Statistics interrupt mask

    /*
     *  14. Initialize the receive and transmit descriptor list queues.
     */

    /*
     *  15. Prepare receive by writing a pointer to the head of the receive buffer descriptor list to RXnHDP.
     */
    EMAC_MACSRCADDRLO = 0x03020100;   /* bytes 0, 1 */
    EMAC_MACSRCADDRHI = 0x0405;       /* bytes 2-5 - channel 0 ??? */

    /*
     *  16. Enable the RX & TX DMA controllers.
     *      Then set GMIIEN
     */
    EMAC_RXCONTROL = 1;
    EMAC_TXCONTROL = 1;
    EMAC_MACCONTROL |= ( 1 << 5 );

    /* 17. Enable the device interrupt in EWCTL. */
    EMAC_EWCTL = 1;
}
