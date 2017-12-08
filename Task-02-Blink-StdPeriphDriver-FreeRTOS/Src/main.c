#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "main.h"

#include "cmsis_os.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

// Private variables
volatile uint32_t time_var1, time_var2;

// Private function prototypes
void Delay(volatile uint32_t nCount);

void init() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}

	// ---------- GPIO -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

volatile osThreadId thread1_id = NULL;
static void Thread1(void const *arg)
{
	  while(1)
	 {
	   GPIO_SetBits(GPIOD, GPIO_Pin_12);
	   osDelay(500);
	   GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	   osDelay(500);
	 }

}

volatile osThreadId thread2_id = NULL;
static void Thread2(void const *arg)
{
	  while(1)
	 {
	   GPIO_SetBits(GPIOD, GPIO_Pin_13);
	   osDelay(250);
	   GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	   osDelay(250);
	 }

}


int main(void) {
	init();

	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	setbuf(stdout, NULL);
/*
	for(;;) {
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(500);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		Delay(500);
	}
*/

  osThreadDef(USER_Thread1, Thread1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  thread1_id = osThreadCreate(osThread(USER_Thread1), NULL);
  osThreadDef(USER_Thread2, Thread2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  thread2_id = osThreadCreate(osThread(USER_Thread2), NULL);
  
	osKernelStart();
	while(1){};
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

