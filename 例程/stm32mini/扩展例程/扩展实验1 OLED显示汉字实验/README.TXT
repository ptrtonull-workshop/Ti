实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习单色OLED显示屏显示汉字的方法
	
硬件资源:
	1,DS0(连接在PA8) 
	2,ALIENTEK 0.96寸OLED模块(连接IO说明,见oled.h文件) 
	
实验现象:
	本实验在标准例程OLED显示实验的基础上修改,新增汉字显示部分,实现OLED汉字显示,本例程下载
	后,OLED将显示ALIENTEK MiniSTM32 开发板等字样. 同时DS0闪烁,提示程序正在运行. 
	
注意事项:
	1,开发板的OLED和LCD共用一个接口，插OLED模块的时候，靠左插即可.
	2,例程默认用的8080方式驱动OLED模块,所以OLED模块的BS0,BS1都需要接VCC(默认接的VCC).
	3,本例程汉字库的取模方式同标准例程OLED显示实验的ASCII取模方式完全一样,汉字大小为24*24。 
	

               	正点原子@ALIENTEK
               	2014-3-25
                广州市星翼电子科技有限公司
                电话：020-38271790
                传真：020-36773971
                购买：http://shop62103354.taobao.com
                      http://shop62057469.taobao.com
                公司网站：www.alientek.com
                技术论坛：www.openedv.com