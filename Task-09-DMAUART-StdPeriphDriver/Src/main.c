#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "main.h"

// Private variables
volatile uint32_t time_var1, time_var2;

// Private function prototypes
void Delay(volatile uint32_t nCount);

//------------------------------------------------
#define RX_BUFFER_SIZE 128
#define TX_MAX_BUFFER_SIZE 128

char UartTxBuffer[TX_MAX_BUFFER_SIZE] = {0};
uint16_t UartTxBuffer_Length_Current = 0;
volatile char UartRxBuffer[RX_BUFFER_SIZE] = {0};
//--------------------------------------------------------------
/*
//USART2, TX:PA2 RX:PA3
//USART2_TX: DMA1_Stream6_Channel4
//USART2_RX: DMA1_Stream5_Channel4
#define UARTX	             USART2
#define GPIOx_RCC_AHBxPeriphClockCmd RCC_AHB1PeriphClockCmd
#define UARTx_RCC_APBxPeriphClockCmd RCC_APB1PeriphClockCmd
#define DMAx_RCC_AHBxPeriphClockCmd  RCC_AHB1PeriphClockCmd
#define GPIOX_CLOCK 		 RCC_AHB1Periph_GPIOA
#define UARTX_CLOCK          RCC_APB1Periph_USART2
//Tx DMA Mode
#define UARTX_TX_PORT        GPIOA
#define UARTX_TX_PIN_SOURCE  GPIO_PinSource2
#define UARTX_TX_AF          GPIO_AF_USART2
#define UARTX_TX_PIN         GPIO_Pin_2
#define UARTX_TX_DMA_CLOCK   RCC_AHB1Periph_DMA1
#define UARTX_TX_DMA_STREAM  DMA1_Stream6
#define UARTX_TX_DMA_CHANNEL DMA_Channel_4
#define UARTX_TX_DMA_IRQn    DMA1_Stream6_IRQn
//Rx Interrupt Mode
#define UARTX_RX_PORT        GPIOA
#define UARTX_RX_PIN_SOURCE  GPIO_PinSource3
#define UARTX_RX_AF          GPIO_AF_USART2
#define UARTX_RX_PIN         GPIO_Pin_3
#define UARTX_RX_DMA_CLOCK   RCC_AHB1Periph_DMA1
#define UARTX_RX_DMA_STREAM  DMA1_Stream5
#define UARTX_RX_DMA_CHANNEL DMA_Channel_4
#define UARTX_RX_DMA_IRQn    DMA1_Stream5_IRQn

#define UARTX_RX_IRQn        USART2_IRQn

#define UARTX_TX_DMA_IRQHandler DMA1_Stream6_IRQHandler
//For TX Stream TCIFx
#define UARTX_TX_DMA_IT_TCIFx DMA_IT_TCIF6
#define UARTX_RX_IRQHandler USART2_IRQHandler
*/
//--------------------------------------------------------------
/*
//USART3, TX:PB10 RX:PB11
//USART3_TX: DMA1_Stream3_Channel4
//USART3_RX: DMA1_Stream1_Channel4
#define UARTX	             USART3
#define GPIOx_RCC_AHBxPeriphClockCmd RCC_AHB1PeriphClockCmd
#define UARTx_RCC_APBxPeriphClockCmd RCC_APB1PeriphClockCmd
#define DMAx_RCC_AHBxPeriphClockCmd  RCC_AHB1PeriphClockCmd
#define GPIOX_CLOCK 		 RCC_AHB1Periph_GPIOB
#define UARTX_CLOCK          RCC_APB1Periph_USART3
//Tx DMA Mode
#define UARTX_TX_PORT        GPIOB
#define UARTX_TX_PIN_SOURCE  GPIO_PinSource10
#define UARTX_TX_AF          GPIO_AF_USART3
#define UARTX_TX_PIN         GPIO_Pin_10
#define UARTX_TX_DMA_CLOCK   RCC_AHB1Periph_DMA1
#define UARTX_TX_DMA_STREAM  DMA1_Stream3
#define UARTX_TX_DMA_CHANNEL DMA_Channel_4
#define UARTX_TX_DMA_IRQn    DMA1_Stream3_IRQn
//Rx Interrupt Mode
#define UARTX_RX_PORT        GPIOB
#define UARTX_RX_PIN_SOURCE  GPIO_PinSource11
#define UARTX_RX_AF          GPIO_AF_USART3
#define UARTX_RX_PIN         GPIO_Pin_11
#define UARTX_RX_DMA_CLOCK   RCC_AHB1Periph_DMA1
#define UARTX_RX_DMA_STREAM  DMA1_Stream1
#define UARTX_RX_DMA_CHANNEL DMA_Channel_4
#define UARTX_RX_DMA_IRQn    DMA1_Stream1_IRQn

#define UARTX_RX_IRQn        USART3_IRQn

#define UARTX_TX_DMA_IRQHandler DMA1_Stream3_IRQHandler
//For TX Stream TCIFx
#define UARTX_TX_DMA_IT_TCIFx DMA_IT_TCIF3
#define UARTX_RX_IRQHandler USART3_IRQHandler
*/
//--------------------------------------------------------------
//USART1, TX:PB6 RX:PB7
//USART1_TX: DMA2_Stream7_Channel4
//USART1_RX: DMA2_Stream2_Channel4
#define UARTX	             USART1
#define GPIOx_RCC_AHBxPeriphClockCmd RCC_AHB1PeriphClockCmd
#define UARTx_RCC_APBxPeriphClockCmd RCC_APB2PeriphClockCmd
#define DMAx_RCC_AHBxPeriphClockCmd  RCC_AHB1PeriphClockCmd
#define GPIOX_CLOCK 		 RCC_AHB1Periph_GPIOB
#define UARTX_CLOCK          RCC_APB2Periph_USART1
//Tx DMA Mode
#define UARTX_TX_PORT        GPIOB
#define UARTX_TX_PIN_SOURCE  GPIO_PinSource6
#define UARTX_TX_AF          GPIO_AF_USART1
#define UARTX_TX_PIN         GPIO_Pin_6
#define UARTX_TX_DMA_CLOCK   RCC_AHB1Periph_DMA2
#define UARTX_TX_DMA_STREAM  DMA2_Stream7
#define UARTX_TX_DMA_CHANNEL DMA_Channel_4
#define UARTX_TX_DMA_IRQn    DMA2_Stream7_IRQn
//Rx Interrupt Mode
#define UARTX_RX_PORT        GPIOB
#define UARTX_RX_PIN_SOURCE  GPIO_PinSource7
#define UARTX_RX_AF          GPIO_AF_USART1
#define UARTX_RX_PIN         GPIO_Pin_7
#define UARTX_RX_DMA_CLOCK   RCC_AHB1Periph_DMA2
#define UARTX_RX_DMA_STREAM  DMA2_Stream2
#define UARTX_RX_DMA_CHANNEL DMA_Channel_4
#define UARTX_RX_DMA_IRQn    DMA2_Stream2_IRQn

