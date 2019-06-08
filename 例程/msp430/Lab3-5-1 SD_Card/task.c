/*
 * task.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */

#include "task.h"
#include "msp430.h"

struct USBMSC_mediaInfoStr mediaInfo;
/*
 * ======== checkInsertionRemoval ========
 *
 * This function checks for insertion/removal of the card.  If either is detected, it informs the API
 * by calling USBMSC_updateMediaInfo().  Whether it detects it or not, it returns non-zero if the card
 * is present, or zero if not present
 */
BYTE checkInsertionRemoval (void)
{
    BYTE newCardStatus = detectCard();                                  //Check card status -- there or not?

    if ((newCardStatus) &&
        (mediaInfo.mediaPresent == kUSBMSC_MEDIA_NOT_PRESENT)){
        //An insertion has been detected -- inform the API
        mediaInfo.mediaPresent = kUSBMSC_MEDIA_PRESENT;
        mediaInfo.mediaChanged = 0x01;
        DRESULT SDCard_result = disk_ioctl(0,
            GET_SECTOR_COUNT,
            &mediaInfo.lastBlockLba);                                   //Get the size of this new medium
        USBMSC_updateMediaInfo(0, &mediaInfo);
    }

    if ((!newCardStatus) && (mediaInfo.mediaPresent == kUSBMSC_MEDIA_PRESENT)){
        //A removal has been detected -- inform the API
        mediaInfo.mediaPresent = kUSBMSC_MEDIA_NOT_PRESENT;
        mediaInfo.mediaChanged = 0x01;
        USBMSC_updateMediaInfo(0, &mediaInfo);
    }

    return ( newCardStatus) ;
}

/*
 * ======== Init_Clock ========
 */
VOID Init_Clock (VOID)
{
    if (USB_PLL_XT == 2){
		#if defined (__MSP430F552x) || defined (__MSP430F550x)
			P5SEL |= 0x0C;                                      //enable XT2 pins for F5529
		#elif defined (__MSP430F563x_F663x)
			P7SEL |= 0x0C;
		#endif

        //use REFO for FLL and ACLK
        UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);

        //MCLK will be driven by the FLL (not by XT2), referenced to the REFO
        Init_FLL_Settle(USB_MCLK_FREQ / 1000, USB_MCLK_FREQ / 32768);   //Start the FLL, at the freq indicated by the config
                                                                        //constant USB_MCLK_FREQ
        XT2_Start(XT2DRIVE_0);                                          //Start the "USB crystal"
    }
	else {
		#if defined (__MSP430F552x) || defined (__MSP430F550x)
			P5SEL |= 0x10;                                      //enable XT1 pins
		#endif
        //Use the REFO oscillator to source the FLL and ACLK
        UCSCTL3 = SELREF__REFOCLK;
        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);

        //MCLK will be driven by the FLL (not by XT2), referenced to the REFO
        Init_FLL_Settle(USB_MCLK_FREQ / 1000, USB_MCLK_FREQ / 32768);   //set FLL (DCOCLK)

        XT1_Start(XT1DRIVE_0);                                          //Start the "USB crystal"
    }
}


/*
 * ======== Init_Ports ========
 */
VOID Init_Ports (VOID)
{
    //Initialization of ports (all unused pins as outputs with low-level
    P1OUT = 0x00;
    P1DIR = 0xFF;
	P2OUT = 0x00;
    P2DIR = 0xFF;
    P3OUT = 0x00;
    P3DIR = 0xFF;
    P4OUT = 0x00;
    P4DIR = 0xFF;
    P5OUT = 0x00;
    P5DIR = 0xFF;
    P6OUT = 0x00;
    P6DIR = 0xFF;
	P7OUT = 0x00;
    P7DIR = 0xFF;
    P8OUT = 0x00;
    P8DIR = 0xFF;
    #if defined (__MSP430F563x_F663x)
		P9OUT = 0x00;
		P9DIR = 0xFF;
    #endif

}


