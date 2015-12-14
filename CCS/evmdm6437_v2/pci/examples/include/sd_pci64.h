/*H***************************************************************************
*
* $Archive::                                                                 $
* $Revision::                                                                $
* $Date::                                                                    $
* $Author::                                                                  $
*
* DESCRIPTION: PCI interface functions for DM642 EVM.  These functions
*              are exported from sd_pci64.dll.  The sd_pci64.dll provides
*              a simply wrapper to the Sheldon Instruments SIC6XWDM driver.
*
* USAGE/LIMITATIONS: sd_pci64.dll and SIC6XWDM are  supported on WDM
*                    compatible OSes, which are Win98, Win2K and WinXP.
*        
*                    Supports little endian mode only.
*                    All PCI writes/reads are to 32 bit aligned data.
*
* NOTES:  If you need more complete PCI support then Sheldon Instruments
*         sells a driver package to support the C64xx/DM642.  Visit
*         www.sheldoninst.com.
*
*         Beware that TI tools may not align coff sections on 32 bit
*         boundary.  It is up to user to format data such that it does
*         not violate the 32 bit alignment restriction.
*   
* (C) Copyright 2003 by Spectrum Digital Incorporated
* All rights reserved
*
*H***************************************************************************/

#ifndef sd_pci64_h
#define sd_pci64_h