#define UARTX_RX_IRQn        USART1_IRQn

#define UARTX_TX_DMA_IRQHandler DMA2_Stream7_IRQHandler
//For TX Stream TCIFx
#define UARTX_TX_DMA_IT_TCIFx DMA_IT_TCIF7
#define UARTX_RX_IRQHandler USART1_IRQHandler
//--------------------------------------------------------------

//For TX
void UARTX_TX_DMA_IRQHandler(void)
{
  // Test on DMA Stream Transfer Complete interrupt
  if (DMA_GetITStatus(UARTX_TX_DMA_STREAM, UARTX_TX_DMA_IT_TCIFx))
  {
    // Clear DMA Stream Transfer Complete interrupt pending bit
    DMA_ClearITPendingBit(UARTX_TX_DMA_STREAM, UARTX_TX_DMA_IT_TCIFx);
  }
}//


void Usart_Printf(char *string){
	UartTxBuffer_Length_Current=strlen(string);
	memcpy(UartTxBuffer, string, UartTxBuffer_Length_Current);
	DMA_SetCurrDataCounter(UARTX_TX_DMA_STREAM, UartTxBuffer_Length_Current);
	DMA_Cmd(UARTX_TX_DMA_STREAM, ENABLE);
}
/*
//DMA RX don't do any thing
char str[RX_BUFFER_SIZE] = "";
void DMA1_Stream5_IRQHandler(void)
{
  // Test on DMA Stream Transfer Complete interrupt
  if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
  {
	//Green LED
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
    // Clear DMA Stream Transfer Complete interrupt pending bit
    DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);

    //After Receive RX_BUFFER_SIZE chars would trigger once
	sprintf (str, "\r\n<%s>\r\n", UartRxBuffer);
	Usart_Printf(str);

    DMA_Cmd(DMA1_Stream5, ENABLE);
  }
}
*/
volatile int receive_flag = 0;
void UARTX_RX_IRQHandler(void)
{
    if(USART_GetITStatus(UARTX, USART_IT_IDLE) != RESET)
    {
    	 GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    	 short CurrDataCount = UARTX->SR;  //Before clean USART_IT_IDLE, need to read this!!
    	 CurrDataCount = UARTX->DR;        //Before clean USART_IT_IDLE, need to read this!!
    	 USART_ClearITPendingBit(UARTX, USART_IT_IDLE);
 		 DMA_Cmd(UARTX_RX_DMA_STREAM, DISABLE);
     	 CurrDataCount = RX_BUFFER_SIZE - DMA_GetCurrDataCounter(UARTX_RX_DMA_STREAM);
 		 UartRxBuffer[CurrDataCount] = '\0';
 		 UARTX_RX_DMA_STREAM->NDTR = RX_BUFFER_SIZE; //Reload
 		 DMA_Cmd(UARTX_RX_DMA_STREAM, ENABLE);

 		 //Here can push UartRxBuffer to your ring buffer

 		 receive_flag  = 1;
    }
}

