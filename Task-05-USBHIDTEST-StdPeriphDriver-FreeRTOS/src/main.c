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

#include "usb_core.h" //in USB_OTG
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

	//-----------USER BTN (PA0)---------
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // Clock for SYSCFG
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    // GPIOA initialization as an input from user button (GPIOA0)
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    // Selects the GPIOA pin 0 used as external interrupt source
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    // External interrupt settings
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStruct);
    // Nested vectored interrupt settings
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    // EXTI0_IRQn has Most important interrupt
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_Init(&NVIC_InitStruct);
}

#include "hid_report_desc.h"
extern uint8_t USBD_HID_SendReport     (USB_OTG_CORE_HANDLE  *pdev, uint8_t *report,uint16_t len);
struct mediaHID_t mediaHID;
struct keyboardHID_t keyboardHID;

volatile int is_user_button_press = 0;
void delay(int i)
{
	i*=1000;
	do{
	}while((i--)>0);
}

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0))
	{

		is_user_button_press = 1;
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		 EXTI_ClearITPendingBit(EXTI_Line0);
	}

}

volatile osThreadId thread2_id = NULL;
static void Thread2(void const *arg)
{
	  while(1)
	 {
	   //GPIO_SetBits(GPIOD, GPIO_Pin_13);
	   //osDelay(250);
	   //printf("Thread2\r\n");
		//
		if (is_user_button_press){
		  mediaHID.id = 2;
		  mediaHID.keys = 0;
		  keyboardHID.id = 1;
		  keyboardHID.modifiers = 0;
		  keyboardHID.key1 = 0;
		  keyboardHID.key2 = 0;
		  keyboardHID.key3 = 0;

		  keyboardHID.modifiers = USB_HID_MODIFIER_LEFT_GUI;
		  keyboardHID.key1 = USB_HID_KEY_R;
		  USBD_HID_SendReport(&USB_OTG_dev, &keyboardHID, sizeof(struct keyboardHID_t));
		  osDelay(30);
		  //HAL_Delay(30);
		  keyboardHID.modifiers = 0;
		  keyboardHID.key1 = 0;
		  USBD_HID_SendReport(&USB_OTG_dev, &keyboardHID, sizeof(struct keyboardHID_t));
		  is_user_button_press = 0;

		  GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		}
		 osDelay(20);
	  // GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	  // osDelay(250);
	 }
}

volatile osThreadId thread3_id = NULL;
extern __IO uint32_t uwTick;
static void Thread3(void const *arg)
{
     printf("test\r\n");
	  while(1)
	 {
		  GPIO_SetBits(GPIOD, GPIO_Pin_12);
		  osDelay(500);
		  //delay(5);
		  //printf("B");
		  GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		  //delay(5);
		  osDelay(500);
	 }

}


extern  USBD_Usr_cb_TypeDef USR_cb;
extern  USBD_DEVICE USR_desc;
extern USBD_Class_cb_TypeDef  USBD_neo_cb;
extern void USBD_Init(USB_OTG_CORE_HANDLE *pdev, USB_OTG_CORE_ID_TypeDef coreID, USBD_DEVICE *pDevice, USBD_Class_cb_TypeDef *class_cb, USBD_Usr_cb_TypeDef *usr_cb);
volatile osThreadId main_thread_id = NULL;
static void mainThread(void const *arg)
{
	// ------------- USB -------------- //
	USBD_Init(&USB_OTG_dev,
			  USB_OTG_FS_CORE_ID,       //1 means use FS, 0 use HS
			  &USR_desc,
			  &USBD_neo_cb,
			  &USR_cb);
	 osThreadDef(USER_Thread2, Thread2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	 thread2_id = osThreadCreate(osThread(USER_Thread2), NULL);
	 osThreadDef(USER_Thread3, Thread3, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	 thread3_id = osThreadCreate(osThread(USER_Thread3), NULL);
	 vTaskDelete( NULL );
}



int main(void) {
	init();
    //printf("init\r\n");
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

