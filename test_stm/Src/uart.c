#include <uart.h>
#include <stdint.h>
#include <string.h>

char UartTxBuffer[TX_MAX_BUFFER_SIZE] = {0};
uint16_t UartTxBuffer_Length_Current = 0;
//volatile
char UartRxBuffer[RX_BUFFER_SIZE] = {0};

void (*uart_rx_callback)(char* buf, short len) = 0;

void Set_Uart_RX_Callback(void (*cb)(char* buf, short len))
{
	uart_rx_callback = cb;
}

//
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

void Usart_Send(char *buf, int len){
	UartTxBuffer_Length_Current=len;
	memcpy(UartTxBuffer, buf, UartTxBuffer_Length_Current);
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
//volatile int receive_flag = 0;
void UARTX_RX_IRQHandler(void)
{
    if(USART_GetITStatus(UARTX, USART_IT_IDLE) != RESET)
    {
    	 //Red LED
    	 //GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
    	 short CurrDataCount = UARTX->SR;  //Before clean USART_IT_IDLE, need to read this!!
    	 CurrDataCount = UARTX->DR;        //Before clean USART_IT_IDLE, need to read this!!
    	 USART_ClearITPendingBit(UARTX, USART_IT_IDLE);
 		 DMA_Cmd(UARTX_RX_DMA_STREAM, DISABLE);
     	 CurrDataCount = RX_BUFFER_SIZE - DMA_GetCurrDataCounter(UARTX_RX_DMA_STREAM);
 		 UartRxBuffer[CurrDataCount] = '\0';
 		 UARTX_RX_DMA_STREAM->NDTR = RX_BUFFER_SIZE; //Reload
 		 DMA_Cmd(UARTX_RX_DMA_STREAM, ENABLE);

 		 //Here can push UartRxBuffer to your ring buffer
 		 if (uart_rx_callback)
 			 (uart_rx_callback)(UartRxBuffer, CurrDataCount);
 		 //receive_flag  = 1;
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

