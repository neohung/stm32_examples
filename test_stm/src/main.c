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

#include "queue.h"

#include "ringbuf.h"
queue_t data_in;
//queue_t data_out;
queue_t commands;
ringbuf_t data_out;


__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

// Private function prototypes
void Delay(volatile uint32_t nCount);

void tim8pwm_init() //PC6/PC7PC8/PC9
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
	  //25000
	  int Prescaler =  (SystemCoreClock / 250) - 1;
	  // 4ms=250Hz => 25000Hz/250Hz = 100
	  // 250/100 = 2.5Hz , 1/2.5 = 400ms
	  int TimerPeriod =  100 ;
	  int  ccr1 = 0;//TimerPeriod / 2;
	  int  ccr2 = 0;//TimerPeriod / 2;
	  int  ccr3 = 0;//TimerPeriod / 2;
	  int  ccr4 = 0;//TimerPeriod / 2;

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
    /*
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
    */
    //
    //Motor_GPIO_Configuration();
    tim8pwm_init();
    //
}

#include "hid_report_desc.h"
extern uint8_t USBD_HID_SendReport(USB_OTG_CORE_HANDLE  *pdev, uint8_t *report,uint16_t len);
struct customerHID_t customerHID;
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
		//GPIO_SetBits(GPIOD, GPIO_Pin_14);
		 EXTI_ClearITPendingBit(EXTI_Line0);
	}

}
volatile osThreadId thread2_id = NULL;

