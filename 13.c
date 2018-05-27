#include "LPC11xx.h" //头文件
#define LED				(1ul<<0)
#define LEDOFF()	LPC_GPIO2->DATA|=LED//熄灭LED
#define LEDON()		LPC_GPIO2->DATA&=~LED//点亮LED
/*
*FunctionName		:LedInit()
*Description		:LED初始化函数
*EntryParameter	:None
*ReturnValue		:None
*/
void LedInit()
{
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<16);
	LPC_IOCON->PIO2_0&=0x07;
	LPC_IOCON->PIO2_0|=0x00;//把p2.0脚设为gpio
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);//禁能IOCON时钟
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<6);//使能GPIO时钟
	LPC_GPIO2->DIR |=LED;//把p2.0设为输出引脚
	LPC_GPIO2->DATA |=LED;//把p2.0设置为高电平
}
/*
*FunctionName		:TIMER32_0_IRQHandler()
*Description		:32位定时计数器0中断服务函数
*EntryParameter	:None
*ReturnValue		:None
*/
void TIMER32_0_IRQHandler(void)
{
	LPC_TMR32B0->IR=0x01;//向匹配通道0写1清除中断
	if(LPC_GPIO2->DATA&LED)
	{
		LEDON();
	}
	else
	{
		LEDOFF();
	}
}
/*
*FunctionName		:T32B1_Init()
*Description		:32位定时计数器0初始化函数
*EntryParameter	:None
*ReturnValue		:None
*/
void Timer0Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<9);//使能TIM32B0时钟
	LPC_TMR32B0->IR=0x01;//每一个PCLK,TC加1
	LPC_TMR32B0->MCR=0x03;
	LPC_TMR32B0->MR0=SystemCoreClock;
	LPC_TMR32B0->TCR=0x01;
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
}
/*
*FunctionName		:main()
*Description		:主函数
*EntryParameter	:None
*ReturnValue		:None
*/
int main(void)
{
	LedInit();
	Timer0Init();
	while(1);
}