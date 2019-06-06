#include <string.h>
#include "task.h"


const WORD BYTES_PER_BLOCK = 512;

struct config_struct USBMSC_config = {
    0x00,
    0x00,
    0x80,
    "TI",
    "MSC",
    "v3.0",
};
uint8_t RWbuf[512];
USBMSC_RWbuf_Info *RWbuf_info;
uint8_t bDetectCard = 0x00;                                 //全局中断标志
/*  
 * ======== main ========
 */
void main (void)
{
    WDTCTL = WDTPW + WDTHOLD;                                           //关看门狗
    Init_Ports();                                               		//IO初始化
    SetVCore(3);
    Init_Clock();                                              			//时钟初始化
    disk_initialize(0);                                                 //SK卡初始化.
    USB_init();                                                         //USB初始化
    USB_setEnabledEvents(kUSB_allUsbEvents);                            //使能USB事件
    RWbuf_info = USBMSC_fetchInfoStruct();
    if (detectCard()){													//SD卡插入检测
        mediaInfo.mediaPresent = kUSBMSC_MEDIA_PRESENT;
    } 
	else {  
	    mediaInfo.mediaPresent = kUSBMSC_MEDIA_NOT_PRESENT;
	}
    mediaInfo.mediaChanged = 0x00;
    mediaInfo.writeProtected = 0x00;
    disk_ioctl(0,GET_SECTOR_COUNT,&mediaInfo.lastBlockLba);
    mediaInfo.bytesPerBlock = BYTES_PER_BLOCK;                          //块尺寸
    USBMSC_updateMediaInfo(0, &mediaInfo);

    USBMSC_registerBufInfo(0, &RWbuf[0], NULL, sizeof(RWbuf));      	//数据交换和内存管理
    TA0CCTL0 = CCIE;                                                    //使能TA0中断
    TA0CCR0 = 32768;                                                    //TA0时钟选择
    TA0CTL = TASSEL_1 + MC_1 + TACLR;                                   //ACLK = 32kHz, 升模式,清除TAR
    if (USB_connectionInfo() & kUSB_vbusPresent){						//如果USB已经连接但是USB总线不能握手
        if (USB_enable() == kUSB_succeed){								//重新使能USB
            USB_reset();												//USB复位
            USB_connect();												//重新连接USB
        }
    }
    __enable_interrupt();                                               //开中断
    while (1)
    {
        switch (USB_connectionState())									//USB连接状态
        {
            case ST_USB_DISCONNECTED:									//没有连接
                __bis_SR_register(LPM3_bits + GIE);         			//低功耗3
                _NOP();
                break;

            case ST_USB_CONNECTED_NO_ENUM:
                break;
            case ST_ENUM_ACTIVE:
                __disable_interrupt();
                if ((USBMSC_poll() == kUSBMSC_okToSleep) && (!bDetectCard)){
                    __bis_SR_register(LPM0_bits + GIE);
                }
                __enable_interrupt();
                while (RWbuf_info->operation == kUSBMSC_READ)			//读数据
                {

                    DRESULT dresult = disk_read(0,          			//屋里驱动号0
                        RWbuf_info->bufferAddr,             			//指针指向使用的地址
                        RWbuf_info->lba,                    			//第一个 LBA操作
                        RWbuf_info->lbCount);               			//操作快号
                    switch (dresult)
                    {
                        case RES_OK:
                            RWbuf_info->returnCode = kUSBMSC_RWSuccess;
                            break;
                        case RES_ERROR:                     			//文件被转移
                            if (!checkInsertionRemoval()){  			//核对SD卡状态
                                RWbuf_info->returnCode =
                                    kUSBMSC_RWMedNotPresent;
                            }
                            break;
                        case RES_NOTRDY:
                            RWbuf_info->returnCode = kUSBMSC_RWNotReady;
                            break;
                        case RES_PARERR:
                            RWbuf_info->returnCode = kUSBMSC_RWLbaOutOfRange;
                            break;
                    }
                    USBMSC_bufferProcessed();
                }
                while (RWbuf_info->operation == kUSBMSC_WRITE)
                {
                    DRESULT dresult = disk_write(0,         			//同上
                        RWbuf_info->bufferAddr,            				//同上
                        RWbuf_info->lba,                    			//同上
                        RWbuf_info->lbCount);               			//同上
                    switch (dresult)
                    {
                        case RES_OK:
                            RWbuf_info->returnCode = kUSBMSC_RWSuccess;
                            break;
                        case RES_ERROR:
                            if (!checkInsertionRemoval()){
                                RWbuf_info->returnCode =
                                    kUSBMSC_RWMedNotPresent;
                            }
                            break;
                        case RES_NOTRDY:
                            RWbuf_info->returnCode = kUSBMSC_RWNotReady;
                            break;
                        case RES_PARERR:
                            RWbuf_info->returnCode = kUSBMSC_RWLbaOutOfRange;
                            break;
                        default:
                            RWbuf_info->returnCode = kUSBMSC_RWNotReady;
                            break;
                    }
                    USBMSC_bufferProcessed();
                }
                if (bDetectCard){										//检查中断标志
                    checkInsertionRemoval();							//核对卡状态
                    bDetectCard = 0x00;                                 //清除中断标志
                }
                break;
            case ST_ENUM_SUSPENDED:
                __bis_SR_register(LPM3_bits + GIE);
                break;

            case ST_ENUM_IN_PROGRESS:
                break;

            case ST_NOENUM_SUSPENDED:
                __bis_SR_register(LPM3_bits + GIE);
                break;

            case ST_ERROR:
                _NOP();
                break;

            default:;
        }
    }
}


/*  
 * ======== TIMER0_A0_ISR ========
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    bDetectCard = 0x01;                                                 //检测卡标志
    __bic_SR_register_on_exit(LPM0_bits);
}

/*  
 * ======== UNMI_ISR ========
 */
#pragma vector = UNMI_VECTOR
__interrupt VOID UNMI_ISR (VOID)
{
    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG ))
    {
        case SYSUNIV_NONE:
            __no_operation();
            break;
        case SYSUNIV_NMIIFG:
            __no_operation();
            break;
        case SYSUNIV_OFIFG:
            UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG);				 //清除 OSC默认标志
            SFRIFG1 &= ~OFIFG;                          			 //清除OFIFG默认标志
            break;
        case SYSUNIV_ACCVIFG:
            __no_operation();
            break;
        case SYSUNIV_BUSIFG:										//如果总线错误
            SYSBERRIV = 0;                              			//清除总线标志
            USB_disable();                              			//禁止USB
    }
}

