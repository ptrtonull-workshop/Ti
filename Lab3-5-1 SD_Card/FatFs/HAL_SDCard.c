/*******************************************************************************
 *
 *  HAL_SDCard.c - Driver for the SD Card slot
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/***************************************************************************//**
 * @file       HAL_SDCard.c
 * @addtogroup HAL_SDCard
 * @{
 ******************************************************************************/
#include "msp430.h"
#include "HAL_SDCard.h"

// Pins from MSP430 connected to the SD Card
#define SPI_SIMO        BIT1
#define SPI_SOMI        BIT2
#define SPI_CLK         BIT3
#define SD_CS           BIT0

// Ports
#define SPI_SEL         P4SEL
#define SPI_DIR         P4DIR
#define SPI_OUT         P4OUT
#define SPI_REN         P4REN
#define SD_CS_SEL       P4SEL
#define SD_CS_OUT       P4OUT
#define SD_CS_DIR       P4DIR
  #define MCLK_FREQ 20000000
/***************************************************************************//**
 * @brief   Initialize SD Card
 * @param   None
 * @return  None
 ******************************************************************************/
//****************дһ�ֽں���***************************
unsigned char  SPI_WriteByte(unsigned char val)
{
 	while(!(UCB1IFG & UCTXIFG)); //�ȴ����ͻ�������
 	UCB1IFG&=~UCTXIFG;
 	UCB1TXBUF = val; //���͵�λ
 	while(UCB1STAT & UCBBUSY); //�ȴ����һλʵ���ͳ�
 	__delay_cycles(MCLK_FREQ / 1000*3);
	return(UCB1RXBUF);

}
//*****************���������****************************
unsigned char SD_SendCommand(unsigned char cmd,unsigned long arg)
{
 unsigned char r1;
 unsigned char retry1=0;      //�ظ���������
 SPI_WriteByte(cmd|0x40);   //�ֱ�д������
 SPI_WriteByte(arg>>24);      //���ݶε�4�ֽ�
 SPI_WriteByte(arg>>16);      //���ݶε�3�ֽ�
 SPI_WriteByte(arg>>8);       //���ݶε�2�ֽ�
 SPI_WriteByte(arg);          //���ݶε�1�ֽ�
 SPI_WriteByte(0x95);         //CRCЧ���
 while((r1=SPI_WriteByte(0xff))==0xff)
	 if(retry1++>200)break;                     //���Ƭѡ�ź�
 return r1;                   //����״ֵ̬
}
unsigned char SD_SendCommand1(unsigned char cmd,unsigned long arg)
{
 unsigned char r1;
 unsigned char retry1=0;      //�ظ���������
 SPI_WriteByte(cmd|0x40);   //�ֱ�д������
 SPI_WriteByte(arg>>24);      //���ݶε�4�ֽ�
 SPI_WriteByte(arg>>16);      //���ݶε�3�ֽ�
 SPI_WriteByte(arg>>8);       //���ݶε�2�ֽ�
 SPI_WriteByte(arg);          //���ݶε�1�ֽ�
 SPI_WriteByte(0xff);         //CRCЧ���
 while((r1=SPI_WriteByte(0xff))==0xff)
	 if(retry1++>200)break;                     //���Ƭѡ�ź�
 return r1;                   //����״ֵ̬
}
unsigned char sd_reset()
{
	unsigned char i;
	unsigned char retry;            		//�ظ�����
	unsigned char r1=0;
	retry=0;
	do
	{
		SD_CS_OUT |= SD_CS;				//Ƭѡ�ź�CS�ø�
		for(i=0;i<20;i++)					//����74+ʱ���ź�
		{
		   SPI_WriteByte(0xff);
		}
		SD_CS_OUT &= ~SD_CS;			//Ƭѡ�ź�CS����
		r1 = SD_SendCommand(0,0);	//��CMD0�������ȡӦ������Ӧ��Ϊ0x01
		retry++;
		if(retry>200) return 1;   //��ʱ�˳�
	} while(r1 != 0x01); //�ȴ�IDLE�����
	retry = 0;
	while(1)
	{
		SD_CS_OUT |= SD_CS;					//Ƭѡ�ź�CS�ø�
		SPI_WriteByte(0xff);				//ʱ�Ӳ���
		SD_CS_OUT &= ~SD_CS;				//Ƭѡ�ź�CS����
		r1 = SD_SendCommand1(1, 0);         //��CMD1�������ȡӦ����Ӧ0x00����SPIģʽ
		retry++;
		if(retry>200) return 1;
		if(r1==0x00) break;
	}
	SD_CS_OUT |= SD_CS;						//Ƭѡ�ź�CS�ø�
	SPI_WriteByte(0xff);					//ʱ�Ӳ���
 return 0;                              //��������
}

