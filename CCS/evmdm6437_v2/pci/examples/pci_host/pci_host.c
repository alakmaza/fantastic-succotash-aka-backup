/*H***************************************************************************
*
* $Archive::                                                                 $
* $Revision::                                                                $
* $Date::                                                                    $
* $Author::                                                                  $
*
* DESCRIPTION:
*
* GLOBALS
*
* PUBLIC FUNCTIONS:
*                         
* PRIVATE FUNCTIONS:

* USAGE/LIMITATIONS:
*
* NOTES:
*   
* (C) Copyright 2003 by Spectrum Digital Incorporated
* All rights reserved
*
*H***************************************************************************/
#define pci_test_c
/*---- compilation control switches ----------------------------------------*/

/*****************************************************************************
* INCLUDE FILES
*****************************************************************************/
/*---- system and platform files -------------------------------------------*/
#ifdef _WIN32
    #if (_MSC_VER >= 900 )
       #define	WIN32_LEAN_AND_MEAN  1
       #define 	INC_OLE2
       #define	NOSERVICE
    #endif 

    #include <windows.h>
    #include <stdio.h>
   
#endif

#include "sd_pci64.h"

/*---- program files -------------------------------------------------------*/

/*****************************************************************************
* EXTERNAL REFERENCE    
*****************************************************************************/
/*---- data declarations ---------------------------------------------------*/
#define IRAM_BASE            0x10800000UL
#define IRAM_LENGTH          0x00004000UL
#define IRAM_ADDR_BITS       14

#define BOOTCOMPLT           0x01C4000CUL

/*---- function prototypes -------------------------------------------------*/

/*****************************************************************************
* PUBLIC DECLARATIONS 
*****************************************************************************/

/*---- data declarations ---------------------------------------------------*/
typedef struct pci_eeprom_config
{
    UINT32  VendorId;                   //0
    UINT32  DeviceId;                   //1
    UINT32  ClassCode_RevisionId;       //2 
    UINT32  ClassCode;                  //3
    UINT32  SubSystemVendorId;          //4
    UINT32  SubSystemId;                //5
    UINT32  MaxLatency_MinGrant;        //6 
    UINT32  PCD1_PCD0;                  //7
    UINT32  PCD3_PCD2;                  //8 
    UINT32  PDD1_PDD0;                  //9
    UINT32  PDD3_PDD2;                  //A 
    UINT32  DataScale;                  //B
    UINT32  Pmc;                        //C
    UINT32  CheckSum;                   //D 
} PCI64_EEPROM_CONFIG;

typedef struct pci_intr_cntl
{
    UINT32  Rstsrc;
    UINT32  Pmdcsr;
    UINT32  Pciis;
    UINT32  Pciien;
} PCI64_INTR_CNTL;

/*****************************************************************************
* PRIVATE DECLARATIONS  
*****************************************************************************/
/*---- context -------------------------------------------------------------*/

/*---- data declarations ---------------------------------------------------*/
HANDLE hCallbackEvent = NULL;

volatile int    gIntsSeen = 0;

/*---- function prototypes -------------------------------------------------*/

/*---- macros --------------------------------------------------------------*/

/*****************************************************************************
* PUBLIC FUNCTION DEFINITIONS
*****************************************************************************/

DWORD WINAPI Callback(LPVOID dummy)
{
    if( hCallbackEvent == NULL )
        return(-1);

	while(1)
	{
		if(WaitForSingleObject(hCallbackEvent, INFINITE))
			printf
			(
				"\nWaiting for interrupt on Event 0x%x\n", 
				hCallbackEvent
			);		
		else
			printf("\nInterrupt # %d has been generated\n", ++gIntsSeen);
	}

	return 0;
}

