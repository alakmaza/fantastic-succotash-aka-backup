/*
 *  Copyright 2007 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 *
 *  Not for distribution.
 */

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main( void )
{
    // Write magic number so pci_host.out can check to see if the code ran
    *((volatile unsigned long *)0x10800000) = 0xabcd1234;

	while(1);
}
