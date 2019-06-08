实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习ATK-NEO-6M GPS模块的使用，实现GPS定位

硬件资源:	
	1,DS0(连接在PA8)
	2,串口1(波特率:38400,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过GPIO驱动,连接关系见lcd.h) 	
	4,串口2(波特率:38400,PA2/PA3接GPS模块的RXD和TXD)
	5,按键KEY0(PC5) 
	6,ATK-NEO-6M GPS模块，同开发板的连接关系如下: 
	STM32开发板-->ATK-NEO-6M GPS模块
			PA2-->RXD
			PA3-->TXD 
			GND-->GND
		5V/3.3V-->VCC
			 
实验现象: 
	本实验将实现如下功能：通过串口2连接ATK-NEO-6M GPS模块，然后通过液晶显示GPS信息，包括精度、纬度、高
	度、速度、用于定位的卫星数、可见卫星数、UTC时间等信息。同时，可以通过USMART工具，设置GPS模块的刷新
	速率（最大支持5Hz刷新）和时钟脉冲的配置。另外，通过KEY0按键，可以开启或关闭NMEA数据的上传（即输出到
	串口1，方便开发调试）。
	
注意事项：
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源供电(5V电源，接VOUT2即可).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,LCD模块在开发板的LCD接口上面，必须靠右插
	4,不要把1820和PA0的跳线帽跳上,否则可能导致按键"不灵".  
	5,ATK-NEO-6M模块的波特率默认设置为38400。 
	6,串口1 的通信波特率也是38400。
	7,GPS模块必须放置于室外或者窗户旁边，否则有可能收不到GPS信号。
	8,如有其他问题，请参考<ATK-NEO-6M GPS模块问题汇总.pdf>。
	9,本例程详细说明，见光盘：9，增值资料\1，ALIENTEK 产品资料\2，ATK-NEO-6M GPS模块\ATK-NEO-6M GPS模块使用说明(战舰V2&Mini V3)_AN1409.pdf
 
               	正点原子@ALIENTEK
               	2014-3-30
                广州市星翼电子科技有限公司
                电话：020-38271790
                传真：020-36773971
                购买：http://shop62103354.taobao.com
                      http://shop62057469.taobao.com
                公司网站：www.alientek.com
                技术论坛：www.openedv.com