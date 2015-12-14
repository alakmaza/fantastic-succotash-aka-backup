/*
 *  Copyright 2006 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  CAN test
 */

#include "evmdm6437.h"
#include "hecc.h"

HECC_Handle hecc = &HECC_MODULE;

/* ------------------------------------------------------------------------ *
 *  can_setup( )                                                            *
 * ------------------------------------------------------------------------ */
void can_setup( )
{
    /* Setup pin mux for CAN test */
    EVMDM6437_clrPinMux( 0, ( 3 << 20 ) );
    EVMDM6437_setPinMux( 0, ( 3 << 20 ) );
}

/* ------------------------------------------------------------------------ *
 *  can_start( )                                                            *
 * ------------------------------------------------------------------------ */
Int16 can_start( )
{
    Int32 i;
    Int16 mode;

    hecc->regs->TIOC |= 0x0008;                 // Set for CAN TX pin
    hecc->regs->RIOC |= 0x0008;                 // Set for CAN RX pin

    hecc->regs->MC |= 0x1000;                   // Enter Init mode
    while( ( hecc->regs->ES & 0x10 ) == 0 );

    hecc->regs->BTC = 0                         // Set BTC timing = 0x000200B1
        | ( 2 << 16 )                           // [8] BRP
        | ( 0 << 10 )                           // ERM
        | ( 0 << 8 )                            // [2] SJW
        | ( 1 << 7 )                            // SAM
        | ( 6 << 3 )                            // [4] TSEG1
        | ( 1 << 0 );                           // [3] TSEG2

    hecc->regs->MC &= ~0x1000;                  // Exit Init mode
    while( ( hecc->regs->ES & 0x10 ) != 0 );

    hecc->regs->MC = 0                          // Clear Master Control
        | ( 0 << 11 )                           // Power Down Request bit
        | ( 0 << 10 )                           // Data Byte Order
        | ( 0 << 9 )                            // Wake up on bus activity
        | ( 0 << 8 )                            // Change Data Request
        | ( 0 << 7 )                            // Auto Bus on
        | ( 0 << 6 )                            // Self-test mode
        | ( 0 << 5 )                            // S/W Reset
        | ( 0 << 4 );                           // [5] Mailbox number

    while( ( hecc->regs->ES & 0x0010 ) != 0 );  // Exit Init mode

    mode = ( hecc->regs->MC >> 13 ) & 1;        // Save mode

    hecc->regs->MC |= ( 1 << 5 );               // Issue a S/W reset

    hecc->regs->MC &= ~( 1 << 12 );             // After S/W reset, Exit out of Init mode

    while( ( hecc->regs->ES & 0x0010 ) != 0 );  // Exit init mode

    if ( mode == 1 )                            // Restore the Mode
        hecc->regs->MC |= ( 1 << 13 );
    else
        hecc->regs->MC &= ~( 1 << 13 );

    /*
     *  Clear regs
     *      Transmit Acknowledge( TA )
     *      Receive Message Pending( RMP )
     *      Global interrupt flag0( GIF0 )
     *      Global interrupt flag1( GIF1 )
     */
    hecc->regs->TA   = 0xFFFFFFFF;
    hecc->regs->RMP  = 0xFFFFFFFF;
    hecc->regs->GIF0 = 0xFFFFFFFF;
    hecc->regs->GIF1 = 0xFFFFFFFF;

    /* Clean up all the Mailboxes */
    for ( i = 0 ; i < 32 ; i++ )
    {
        hecc->regs->MBX[i].MID = 0;
        hecc->regs->MBX[i].MCF = 0;
        hecc->regs->MBX[i].MDL = 0;
        hecc->regs->MBX[i].MDH = 0;
    }

    return 0;
}

Uint32 tx_msg_idreg;
Uint32 tx_msg_mcfreg;

/* ------------------------------------------------------------------------ *
 *  can_tx_setup( )                                                         *
 * ------------------------------------------------------------------------ */
Int16 can_tx_setup( Uint32 mailbox, Uint32 msg_id, Uint32 tx_length, Int32 tx_priority )
{
    Uint32 mailbox_bit = 1 << mailbox;

    tx_priority = tx_priority & 0x3F;
    tx_length   = tx_length & 0x0F;

    hecc->regs->ME &= ~mailbox_bit;             // Disable mailbox

    tx_msg_idreg = 0
        | ( STANDARD_ID << 31 )                 // Set the identifier type
        | ( 0 << 30 )                           // Clear Acceptance Mask Enable( AME )
        | ( 0 << 29 )                           // Auto-Answer mode( AAM )
        | ( msg_id << 0 );                      // Load the identifier

    tx_msg_mcfreg = 0
        | ( tx_priority << 8 )                  // Set Priority
        | ( 0 << 4 )                            // No Remote Frame
        | ( tx_length << 0 );                   // Data length

    hecc->regs->MD &= ~mailbox_bit;             // Set transmit mode
    hecc->regs->ME |= mailbox_bit;              // Enable mailbox

    return 0;
}

