#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "main.h"

// Private variables
volatile uint32_t time_var1, time_var2;

// Private function prototypes
void Delay(volatile uint32_t nCount);

void ledpwm() //PD12
{
	//Init LED GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &gpioStructure);
	//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_6;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioStructure);
	//
	//Init Timer TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	//timer_tick_frequency = 84000000 / (3359+1) = 25000
	timerInitStructure.TIM_Prescaler = 3359;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//Want PWM Period 4ms : PWM_frequency 250Hz = 25000 / (TIM_Period + 1)
	timerInitStructure.TIM_Period = 99;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timerInitStructure);
	TIM_Cmd(TIM4, ENABLE);
	//Set PWM TIM4_CH1 / OC1
	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	//4ms 50% duty cycle=2ms
	outputChannelInit.TIM_Pulse = 50;
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	//Assign GPIO_PinSource12
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

}

void tim2pwm() //PA0/PA1/PA2/PA3
{
	//Init GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioStructure);
	//
	//Init Timer TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	//timer_tick_frequency = 84000000 / (3359+1) = 25000
	timerInitStructure.TIM_Prescaler = 3359;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//Want PWM Period 4ms : PWM_frequency 250Hz = 25000 / (TIM_Period + 1)
	timerInitStructure.TIM_Period = 99;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	//Set PWM TIM2_CH1 / OC1
	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	//4ms 50% duty cycle=2ms
	outputChannelInit.TIM_Pulse = 50;
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH2 / OC2
	outputChannelInit.TIM_Pulse = 50;
	TIM_OC2Init(TIM2, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH3 / OC3
	outputChannelInit.TIM_Pulse = 50;
	TIM_OC3Init(TIM2, &outputChannelInit);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH4 / OC4
	outputChannelInit.TIM_Pulse = 50;
	TIM_OC4Init(TIM2, &outputChannelInit);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	//Assign Pin to TIM2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
}

void tim4pwm() //PB6/PB7/PB8/PB9
{
	//Init GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType =  GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//
	//Init Timer TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//timer_tick_frequency = 84000000 / (3359+1) = 25000
	TIM_TimeBaseInitStructure.TIM_Prescaler = 3359;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//Want PWM Period 4ms : PWM_frequency 250Hz = 25000 / (TIM_Period + 1)
	TIM_TimeBaseInitStructure.TIM_Period = 99;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM4, ENABLE);
	//Set PWM TIM2_CH1 / OC1
	TIM_OCInitTypeDef TIM_OCInitStructure = {0,};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//4ms 50% duty cycle=2ms
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH2 / OC2
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH3 / OC3
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH4 / OC4
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	//Assign Pin to TIM2
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
}


void tim5pwm() //PA0/PA1/PA2/PA3
{
	//Init GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioStructure);
	//
	//Init Timer TIM5
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	//timer_tick_frequency = 84000000 / (3359+1) = 25000
	timerInitStructure.TIM_Prescaler = 3359;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//Want PWM Period 4ms : PWM_frequency 250Hz = 25000 / (TIM_Period + 1)
	timerInitStructure.TIM_Period = 99;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &timerInitStructure);
	TIM_Cmd(TIM5, ENABLE);
	//Set PWM TIM2_CH1 / OC1
	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	//4ms 50% duty cycle=2ms
	outputChannelInit.TIM_Pulse = 50;
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM5, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH2 / OC2
	outputChannelInit.TIM_Pulse = 50;
	TIM_OC2Init(TIM5, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH3 / OC3
	outputChannelInit.TIM_Pulse = 50;
	TIM_OC3Init(TIM5, &outputChannelInit);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	////Set PWM TIM2_CH4 / OC4
	outputChannelInit.TIM_Pulse = 50;
	TIM_OC4Init(TIM5, &outputChannelInit);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
	//Assign Pin to TIM2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
}