#ifdef __cplusplus
extern "C" {
#endif
/*---- compilation control switches ----------------------------------------*/

/*****************************************************************************
* INCLUDE FILES (minimize nesting of header files)
*****************************************************************************/

/*---- system and platform files -------------------------------------------*/

/*---- program files -------------------------------------------------------*/

/*****************************************************************************
* FILE CONTENT
*****************************************************************************/

/*****************************************************************************
* FUNCTIONAL AREA DETAIL
*****************************************************************************/

/*A***************************************************************************
* NAME:  
*
* USAGE: 
*
* NOTES:  
*
*A***************************************************************************/

/*---- context -------------------------------------------------------------*/

#if !defined( PCI64_MAX_BOARDS )
    #define PCI64_MAX_BOARDS         4
#endif

/*---- data descriptions ---------------------------------------------------*/
typedef  void * PCI64_HANDLE;

/*---- global data declarations --------------------------------------------*/

/*---- global function prototypes ------------------------------------------*/
#if defined(_WIN32) 
    #if defined(SD_PCI64_EXPORTS)
        #define PCI64_API __declspec(dllexport)
    #else   
        #define PCI64_API __declspec(dllimport)
    #endif
#else
    #define PCI64_API
#endif

/*F***************************************************************************
* NAME:  PCI64_Open()
*
* DESCRIPTION:  Open access to DM642 EVM at BoardNum
*
* PARMETERS:
*   BoardNum       - Board number 0-3
*   pHndl          - Pointer to return handle
*     
* NOTES: The return handle can be valid even if PCI64_Open fails.  In this 
*        case you should immediately call PCI64_Close.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*   
*F***************************************************************************/
PCI64_API INT32 PCI64_Open( int BoardNum,               
                            PCI64_HANDLE * pHndl );     

/*F***************************************************************************
* NAME:  PCI64_Close()
*
* DESCRIPTION:  Close access to DM642 EVM.  Should always close
*   
* PARMETERS:
*   Hndl           - Handle returned from PCI64_Open
*
* NOTES: Always call PCI64_Close() even if PCI64_Open fails.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 PCI64_Close( PCI64_HANDLE Hndl );

/*F***************************************************************************
* NAME:  PCI64_MemWrite32()
*
* DESCRIPTION:  Write Count 32 bit values to Taddr.
*         
* PARMETERS:
*   Hndl           - Handle returned from PCI64_Open
*   Taddr          - C64x target address  
*   Count          - Number of 32 bit data values
*   pData          - Pointer to array of 32 bit data types  
*
* NOTES: Taddr must be aligned to 32 bit address boundary.  
*        Count must be less then 8M bytes or the basic 64xx PCI memory 
*          window.
*        Taddr+Count must not cross a 8M byte PCI page boundary.
*
*        Function maps PCI region 4 to the base of the address space
*        specified by Taddr and then performs the write operation.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 PCI64_MemWrite32( PCI64_HANDLE Hndl,  
                                  UINT32   Taddr,     
                                  UINT32   Count,      
                                  UINT32 * pData );

/*F***************************************************************************
* NAME:  PCI64_MemFill32()
*
* DESCRIPTION:  Fill Count 32 bit values to Taddr.
*
* PARMETERS:
*   Hndl           - Handle returned from PCI64_Open
*   Taddr          - C64x target address  
*   Count          - Number of 32 bit data values
*   pData          - Pointer to array of 32 bit data types  
*         
* NOTES: Taddr must be aligned to 32 bit address boundary.  
*        Count must be less then 8M bytes or the basic 64xx PCI memory 
*          window.
*        Taddr+Count must not cross a 8M byte PCI page boundary.
*
*        Function maps PCI region 4 to the base of the address space
*        specified by Taddr and then performs the memfill operation.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 PCI64_MemFill32( PCI64_HANDLE Hndl,  UINT32   Taddr,
                                                     UINT32   Count,
                                                     UINT32 * pData );

/*F***************************************************************************
* NAME:  PCI64_MemRead32()
*
* DESCRIPTION:  Read Count 32 bit values from Taddr.
*   
* PARMETERS:
*   Hndl           - Handle returned from PCI64_Open
*   Taddr          - C64x target address  
*   Count          - Number of 32 bit data values
*   pData          - Pointer to array of 32 bit data types  
*      
* NOTES: Taddr must be aligned to 32 bit address boundary.  
*        Count must be less then 8M bytes or the basic 64xx PCI memory 
*          window.
*        Taddr+Count must not cross a 8M byte PCI page boundary.
*
*        Function maps PCI region 4 to the base of the address space
*        specified by Taddr and then performs the read operation.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 PCI64_MemRead32( PCI64_HANDLE Hndl,  UINT32   Taddr,
                                                     UINT32   Count,
                                                     UINT32 * pData );


/*F***************************************************************************
* NAME:  PCI64_RegWrite32()
*
* DESCRIPTION:  Write Count 32 bit values to Taddr.
*
* PARMETERS:
*   Hndl           - Handle returned from PCI64_Open
*   Taddr          - C64x target address  
*   Count          - Number of 32 bit data values
*   pData          - Pointer to array of 32 bit data types  
*         
* NOTES: Taddr must be aligned to 32 bit address boundary.  
*        Count must be less then 1024.
*        
*        Function uses Base 1 address of 8M byte non-prefecthable region.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 PCI64_RegWrite32( PCI64_HANDLE Hndl, UINT32   Taddr,
                                                     UINT32   Count,
                                                     UINT32 * pData );


/*F***************************************************************************
* NAME:  PCI64_RegRead32()
*
* DESCRIPTION:  Read Count 32 bit values from Taddr.
* 
* PARMETERS:
*   Hndl           - Handle returned from PCI64_Open
*   Taddr          - C64x target address  
*   Count          - Number of 32 bit data values
*   pData          - Pointer to array of 32 bit data types  
*        
* NOTES: Taddr must be aligned to 32 bit address boundary.  
*        Count must be less then 1024.
*        
*        Function uses the PCI region 2 default address mapping at DSP
*        address 0x01C00000 with a range of 4MBytes.  It can only address
*        registers in that range.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 PCI64_RegRead32( PCI64_HANDLE Hndl,  UINT32   Taddr,
                                                     UINT32   Count,
                                                     UINT32 * pData );

/*F***************************************************************************
* NAME:  PCI64_LoadCoff()
*
* DESCRIPTION:  Load Coff image to PCI target board
*    
* PARMETERS:
*   Hndl           - Handle returned from PCI64_Open
*   pCoffName      - Pointer to coff name string 
*     
* NOTES: The following restrictions apply:
*        -C64xx is held in reset.
*        -C64xx EMIF has been setup if loading off chip
*        -Non-relocatable coff file.
*        -All COFF sections must begin on a 32 bit boundary.  Beware this
*          may require link command file changes to ensure alignment. 
*          If a COFF ends on a non-32 bit boundary then the remaining
*          1-3 bytes will be filled with zero(s). 
*        -C64xx will start execution from address 0x00000000 per C64xx
*          PCI boot mode.
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 PCI64_LoadCoff( PCI64_HANDLE hPci, char * pCoffName);


/*F***************************************************************************
* NAME:  PCI64_GetCallbackEvent()
*
* DESCRIPTION:  Get the handle used for callback events
*    
* PARMETERS:
*   Hndl  - Handle returned from PCI64_Open
*
* NOTES: This call is only valid after a successfull PCI64_Open call.
*
* RETURN VALUE:
*     HANDLE for Success; NULL for Fail
*
*F***************************************************************************/
PCI64_API HANDLE PCI64_GetCallbackEvent( PCI64_HANDLE hPci );

/*F***************************************************************************
* NAME:  PCI64_SetupCallbackEvent()
*
* DESCRIPTION:  Setup a callback function to respond to PCI interrupts
*    
* PARMETERS:
*   Hndl                  - Handle returned from PCI64_Open
*   callbackFunction      - Pointer to callback function of type
*                             DWORD WINAPI Callback(LPVOID dummy)
*
* NOTES:  This function creates a thread to run the Callback function.
*
*
* RETURN VALUE:
*    0 Success, non-0 for Fail
*
*F***************************************************************************/
PCI64_API INT32 
PCI64_SetupCallbackEvent(PCI64_HANDLE Hndl, void *callbackFunction);


#ifdef __cplusplus
}       // Balance extern "C" above
#endif
#endif /* sd_pci64_h ---- END OF FILE ----------------------------------------*/