/* ------------------------------------------------------------------------ *
 *  can_tx( )                                                               *
 * ------------------------------------------------------------------------ */
Int16 can_tx( Uint32 mailbox, Uint32 low, Uint32 high )
{
    Uint32 mailbox_bit = 1 << mailbox;

    /* Write into Mailbox# */
    hecc->regs->ME &= ~mailbox_bit;             // Disable MBX
    hecc->regs->MBX[mailbox].MID = tx_msg_idreg;
    hecc->regs->ME |= mailbox_bit;              // Enable MBX

    hecc->regs->MBX[mailbox].MCF = tx_msg_mcfreg;
    hecc->regs->MBX[mailbox].MDL = low;
    hecc->regs->MBX[mailbox].MDH = high;

    /* Start transmission */
    hecc->regs->TRS |= mailbox_bit;
    while( hecc->regs->TA == 0 );

    return 0;
}

/* ------------------------------------------------------------------------ *
 *  can_rx_setup( )                                                         *
 * ------------------------------------------------------------------------ */
Int16 can_rx_setup( Uint32 mailbox, Uint32 msg_id )
{
    Uint32 mailbox_bit = 1 << mailbox;

    hecc->regs->ME &= ~mailbox_bit;             // Disable mailbox

    hecc->regs->MBX[mailbox].MID = 0
        | ( STANDARD_ID << 31 )                 // Set identifier type
        | ( 0 << 30 )                           // Clear AME
        | ( 0 << 29 )                           // Clear AAM, RTR
        | ( msg_id << 0 );                      // Load the identifier

    hecc->regs->MBX[mailbox].MCF &= ~( 1 << 4 ); // No Frames
    hecc->regs->MD |= mailbox_bit;              // Set receive mode
    hecc->regs->ME |= mailbox_bit;              // Enable mailbox
    hecc->regs->MC &= ~mailbox_bit;             // Clear loopback mode

    return 0;
}

/* ------------------------------------------------------------------------ *
 *  can_rx( )                                                               *
 * ------------------------------------------------------------------------ */
Int16 can_rx( Uint32 mailbox, Uint32* low, Uint32* high )
{
    Uint32 mailbox_bit = 1 << mailbox;

    Uint32 rx_msg_id;
    Uint32 rx_data_length;
    Uint32 rx_time_stamp;

	Int32 blink_count = 400000;
	Int32 count;

	count = blink_count;

    /* Wait for the data on the mailbox# */
    while ( ( hecc->regs->RMP & mailbox_bit )== 0 )
    {
		if ( count-- < 0 )
		{
		    EVMDM6437_LED_toggle( 0 );
			count = blink_count;
		}
	}

    hecc->regs->RMP |= mailbox_bit;

    /* When data is available, read the mailbox contents */
    *low            = hecc->regs->MBX[mailbox].MDL;
    *high           = hecc->regs->MBX[mailbox].MDH;
    rx_data_length  = hecc->regs->MBX[mailbox].MCF & 0x0F;
    rx_time_stamp   = hecc->regs->MOTS[mailbox];
    rx_msg_id       = hecc->regs->MBX[mailbox].MID & 0x1FFFFFFF;

    return 0;
}

/* ------------------------------------------------------------------------ *
 *  can_test( )                                                             *
 * ------------------------------------------------------------------------ */
Int16 can_test( )
{
    Uint32 tx_mailbox = 3;
    Uint32 rx_mailbox = 6;

    Uint32 tx_data_low, tx_data_high;
    Uint32 rx_data_low, rx_data_high;

    Uint32 msg_id = 0x04C08E2F;

	while ( 1 )
	{
	    /* -------------------------------- *
	     *  Setup CAN ( Pin Mux )           *
	     * -------------------------------- */
	    can_setup( );

	    /* -------------------------------- *
	     *  Start CAN ( Initialization )    *
	     * -------------------------------- */
	    can_start( );

	    /* -------------------------------- *
	     *  Setup TX ( Mailbox# 3 )         *
	     * -------------------------------- */
	    can_tx_setup( tx_mailbox, msg_id, DATA_LENGTH_4, PRIORITY_0 );

	    /* -------------------------------- *
	     *  Setup RX ( Mailbox# 6 )         *
	     * -------------------------------- */
	    can_rx_setup( rx_mailbox, msg_id );

	    /* -------------------------------- *
	     *  CAN Test                        *
	     * -------------------------------- */
	    while ( 1 )
	    {
	        /* -------------------------------- *
	         *  Receive data in sync            *
	         * -------------------------------- */
	        can_rx( rx_mailbox, &rx_data_low, &rx_data_high );

	        tx_data_low = ~rx_data_low;
	        tx_data_high = ~rx_data_high;

	        /* -------------------------------- *
	         *  Transmit data async             *
	         * -------------------------------- */
	        can_tx( tx_mailbox, tx_data_low, tx_data_high );

			if ( rx_data_low == 0xFFFF )
				break;
	    }
	}
}
