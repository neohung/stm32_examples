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

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

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

volatile osThreadId thread2_id = NULL;
static void Thread2(void const *arg)
{
	  while(1)
	 {
	   GPIO_SetBits(GPIOD, GPIO_Pin_13);
	   osDelay(250);
	   printf("A");
	   GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	   osDelay(250);
	 }

}

volatile osThreadId thread3_id = NULL;
static void Thread3(void const *arg)
{
	  while(1)
	 {
		  GPIO_SetBits(GPIOD, GPIO_Pin_12);
		  osDelay(500);
		  //printf("B");
		  GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		  osDelay(500);
	 }

}

volatile osThreadId main_thread_id = NULL;
static void mainThread(void const *arg)
{
	// ------------- USB -------------- //
	USBD_Init(&USB_OTG_dev,
			  USB_OTG_FS_CORE_ID,
			  &USR_desc,
			  &USBD_CDC_cb,
			  &USR_cb);
	 osThreadDef(USER_Thread2, Thread2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	 thread2_id = osThreadCreate(osThread(USER_Thread2), NULL);
	 osThreadDef(USER_Thread3, Thread3, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	 thread3_id = osThreadCreate(osThread(USER_Thread3), NULL);
	 vTaskDelete( NULL );
}

int main(void) {
	init();

	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	setbuf(stdout, NULL);
    osThreadDef(USER_Thread1, mainThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    main_thread_id = osThreadCreate(osThread(USER_Thread1), NULL);
    osKernelStart();
	while(1){};
	return 0;
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {

}