void tim8pwm() //PC6/PC7PC8/PC9
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_Init(GPIOC,&GPIO_InitStructure);

	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);

	  //SystemCoreClock = 168000000;
	  ////timer_tick_frequency = 168000000 / ( Prescaler + 1) = 25000Hz
	  int Prescaler =  (SystemCoreClock / 25000) - 1;
	  // 4ms=250Hz => 25000Hz/250Hz = 100
	  int TimerPeriod =  100 ;
	  int  ccr1 = TimerPeriod / 2;
	  int  ccr2 = TimerPeriod / 2;
	  int  ccr3 = TimerPeriod / 2;
	  int  ccr4 = TimerPeriod / 2;

	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	    TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;
	    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	    TIM_TimeBaseInitStructure.TIM_Period = TimerPeriod - 1;
	    TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);

	    TIM_OCInitTypeDef TIM_OCInitStructure = {0,};
	    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	    TIM_OCInitStructure.TIM_Pulse = ccr1;
	    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	    TIM_OC1Init(TIM8,&TIM_OCInitStructure);

	    TIM_OCInitStructure.TIM_Pulse = ccr2;
	    TIM_OC2Init(TIM8,&TIM_OCInitStructure);

	    TIM_OCInitStructure.TIM_Pulse = ccr3;
	    TIM_OC3Init(TIM8,&TIM_OCInitStructure);

	    TIM_OCInitStructure.TIM_Pulse = ccr4;
	    TIM_OC4Init(TIM8,&TIM_OCInitStructure);

	    TIM_Cmd(TIM8,ENABLE);
	    TIM_CtrlPWMOutputs(TIM8,ENABLE);   //TIM1 and TIM8 need to exec this function
}
void tim1pwm() //PE9/PE10/PE13/PE14
{
		  GPIO_InitTypeDef GPIO_InitStructure;
		  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_14;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		  GPIO_Init(GPIOE,&GPIO_InitStructure);

		  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
		  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_TIM1);
		  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
		  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);

		  //SystemCoreClock = 168000000;
		  ////timer_tick_frequency = 168000000 / ( Prescaler + 1) = 25000Hz
		  int Prescaler =  (SystemCoreClock / 25000) - 1;
		  // 4ms=250Hz => 25000Hz/250Hz = 100
		  int TimerPeriod =  100 ;
		  int  ccr1 = TimerPeriod / 2;
		  int  ccr2 = TimerPeriod / 2;
		  int  ccr3 = TimerPeriod / 2;
		  int  ccr4 = TimerPeriod / 2;

		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
		    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

		    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		    TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;
		    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		    TIM_TimeBaseInitStructure.TIM_Period = TimerPeriod - 1;
		    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

		    TIM_OCInitTypeDef TIM_OCInitStructure = {0,};
		    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		    TIM_OCInitStructure.TIM_Pulse = ccr1;
		    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

		    TIM_OC1Init(TIM1,&TIM_OCInitStructure);

		    TIM_OCInitStructure.TIM_Pulse = ccr2;
		    TIM_OC2Init(TIM1,&TIM_OCInitStructure);

		    TIM_OCInitStructure.TIM_Pulse = ccr3;
		    TIM_OC3Init(TIM1,&TIM_OCInitStructure);

		    TIM_OCInitStructure.TIM_Pulse = ccr4;
		    TIM_OC4Init(TIM1,&TIM_OCInitStructure);

		    TIM_Cmd(TIM1,ENABLE);
		    TIM_CtrlPWMOutputs(TIM1,ENABLE);   //TIM1 and TIM8 need to exec this function
}


void init() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}
	tim8pwm();
	//tim1pwm();
	//tim4pwm();
	// ---------- GPIO -------- //
	// GPIOD Periph clock enable
	/*
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	 */
}

int main(void) {
	init();

	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	setbuf(stdout, NULL);

	for(;;) {
		/*
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(500);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		*/
		Delay(500);
	}

	return 0;
}

/*
 * Called from systick handler
 */
void timing_handler() {
	if (time_var1) {
		time_var1--;
	}

	time_var2++;
}

/*
 * Delay a number of systick cycles (1ms)
 */
void Delay(volatile uint32_t nCount) {
	time_var1 = nCount;
	while(time_var1){};
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {

}

