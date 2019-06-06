#ifndef __DR_TFT_H_
#define __DR_TFT_H_

#include <stdint.h>

#ifndef MCLK_FREQ
  #define MCLK_FREQ 20000000
#endif

#ifndef SMCLK_FREQ
  #define SMCLK_FREQ 20000000
#endif

#define SPI_FREQ 10000000

#define TFT_XSIZE 240
#define TFT_YSIZE 320

#define TFTREG_RAM_XADDR  0x0201
#define TFTREG_RAM_YADDR  0x0200
#define TFTREG_RAM_ACCESS 0x0202

#define TFTREG_SOFT_RESET 0x0003

#define TFTREG_WIN_MINX   0x0212
#define TFTREG_WIN_MAXX   0x0213
#define TFTREG_WIN_MINY   0x0210
#define TFTREG_WIN_MAXY   0x0211

#define TRUE 	0xFF
#define FALSE	0x00
#define OK		0xFF
#define ERROR	0x00
//#define NULL	0x00

#define		RED			0xf800
#define		GREEN		0x07e0
#define		BLUE		0x001f
#define		PURPLE		0xf81f
#define		YELLOW		0xffe0
#define		CYAN		0x07ff 		//����ɫ
#define		ORANGE		0xfc08
#define		BLACK		0x0000
#define		WHITE		0xffff

#define uint8 	unsigned char
#define uint16 	unsigned short int
/* TFT���ײ�ӿ� */

//��ʼ��TFT
void initTFT();

//��TFT������һ����ַ�������Ƿ��ͳɹ�
void tft_draw_circle(char x,char y,char r,unsigned int color_dat);
void tft_draw_line(char xs,char ys,char xe,char ye,unsigned int color_dat);
void tft_draw_part(char xs,char ys,char xe,char ye,unsigned int color_dat);
void tft_draw_rectangle(char xs,char ys,char xe,char ye,unsigned int color_dat);
void tft_prints16(char x,char y,char *s_dat,unsigned int word_color,unsigned int back_color);
void tft_DisplayFul(unsigned int color);
void tft_printc6(char x, char y, char c_dat,unsigned int word_color,unsigned int back_color);
void tft_prints6(char x, char y, char *s_dat,unsigned int word_color,unsigned int back_color);
void tft_printc(char x, char y, char c_dat,unsigned int word_color,unsigned int back_color);
void tft_prints(char x, char y, char *s_dat,unsigned int word_color,unsigned int back_color);
void tft_draw_dot(char x,char y,unsigned int color_dat);
/* TFT���߲�ӿ� */
/* ���и߲�ӿ�����X��Y�Ե������ӿڴ�XΪ��YΪ�� */
//void tft_prints16(char x,char y,char *s_dat,unsigned int word_color,unsigned int back_color);
//��0~255��ʾ��RGB��ɫת��ΪTFT��Ļʹ�õ���ɫ

//��һ��������Ϊĳ����ɫ
//void etft_AreaSet(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint16_t color);

//��ָ����λ����ʾһ���ַ���
//void etft_DisplayString(const char* str, uint16_t sx, uint16_t sy, uint16_t fRGB, uint16_t bRGB);

//��ָ����λ����ʾһ��ͼƬ��image��24λλͼ��������ʾ
//������˳������ҡ����µ���(����˳��ת)��ÿ3�ֽ�һ�����أ�˳��ΪB��G��R��ÿ���ֽ�����0������4��������
//�Գ���24λλͼ����0x36���Ƶ��ļ�ĩβ����
//void etft_DisplayImage(const uint8_t* image, uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);

#endif