static void Thread2(void const *arg)
{
	  while(1)
	 {
	   GPIO_SetBits(GPIOD, GPIO_Pin_13);
	   osDelay(250);
		if (is_user_button_press){
		 is_user_button_press = 0;
		  //GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		}
	  GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	   osDelay(250);
	 }
}

unsigned char polling_data(void)
{
	static queue_element_t *e;
	//unsigned int tail;
	while(1){
	    //tail =
	    getNextQueueData(&data_in, &e);
	    if (e) {
	    	break;
	    }else{
	    	osDelay(1);
	    }
	};
	return e->data[0];
}

volatile osThreadId thread3_id = NULL;
volatile osThreadId thread4_id = NULL;
volatile osThreadId thread5_id = NULL;
extern __IO uint32_t uwTick;


void send_message(char* buf,unsigned int len){
	unsigned int i;
	char crc = 0xFF;
	// Pushh Magic Number
	while (!pushElement(&data_out,0xEA)) {
		osDelay(1);
	}
	//}while(!pushQueueElement(&data_out,  elem));
	crc -= 0xEA;
	//
	for(i=0;i<len;i++){
		while (!pushElement(&data_out,buf[i])) {
				osDelay(1);
		}
		crc -= buf[i];
	}
	//push crc
	//printf("Push crc: 0x%X \n",crc);
	while (!pushElement(&data_out,crc)) {
					osDelay(1);
				}
	//osDelay(15);
}

static void process_data_out(void const *arg)
{
	 //queue_element_t *e = 0;
	 unsigned char* pdata;
	 //unsigned int tail;
	 while(1)
	 {
		 //printf("process_data_out\r\n");
		 //e = 0;
		 	while(1){
		 		//tail =
		 		getNextData(&data_out, &pdata);
		 	    if (pdata) {
		 	    	break;
		 	    }else{
		 	    	osDelay(1);
		 	    }
		 	};
		 	customerHID.data = *pdata;
		 	USBD_HID_SendReport(&USB_OTG_dev, (uint8_t*) &customerHID, sizeof(struct customerHID_t));
		 	osDelay(1);
	 }
}
//----------------------------------
typedef enum _OI_Opcode {
	// Command opcodes
	 OI_OPCODE_MOTORCONTROL  = 1,
	 OI_OPCODE_ODOMUPDATE = 2,
	 OI_OPCODE_QUERY = 3,
	 OI_OPCODE_IMUQUERY = 4,
	 OI_OPCODE_IMUSTATE = 5,
} OIOpcode;

//------------------------------------
static char str[12];

void oiProcessQuery(queue_element_t *e)
{
	printf("Process Odom Query\r\n");
	int x = 0x0001; //mm
	int y = 0x0002; //mm
	int theta = 0x0003; //degree 0~360
	int linear_speed = 0x0004; //mm
	int angular_speed = 0x0005; //mm
	str[0] = OI_OPCODE_ODOMUPDATE;  // command type id
	str[1] = 10;  // len
	str[2] = x & 0xFF;
	str[3] = (x >> 8) & 0xFF;
	str[4] = y & 0xFF;
	str[5] = (y >> 8)& 0xFF;
	str[6] = theta & 0xFF;
	str[7] = (theta >> 8) & 0xFF;
	str[8] = linear_speed & 0xFF;
	str[9] = (linear_speed >> 8) & 0xFF;
	str[10] = angular_speed & 0xFF;
	str[11] = (angular_speed >> 8)& 0xFF;
	send_message(str, sizeof(str));

}
void oiProcessMotorControl(queue_element_t *e)
{
	//printf("Process Motor Control, 0x%X, 0x%X, 0x%X, 0x%X\r\n",e->data[1],e->data[2],e->data[3],e->data[4]);
	short ls = (short)(e->data[1] + (e->data[2]<<8));
	short as = (short)(e->data[3] + (e->data[4]<<8));
	//printf("Process Motor Control, len: %d, ls:%d, as:%d\r\n",e->len,ls,as);
    TIM_OCInitTypeDef TIM_OCInitStructure = {0,};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	if (ls == 0){
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC1Init(TIM8,&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM8,&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC3Init(TIM8,&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC4Init(TIM8,&TIM_OCInitStructure);
	}else if (ls > 0){
		//PC8, PC9 gen pwm
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC1Init(TIM8,&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM8,&TIM_OCInitStructure);

		TIM_OCInitStructure.TIM_Pulse = ls;
		TIM_OC3Init(TIM8,&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_Pulse = ls;
		TIM_OC4Init(TIM8,&TIM_OCInitStructure);
	}else{
		//PC6, PC7 gen pwm
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);

		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC3Init(TIM8,&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC4Init(TIM8,&TIM_OCInitStructure);

		TIM_OCInitStructure.TIM_Pulse = ls;
		TIM_OC1Init(TIM8,&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_Pulse = ls;
		TIM_OC2Init(TIM8,&TIM_OCInitStructure);
	}
    //TIM_Cmd(TIM8,ENABLE);
    //TIM_CtrlPWMOutputs(TIM8,ENABLE);   //TIM1 and TIM8 need to exec this function

}
void oiProcessIMUQuery(queue_element_t *e)
{
	printf("Process IMU Query\r\n");
		char str[8];
		int yaw = 0x0001; //-180~180
		int pitch = 0x0002; //-180~180
		int roll = 0x0003; //-180~180
		str[0] = OI_OPCODE_IMUSTATE;  // command type id
		str[1] = 6;  // len
		str[2] = yaw & 0xFF;
		str[3] = (yaw >> 8) & 0xFF;
		str[4] = pitch & 0xFF;
		str[5] = (pitch >> 8)& 0xFF;
		str[6] = roll & 0xFF;
		str[7] = (roll >> 8)& 0xFF;
		send_message(str, sizeof(str));
}
//------------------------------------
typedef void (*OiHandleOpcode_t)(queue_element_t *e);

typedef struct _OiCmdDispatcher{
    OIOpcode                opcode;
    OiHandleOpcode_t        func;
} OiCmdDispatcher;

OiCmdDispatcher oiOpChecker[] =
{
  {
	OI_OPCODE_MOTORCONTROL,
    oiProcessMotorControl,
  },
  {
     OI_OPCODE_QUERY,
     oiProcessQuery,
   },
   {
	 OI_OPCODE_IMUQUERY,
     oiProcessIMUQuery,
   },
};

#define NUMBER_SUPPORTED_OI_CMD sizeof(oiOpChecker)/sizeof(struct _OiCmdDispatcher)
//------------------------------------


static void process_command(void const *arg)
{
	//unsigned int tail;
	queue_element_t *e = 0;
	//unsigned char *pdata = 0;
	while(1)
	{
		  e=0;
		  //tail =
		  getNextQueueData(&commands, &e);
		  if (e) {
			  int i;
			  for(i=0;i<NUMBER_SUPPORTED_OI_CMD;i++)
			  {
			      if(oiOpChecker[i].opcode == e->data[0])
			      {
			    	   //green led
			    	   GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
			    	   oiOpChecker[i].func(e);
			      }
			   }
			   osDelay(1);
		  }else{
			  osDelay(1);
		  }
	}
}

char param[256];
//queue_element_t elemarray[16];
//int index=0;
static void process_data_in(void const *arg)
{
     //printf("test\r\n");

	 while(1)
	 {
		 queue_element_t elem;
		// printf("process_data_in\r\n");
		  char checksum = 0xFF;
		  do{
		  }while(polling_data() != 0xEA);
		  checksum -= 0xEA;
		  unsigned char Command_type_id = polling_data();
		  checksum -= Command_type_id;
		  //printf("Command_type_id: %d\r\n",Command_type_id);
		  unsigned char len = polling_data();
		  checksum -= len;
		  // Check len if make sense
		  if (len > 16){
			 printf("Check len=%d error, should not longer than 16, skip this\r\n", len);
			 continue;
		  }
		     //
		  //printf("len: %d\r\n",len);
		  int i;
		  for (i=0;i<len;i++){
			  param[i] = polling_data();
			  checksum -= param[i];
		  }
		  char crc = polling_data();
		  if (crc != checksum){
			  printf("Checksum error, skip this\r\n");
			  printf("read crc=0x%X, should be 0x%X\r\n",crc,checksum);
			  continue;
		  }
		  // Pass command_type id and param[] and len


		  elem.data[0] = Command_type_id;
		  for (i=0;i<len;i++){
			  elem.data[i+1]  = param[i];
		  }
		  elem.len = len+1;
		  int ret = pushQueueElement(&commands,  elem);
		  if (!ret){
			  printf("Push command error, skip this\r\n");
			  GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		  }else{
			  //index = ret;
		  }

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

	 osThreadDef(USER_Thread_process_data_in, process_data_in, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	 thread3_id = osThreadCreate(osThread(USER_Thread_process_data_in), NULL);

	 osThreadDef(USER_Thread_process_data_out, process_data_out, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	 thread4_id = osThreadCreate(osThread(USER_Thread_process_data_out), NULL);

	 osThreadDef(USER_Thread_process_command, process_command, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	 thread5_id = osThreadCreate(osThread(USER_Thread_process_command), NULL);


	 vTaskDelete( NULL );
}



int main(void) {
	init();
	  //ledpwm_init();
   // printf("init\r\n");
	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	setbuf(stdout, NULL);
    osThreadDef(USER_Thread1, mainThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    main_thread_id = osThreadCreate(osThread(USER_Thread1), NULL);
    //
    queueInit(&data_in);
    ringbufInit(&data_out);
    queueInit(&commands);
    //
    osKernelStart();
	while(1){};
	return 0;
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {

}