unsigned char SDCard_init(void)
{
	unsigned char SUCFlag;
	SPI_DIR |= SPI_CLK + SPI_SIMO;						   // CLK,SIMO��������Ϊ�����
	SPI_REN |= SPI_SOMI;                                   // ʹ��SD��SOMI������������
	SPI_OUT |= SPI_SOMI;                                   // SOMI��������ߵ�ƽ
	SPI_SEL |= SPI_CLK + SPI_SOMI + SPI_SIMO;			   // CLK,SOMI,SIMO����Ϊ���蹦��
	SD_CS_SEL &= ~SD_CS;								   // CS������Ϊ��ͨIO��
	SPI_REN |=SD_CS;									   // ʹ��CS���ŵ���������
	SD_CS_DIR |= SD_CS;									   // CS��������Ϊ�������
	SD_CS_OUT |= SD_CS;									   // CS��������ߵ�ƽ
	// ��ʼ�� USCI_B1 Ϊ SPI ����ģʽ
	UCB1CTL1 |= UCSWRST;                                   //����USCI ״̬
	UCB1CTL0 = UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC; // ����, 8λ SPI ����ģʽ
	// ʱ�Ӽ���ѡ�� - �ߵ�ƽΪ����״̬
	//��λ��ǰ����λ�ں�
	UCB1CTL1 = UCSWRST + UCSSEL_2;                         // ʹ��SMCLK
	UCB1BR0 = 200;                                          // ��ʼ��SD��SPI���� <400kHz
	UCB1BR1 = 1;                                           // f_UCxCLK = 20MHz/100 = 200kHz
	UCB1CTL1 &= ~UCSWRST;                                  // �ͷ� USCI ״̬
	UCB1IFG &= ~UCRXIFG;                                   // ������ݵ�ĩβ�����������ݣ������ط�������
	SUCFlag=sd_reset();
	return SUCFlag;
}

/***************************************************************************//**
 * @brief   Enable fast SD Card SPI transfers. This function is typically
 *          called after the initial SD Card setup is done to maximize
 *          transfer speed.
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_fastMode(void)
{
    UCB1CTL1 |= UCSWRST;                                   // Put state machine in reset
    UCB1BR0 = 2;                                           // f_UCxCLK = 20MHz/2 = 10.0MHz
    UCB1BR1 = 0;
    UCB1CTL1 &= ~UCSWRST;                                  // Release USCI state machine
}

/***************************************************************************//**
 * @brief   Read a frame of bytes via SPI
 * @param   pBuffer Place to store the received bytes
 * @param   size Indicator of how many bytes to receive
 * @return  None
 ******************************************************************************/

void SDCard_readFrame(uint8_t *pBuffer, uint16_t size)
{
    uint16_t gie = __get_SR_register() & GIE;              // Store current GIE state

    __disable_interrupt();                                 // Make this operation atomic

    UCB1IFG &= ~UCRXIFG;                                   // Ensure RXIFG is clear

    // Clock the actual data transfer and receive the bytes
    while (size--){
        while (!(UCB1IFG & UCTXIFG)) ;                     // Wait while not ready for TX
        UCB1TXBUF = 0xff;                                  // Write dummy byte
        while (!(UCB1IFG & UCRXIFG)) ;                     // Wait for RX buffer (full)
        *pBuffer++ = UCB1RXBUF;
    }

    __bis_SR_register(gie);                                // Restore original GIE state
}

/***************************************************************************//**
 * @brief   Send a frame of bytes via SPI
 * @param   pBuffer Place that holds the bytes to send
 * @param   size Indicator of how many bytes to send
 * @return  None
 ******************************************************************************/

void SDCard_sendFrame(uint8_t *pBuffer, uint16_t size)
{
    uint16_t gie = __get_SR_register() & GIE;              // Store current GIE state

    __disable_interrupt();                                 // Make this operation atomic

    // Clock the actual data transfer and send the bytes. Note that we
    // intentionally not read out the receive buffer during frame transmission
    // in order to optimize transfer speed, however we need to take care of the
    // resulting overrun condition.
    while (size--){
        while (!(UCB1IFG & UCTXIFG)) ;                     // Wait while not ready for TX
        UCB1TXBUF = *pBuffer++;                            // Write byte
    }
    while (UCB1STAT & UCBUSY) ;                            // Wait for all TX/RX to finish

    UCB1RXBUF;                                             // Dummy read to empty RX buffer
                                                           // and clear any overrun conditions

    __bis_SR_register(gie);                                // Restore original GIE state
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to high
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_setCSHigh(void)
{
    SD_CS_OUT |= SD_CS;
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to low
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_setCSLow(void)
{
    SD_CS_OUT &= ~SD_CS;
}


/***************************************************************************//**
 * @}
 ******************************************************************************/
