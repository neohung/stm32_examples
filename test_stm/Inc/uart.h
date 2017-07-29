#ifndef UART_H_
#define UART_H_

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

void Usart_Init(uint32_t rate);
void Set_Uart_RX_Callback(void (*cb)(char* buf, short len));
void Usart_Printf(char *string);
void Usart_Send(char *buf, int len);

//------------------------------------------------
#define RX_BUFFER_SIZE 128
#define TX_MAX_BUFFER_SIZE 128
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



#endif /* UART_H_ */
