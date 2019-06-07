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
uint8_t bDetectCard = 0x00;                                 //ȫ���жϱ�־
/*  
 * ======== main ========
 */
void main (void)
{
    WDTCTL = WDTPW + WDTHOLD;                                           //�ؿ��Ź�
    Init_Ports();                                               		//IO��ʼ��
    SetVCore(3);
    Init_Clock();                                              			//ʱ�ӳ�ʼ��
    disk_initialize(0);                                                 //SK����ʼ��.
    USB_init();                                                         //USB��ʼ��
    USB_setEnabledEvents(kUSB_allUsbEvents);                            //ʹ��USB�¼�
    RWbuf_info = USBMSC_fetchInfoStruct();
    if (detectCard()){													//SD��������
        mediaInfo.mediaPresent = kUSBMSC_MEDIA_PRESENT;
    } 
	else {  
	    mediaInfo.mediaPresent = kUSBMSC_MEDIA_NOT_PRESENT;
	}
    mediaInfo.mediaChanged = 0x00;
    mediaInfo.writeProtected = 0x00;
    disk_ioctl(0,GET_SECTOR_COUNT,&mediaInfo.lastBlockLba);
    mediaInfo.bytesPerBlock = BYTES_PER_BLOCK;                          //��ߴ�
    USBMSC_updateMediaInfo(0, &mediaInfo);

    USBMSC_registerBufInfo(0, &RWbuf[0], NULL, sizeof(RWbuf));      	//���ݽ������ڴ����
    TA0CCTL0 = CCIE;                                                    //ʹ��TA0�ж�
    TA0CCR0 = 32768;                                                    //TA0ʱ��ѡ��
    TA0CTL = TASSEL_1 + MC_1 + TACLR;                                   //ACLK = 32kHz, ��ģʽ,���TAR
    if (USB_connectionInfo() & kUSB_vbusPresent){						//���USB�Ѿ����ӵ���USB���߲�������
        if (USB_enable() == kUSB_succeed){								//����ʹ��USB
            USB_reset();												//USB��λ
            USB_connect();												//��������USB
        }
    }
    __enable_interrupt();                                               //���ж�
    while (1)
    {
        switch (USB_connectionState())									//USB����״̬
        {
            case ST_USB_DISCONNECTED:									//û������
                __bis_SR_register(LPM3_bits + GIE);         			//�͹���3
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
                while (RWbuf_info->operation == kUSBMSC_READ)			//������
                {

                    DRESULT dresult = disk_read(0,          			//����������0
                        RWbuf_info->bufferAddr,             			//ָ��ָ��ʹ�õĵ�ַ
                        RWbuf_info->lba,                    			//��һ�� LBA����
                        RWbuf_info->lbCount);               			//�������
                    switch (dresult)
                    {
                        case RES_OK:
                            RWbuf_info->returnCode = kUSBMSC_RWSuccess;
                            break;
                        case RES_ERROR:                     			//�ļ���ת��
                            if (!checkInsertionRemoval()){  			//�˶�SD��״̬
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
                    DRESULT dresult = disk_write(0,         			//ͬ��
                        RWbuf_info->bufferAddr,            				//ͬ��
                        RWbuf_info->lba,                    			//ͬ��
                        RWbuf_info->lbCount);               			//ͬ��
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
                if (bDetectCard){										//����жϱ�־
                    checkInsertionRemoval();							//�˶Կ�״̬
                    bDetectCard = 0x00;                                 //����жϱ�־
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
    bDetectCard = 0x01;                                                 //��⿨��־
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
            UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG);				 //��� OSCĬ�ϱ�־
            SFRIFG1 &= ~OFIFG;                          			 //���OFIFGĬ�ϱ�־
            break;
        case SYSUNIV_ACCVIFG:
            __no_operation();
            break;
        case SYSUNIV_BUSIFG:										//������ߴ���
            SYSBERRIV = 0;                              			//������߱�־
            USB_disable();                              			//��ֹUSB
    }
}

