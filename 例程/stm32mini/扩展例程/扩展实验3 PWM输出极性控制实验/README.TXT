实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习OCRxREF和CCxP对PWM波形的影响
	
硬件资源:
	1,DS0(连接在PA8) 
	2,定时器1(TIM1),使用TIM1的通道1(CH1),开启部分重映射,将TIM1_CH1输出到PA8.
	从而实现PWM输出控制DS0亮度.
	
实验现象:
	本实验在标准例程实验8的基础上修改而来，验证了OCRxREF的中间作用，同时检测了CCxP对有效
	电平极性的控制。次试验不需要在开发板上做，直接软件仿真即可观察结果。您可以在波形窗口新
	建一个PORTA.8的信号，然后执行一段时间，就会看到结果了。

注意事项: 
	1,详细情况见此贴：http://www.openedv.com/posts/list/140.htm 
	

					正点原子@ALIENTEK
					2014-3-25
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com