void Usart_Init(uint32_t rate)
{
             USART_InitTypeDef USART_InitStructure;
             GPIO_InitTypeDef GPIO_InitStructure;
             GPIOx_RCC_AHBxPeriphClockCmd(GPIOX_CLOCK, ENABLE);
             UARTx_RCC_APBxPeriphClockCmd(UARTX_CLOCK, ENABLE);
             /*PA2 Tx, PA3 Rx*/
             GPIO_PinAFConfig(UARTX_TX_PORT, UARTX_TX_PIN_SOURCE, UARTX_TX_AF);
             GPIO_PinAFConfig(UARTX_RX_PORT, UARTX_RX_PIN_SOURCE, UARTX_RX_AF);
             /*PA2: Tx Setup*/
             GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
             GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
             GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
             GPIO_InitStructure.GPIO_Pin = UARTX_TX_PIN;
             GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
             GPIO_Init(UARTX_TX_PORT, &GPIO_InitStructure);
             /*PA3: Rx Setup*/
             GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
             GPIO_InitStructure.GPIO_Pin = UARTX_RX_PIN;
             GPIO_Init(UARTX_RX_PORT, &GPIO_InitStructure);
             USART_InitStructure.USART_BaudRate = rate;
             USART_InitStructure.USART_WordLength = USART_WordLength_8b;
             USART_InitStructure.USART_StopBits = USART_StopBits_1;
             USART_InitStructure.USART_Parity = USART_Parity_No;
             USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
             USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
             USART_Init(UARTX, &USART_InitStructure);
             USART_Cmd(UARTX, ENABLE);
             /* DMA1 clock enable */
             DMAx_RCC_AHBxPeriphClockCmd(UARTX_TX_DMA_CLOCK, ENABLE);
             DMA_InitTypeDef  DMA_InitStructure;
             DMA_DeInit(UARTX_TX_DMA_STREAM);
             DMA_InitStructure.DMA_Channel = UARTX_TX_DMA_CHANNEL;
             DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
             DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UartTxBuffer;
             DMA_InitStructure.DMA_BufferSize = UartTxBuffer_Length_Current;
             DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UARTX->DR;
             DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
             DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
             DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
             DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
             DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
             DMA_InitStructure.DMA_Priority = DMA_Priority_High;
             DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
             DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
             DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
             DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
             DMA_Init(UARTX_TX_DMA_STREAM, &DMA_InitStructure);
             /* Enable the USART Tx DMA request */
             USART_DMACmd(UARTX, USART_DMAReq_Tx, ENABLE);
             /* Enable DMA Stream Transfer Complete interrupt */
             DMA_ITConfig(UARTX_TX_DMA_STREAM, DMA_IT_TC, ENABLE);
             /* Disable the DMA Tx Stream */
             DMA_Cmd(UARTX_TX_DMA_STREAM, DISABLE);
             /* Enable the USART1 Tx DMA Interrupt */
             NVIC_EnableIRQ(UARTX_TX_DMA_IRQn);
             /* DMA1 clock enable */
             DMAx_RCC_AHBxPeriphClockCmd(UARTX_RX_DMA_CLOCK, ENABLE);
             DMA_DeInit(UARTX_RX_DMA_STREAM);
             DMA_InitStructure.DMA_Channel = UARTX_RX_DMA_CHANNEL;
             DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
             DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UartRxBuffer;
             DMA_InitStructure.DMA_BufferSize = RX_BUFFER_SIZE;
             DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UARTX->DR;
             DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
             DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
             DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
             DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
             DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
             DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
             DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
             DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
             DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
             DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
             DMA_Init(UARTX_RX_DMA_STREAM, &DMA_InitStructure);
             /* Enable the USART Rx DMA request */
             USART_DMACmd(UARTX, USART_DMAReq_Rx, ENABLE);
             /* Enable the DMA Rx Stream */
             DMA_Cmd(UARTX_RX_DMA_STREAM, ENABLE);
             NVIC_InitTypeDef NVIC_InitStruct;
             NVIC_InitStruct.NVIC_IRQChannel = UARTX_RX_IRQn;
             NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
             NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
             NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
             NVIC_Init(&NVIC_InitStruct);
             /* Enable the UARTX Rx IDLE Interrupt */
             USART_ITConfig(UARTX, USART_IT_IDLE, ENABLE);
             NVIC_EnableIRQ(UARTX_RX_IRQn);
             /* Enable DMA Stream Transfer Complete [TC] interrupt */
             //DMA_ITConfig(UARTX_RX_DMA_STREAM, DMA_IT_TC, ENABLE);
             //NVIC_EnableIRQ(UARTX_RX_DMA_IRQn);
}


void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void init() {
	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}
	//
	LED_Init();
	//
	//Usart_Init(115200);
	Usart_Init(460800);
	//
}

int main(void) {
	init();
	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	setbuf(stdout, NULL);
	char s[128];
	Usart_Printf("Init complete! Hello World DMA !\r\n");
	for(;;) {
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		while(receive_flag == 0){
		};
		receive_flag = 0;
		sprintf (s, "%s", UartRxBuffer);
		Usart_Printf(s);
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

