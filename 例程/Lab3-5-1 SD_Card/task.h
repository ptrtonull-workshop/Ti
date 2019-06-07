/*
 * task.h
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */

#ifndef TASK_H_
#define TASK_H_
#include <string.h>

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_API/USB_Common/usb.h"

#include "F5xx_F6xx_Core_Lib/HAL_UCS.h"
#include "F5xx_F6xx_Core_Lib/HAL_PMM.h"
#include "F5xx_F6xx_Core_Lib/HAL_FLASH.h"

//MSC #includes
#include "USB_API/USB_MSC_API/UsbMscScsi.h"
#include "USB_API/USB_MSC_API/UsbMsc.h"
#include "USB_API/USB_MSC_API/UsbMscStateMachine.h"

//Board and FatFs #includes
#include "FatFs/diskio.h"
#include "FatFs/mmc.h"


extern BYTE checkInsertionRemoval (void);
extern VOID Init_Clock (VOID);
extern VOID Init_Ports (VOID);

extern struct USBMSC_mediaInfoStr mediaInfo;

#endif /* TASK_H_ */