/*F***************************************************************************
* NAME:  MEM_SlidingData32()
*
* DESCRIPTION:  Sliding data test.  Tests walking 1/0 accross the data bus.
*         
* PARMETERS:
*
* NOTES: 
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
INT32
MEM_SlidingData32( PCI64_HANDLE hPci, UINT32 Addr, UINT32 Pattern, UINT32 Count )
{
	INT32      Error = 0;
	UINT32     Value = 1;
    UINT32     MemPattern[2];
	UINT32     i;

	for( i=0; i<Count; i++ )
	{
	    if( Pattern )
	    {   // Test for sliding 1
		    MemPattern[0] = Value;
            MemPattern[1] = ~Value;
            Error = PCI64_MemWrite32( hPci, Addr,
                                            2,
                                            MemPattern );

            if( Error ) 
                goto EXIT_MEM_SlidingData32;


            Error = PCI64_MemRead32( hPci,  Addr,
                                            2,
                                            MemPattern );
    
		    if( MemPattern[0] != Value )
		    	Error++;
		}
		else
		{  // Test for sliding 0
            MemPattern[0] = ~Value;
            MemPattern[1] = Value;

            Error = PCI64_MemWrite32( hPci, Addr,
                                            2,
                                            MemPattern );

            if( Error ) 
                goto EXIT_MEM_SlidingData32;


            Error = PCI64_MemRead32( hPci,  Addr,
                                            2,
                                            MemPattern );
    
		    if( MemPattern[0] != ~Value )
		    	Error++;
		}
	    	
	    Value = Value << 1;
	}

EXIT_MEM_SlidingData32:
	return( Error );
}

/*F***************************************************************************
* NAME:  MEM_SlidingAddr32()
*
* DESCRIPTION:   Sliding address test.  Slides 1 accross the address bus.
*         
* PARMETERS:
*
* NOTES: 
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
INT32
MEM_SlidingAddr32(  PCI64_HANDLE hPci,UINT32 DramAddr, UINT32 Count )
{

	UINT32            TestAddr;
	UINT32            BckgndAddr;
	UINT32            TestPattern,NotTestPattern,ReadPattern;
	UINT32            i;
	UINT32            j;
	INT32             Error;
	
	
	// The address bit must be on an 4 byte boundary for this test.
	// We want to have only 1 address bit change and all bytes 
	// strobes in a word.
	TestPattern = 0x87654321;
	Error       = 0;
	
	for( i=0; i<Count; i++ )
	{
	    TestAddr = DramAddr + (4<<i);
	    
	    // Write the test pattern
        PCI64_MemWrite32( hPci, TestAddr,1,&TestPattern );	    

	    // Now write to all the other address bits with compliment data
	    // to test for disturbance.
		for( j=0; j<Count; j++ )
		{
			BckgndAddr = DramAddr + (4<<j);
			if( TestAddr != BckgndAddr )
			{
	    		NotTestPattern = ~TestPattern+i;
                PCI64_MemWrite32( hPci, BckgndAddr,1,&NotTestPattern );

	    	}
	    }
	    PCI64_MemRead32( hPci, TestAddr,1,&ReadPattern );
		if( ReadPattern != TestPattern )
		    Error++;
	}
	return( Error );
}

/*F***************************************************************************
* NAME:  TestIram()
*
* DESCRIPTION:  Test on-chip SRAM.
*         
* PARMETERS:
*
* NOTES: 
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/

INT32 TestIram( PCI64_HANDLE hPci )
{
    UINT32 * pMemBuf = NULL;
    UINT32   i;
    INT32    Error = 0;

    Error = MEM_SlidingData32(  hPci, IRAM_BASE, 1, 32 );
    if( Error )
        printf("Failed sliding data test 1\n");

    Error = MEM_SlidingData32(  hPci, IRAM_BASE, 0, 32 );
    if( Error )
        printf("Failed sliding data test 0\n");


    Error = MEM_SlidingAddr32(  hPci, IRAM_BASE, IRAM_ADDR_BITS );
    if( Error )
        printf("Failed sliding address test\n");

    pMemBuf = (UINT32 *)malloc(IRAM_LENGTH+10);
    if( pMemBuf == NULL )
        return( -1 );

    for(i=0;i<IRAM_LENGTH/sizeof(UINT32); i++ )
        pMemBuf[i] = i;

    // The count must be in complete UINT32.
    PCI64_MemWrite32( hPci, (UINT32)IRAM_BASE,
                            (UINT32)IRAM_LENGTH/sizeof(UINT32),
                            (UINT32*)pMemBuf );

    ZeroMemory( pMemBuf, IRAM_LENGTH+10 );

    PCI64_MemRead32( hPci, (UINT32)IRAM_BASE,
                           (UINT32)IRAM_LENGTH/sizeof(UINT32),
                           (UINT32*)pMemBuf );

    for(i=0;i<IRAM_LENGTH/sizeof(UINT32); i++ )
        if( pMemBuf[i] != i )
            Error++;

    if( Error )
        printf("Failed data increment test\n");

    if( pMemBuf != NULL )
        free( pMemBuf );

    return( Error );

}


/*F***************************************************************************
* NAME: main()
*
* DESCRIPTION:  Sample test code.
*         
* PARMETERS:
*
* NOTES: 
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
int main(int argc, char *argv[])
{
	INT32        Error;
	PCI64_HANDLE MyPci;
    UINT32       FillPattern = 0UL;
    UINT32       data;

    // Open connection to SI6XWDM driver
    printf("Open PCI channel\n");

	Error = PCI64_Open( 0, &MyPci );
	if( Error )
		goto EXIT_main;

    // Get the callback event handle
    Error = 0;
    hCallbackEvent = PCI64_GetCallbackEvent( MyPci );
    if( hCallbackEvent != NULL )
        Error = PCI64_SetupCallbackEvent( MyPci, (void *)Callback);
    else
        Error = -1;

    if( Error )
    {
        printf("Callback event setup failed\n");
        Error = 0;
    }

    // Test the internal SRAM
    printf("Test DSP Internal SRAM\n");
    Error = TestIram( MyPci );
	if ( Error )
		printf("---> Internal SRAM FAIL\n");
	else
		printf("---> Internal SRAM PASS\n");

    // Fill the memory with pattern before loading.  Not required
    // just included for testing.
    PCI64_MemFill32( MyPci, (UINT32)IRAM_BASE,
                            (UINT32)IRAM_LENGTH/sizeof(UINT32),
                                    &FillPattern );

	// Check BOOTCOMPLT.BC to make sure this is the first boot attempt
    PCI64_RegRead32( MyPci,BOOTCOMPLT,1, &data );
    if ((data & 0x1) == 1)
	{
		printf("Boot process already completed (BOOTCOMPLT.BC = 1), please reboot your system.\n");
		goto EXIT_main;
	}

    // Check command line arguments
    if( argc == 1 )
	{
		// If no arguments, load pci_slave.out and check to see that it ran
		printf("Loading pci_slave.out\n");

		Error = PCI64_LoadCoff( MyPci, "pci_slave.out");
    	if ( Error )
	    	printf("---> pci_slave.out not loaded\n");
	    else
		    printf("---> pci_slave.out loaded\n");

		// Set BOOTCOMPLT.BC to tell bootloader to execute code
        PCI64_RegRead32( MyPci,BOOTCOMPLT,1, &data );
		data |= 1;
        PCI64_RegWrite32( MyPci,BOOTCOMPLT,1, &data );

        // Let the test code run for 3 seconds before moving on
        Sleep(3000);
        
		// Check magic number for sucessful completion (see pci_slave)
        Error = PCI64_MemRead32( MyPci,  0x10800000UL,
                                        1,
                                        &data );
		if (data == 0xABCD1234)
			printf("---> pci_slave ran sucessfully\n");
		else
			printf("---> pci_slave did not run successfully\n");

	} else
	{
        printf("Loading %s\n",argv[1]);

        Error = PCI64_LoadCoff( MyPci, argv[1] );
    	if ( Error )
	    	printf("---> %s not loaded\n", argv[1]);
	    else
		    printf("---> %s loaded\n", argv[1]);
        if( Error )
            goto EXIT_main;

		// Set BOOTCOMPLT.BC to tell bootloader to execute code
        PCI64_RegRead32( MyPci,BOOTCOMPLT,1, &data );
		data |= 1;
        PCI64_RegWrite32( MyPci,BOOTCOMPLT,1, &data );
    }

EXIT_main:
	// Wait for 5 seconds so user can see program output
	printf("Waiting to close...\n");
	Sleep(3000);

    // Always close the PCI connection on exit.
    printf("Close PCI channel\n");
	PCI64_Close( MyPci );
}