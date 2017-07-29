#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "main.h"

#define MAX_STRLEN 12 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string


// Private variables
volatile uint32_t time_var1, time_var2;

// Private function prototypes
void Delay(volatile uint32_t nCount);


//-------------------------------------------------
//USART2, TX:PA2 RX:PA3
#define UARTX	             USART2
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
#define UARTX_RX_IRQn        USART2_IRQn
//---------------------------------------------------
void USART2_IRQHandler(){
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

	if(USART_GetITStatus(UARTX, USART_IT_RXNE) != RESET){
		char c = USART_ReceiveData(UARTX);
		USART_SendData(UARTX, c);
	}
}
//

void Usart_Printf(char *string){
    while(*string){
	// wait until data register is empty
	  while( !(UARTX->SR & 0x00000040) );
	  //Send Data to UART
      USART_SendData(UARTX, *string);
      *string++;
    }
}

void Usart_Init(uint32_t rate)
{
      USART_InitTypeDef USART_InitStructure;
      GPIO_InitTypeDef GPIO_InitStructure;
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      RCC_APB1PeriphClockCmd(UARTX_CLOCK, ENABLE);
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

      USART_ITConfig(UARTX, USART_IT_RXNE, ENABLE);
      NVIC_InitTypeDef NVIC_InitStruct;
      NVIC_InitStruct.NVIC_IRQChannel = UARTX_RX_IRQn;
      NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
      NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStruct);

      USART_Cmd(UARTX, ENABLE);

      NVIC_EnableIRQ(UARTX_RX_IRQn);
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
	Usart_Init(115200);
	//
}

int main(void) {
	init();

	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	setbuf(stdout, NULL);

	Usart_Printf("Init complete! Hello World!\r\n");
	for(;;) {
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		//GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(500);
		//GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
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

