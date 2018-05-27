#include "LPC11xx.h" //ͷ�ļ�
#define LED				(1ul<<0)
#define LEDOFF()	LPC_GPIO2->DATA|=LED//Ϩ��LED
#define LEDON()		LPC_GPIO2->DATA&=~LED//����LED
/*
*FunctionName		:LedInit()
*Description		:LED��ʼ������
*EntryParameter	:None
*ReturnValue		:None
*/
void LedInit()
{
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<16);
	LPC_IOCON->PIO2_0&=0x07;
	LPC_IOCON->PIO2_0|=0x00;//��p2.0����Ϊgpio
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);//����IOCONʱ��
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<6);//ʹ��GPIOʱ��
	LPC_GPIO2->DIR |=LED;//��p2.0��Ϊ�������
	LPC_GPIO2->DATA |=LED;//��p2.0����Ϊ�ߵ�ƽ
}
/*
*FunctionName		:TIMER32_0_IRQHandler()
*Description		:32λ��ʱ������0�жϷ�����
*EntryParameter	:None
*ReturnValue		:None
*/
void TIMER32_0_IRQHandler(void)
{
	LPC_TMR32B0->IR=0x01;//��ƥ��ͨ��0д1����ж�
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
*Description		:32λ��ʱ������0��ʼ������
*EntryParameter	:None
*ReturnValue		:None
*/
void Timer0Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<9);//ʹ��TIM32B0ʱ��
	LPC_TMR32B0->IR=0x01;//ÿһ��PCLK,TC��1
	LPC_TMR32B0->MCR=0x03;
	LPC_TMR32B0->MR0=SystemCoreClock;
	LPC_TMR32B0->TCR=0x01;
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
}
/*
*FunctionName		:main()
*Description		:������
*EntryParameter	:None
*ReturnValue		:None
*/
int main(void)
{
	LedInit();
	Timer0Init();
	while(1);
}