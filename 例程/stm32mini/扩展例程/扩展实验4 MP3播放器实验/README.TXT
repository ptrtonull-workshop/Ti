实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习VS1053的使用,实现一个简单的MP3.

硬件资源:	
	1,DS0(连接在PA8)
	2,串口1(波特率:9600,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过GPIO驱动,连接关系见lcd.h) 	 
	4,W25Q64(SPI FLASH芯片,连接在SPI1上)
	5,按键KEY0(PC5)/KEY1(PA15)/KEY_UP(PA0,也称之为WK_UP)  
	5,SD卡,通过SPI1连接 
	6,ATK-VS1053模块与STM32芯片连接关系为(请按如下接线连接Mini板和MP3模块): 
	  ATK-VS1053模块----------STM32芯片
		 VS_SCK    -----------   PA5
		 VS_MISO   ------------  PA6
		 VS_MOSI   ------------  PA7
		 VS_RST    ------------  PA11
		 VS_XDCS   -----------   PA4
		 VS_XCS    -----------   PA8
		 VS_DREQ   ------------  PA12
		   5V      ------------  5V 
		   GND     ------------  GND
		   
实验现象:
	本实验开机先检测字库是否存在，如果检测无问题，则对VS1053进行RAM测试和正弦测试，测试完后开始循环播
	放SD卡MUSIC文件夹里面的歌曲（必须在SD卡根目录建立一个MUSIC文件夹，并存放歌曲在里面），在TFTLCD上
	显示歌曲名字、播放时间、歌曲总时间、歌曲总数目、当前歌曲的编号等信息。KEY0用于选择下一曲，KEY1用于
	选择上一曲,WK_UP暂停.本实验用DS0指示程序运行状况。 
	本例程可以通过USMART来调用VS_Set_Vol/VS_Set_Bass/VS_Set_Effect等函数,实现对VS1053的配置.
	
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源供电(5V电源，接VOUT2即可).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,LCD模块在开发板的LCD接口上面，必须靠右插
	4,不要把1820和PA0的跳线帽跳上,否则可能导致按键"不灵". 
	5,该实验必须自备SD卡一个,并在SD卡根目录新建MUSIC文件夹，存放一些音乐文件(mp3/wav/wma/flac/midi/ogg)
	到该文件夹下面。
	6,本例程需要自备ATK-VS1053模块一个，并按照硬件资源所示方法连接ATK-VS1053模块和MiniSTM32开发板.
	7,需自备耳机一副,插入PHONE端口才可以欣赏歌曲.
	8,如果要更新字库，请拷贝开发板光盘-->5，SD卡根目录文件 文件夹里面的SYSTEM文件夹到SD卡根目录。
	9,本例程的详细说明，见光盘：9，增值资料\1，ALIENTEK 产品资料\6，ATK-VS1053 MP3模块\3，模块使用说明\
	ATK-VS1053 MP3模块使用说明(Mini V3)_AN1401.pdf
	

					正点原子@ALIENTEK
					2014-3-25
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com