/**
  ******************************************************************************
  * @file    usb_bsp.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    22-July-2011
  * @brief   This file is responsible to offer board support package and is
  *          configurable by user.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usb_bsp.h"
#include "usbd_conf.h"
#include "stm32f4xx_conf.h"


/** @addtogroup USB_OTG_DRIVER
* @{
*/

/** @defgroup USB_BSP
  * @brief This file is responsible to offer board support package
  * @{
  */ 

/** @defgroup USB_BSP_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_BSP_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 





/** @defgroup USB_BSP_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBH_BSP_Private_Variables
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBH_BSP_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_BSP_Private_Functions
  * @{
  */ 


/**
  * @brief  USB_OTG_BSP_Init
  *         Initilizes BSP configurations
  * @param  None
  * @retval None
  */

//void USB_OTG_BSP_Init(void)
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE);
	/* Configure SOF ID DM DP Pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  |
	                              GPIO_Pin_11 |
	                              GPIO_Pin_12;

	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_OTG1_FS) ;
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_OTG1_FS) ;
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_OTG1_FS) ;

	  /* Configure  VBUS Pin */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  /* Configure ID pin */
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_OTG1_FS) ;

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE) ;
}
/**
  * @brief  USB_OTG_BSP_EnableInterrupt
  *         Enabele USB Global interrupt
  * @param  None
  * @retval None
  */
//void USB_OTG_BSP_EnableInterrupt(void)
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  BSP_Drive_VBUS
  *         Drives the Vbus signal through IO
  * @param  speed : Full, Low 
  * @param  state : VBUS states
  * @retval None
  */

void USB_OTG_BSP_DriveVBUS(uint32_t speed, uint8_t state)
{

}

/**
  * @brief  USB_OTG_BSP_ConfigVBUS
  *         Configures the IO for the Vbus and OverCurrent
  * @param  Speed : Full, Low 
  * @retval None
  */

void  USB_OTG_BSP_ConfigVBUS(uint32_t speed)
{

}

/**
  * @brief  USB_OTG_BSP_TimeInit
  *         Initialises delay unit Systick timer /Timer2
  * @param  None
  * @retval None
  */
void USB_OTG_BSP_TimeInit ( void )
{

}

/**
  * @brief  USB_OTG_BSP_uDelay
  *         This function provides delay time in micro sec
  * @param  usec : Value of delay required in micro sec
  * @retval None
  */
void USB_OTG_BSP_uDelay (const uint32_t usec)
{

  uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1); 
  
}


/**
  * @brief  USB_OTG_BSP_mDelay
  *          This function provides delay time in milli sec
  * @param  msec : Value of delay required in milli sec
  * @retval None
  */
void USB_OTG_BSP_mDelay (const uint32_t msec)
{

    USB_OTG_BSP_uDelay(msec * 1000);    

}


/**
  * @brief  USB_OTG_BSP_TimerIRQ
  *         Time base IRQ
  * @param  None
  * @retval None
  */

void USB_OTG_BSP_TimerIRQ (void)
{

} 

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/


void USBD_USR_Init(void)
{
  printf("USBD_USR_Init\r\n");
}

void USBD_USR_DeviceReset(uint8_t speed )
{
 switch (speed)
 {
   case USB_OTG_SPEED_HIGH:
     break;

  case USB_OTG_SPEED_FULL:
	  printf("USBD_USR_DeviceReset FULL\r\n");
     break;
 default:
     break;

 }
}

void USBD_USR_DeviceSuspended(void)
{
	printf("USBD_USR_DeviceSuspended\r\n");
}

void USBD_USR_DeviceResumed(void)
{
	printf("USBD_USR_DeviceResumed\r\n");
}

static uint8_t  usbd_neo_Init (void  *pdev,
                               uint8_t cfgidx)
{
	printf("usbd_neo_Init\r\n");
}

USBD_DEVICE USR_desc =
{
  0,//USBD_USR_DeviceDescriptor,
  0,//USBD_USR_LangIDStrDescriptor,
  0,//USBD_USR_ManufacturerStrDescriptor,
  0,//USBD_USR_ProductStrDescriptor,
  0,//USBD_USR_SerialStrDescriptor,
  0,//USBD_USR_ConfigStrDescriptor,
  0,//USBD_USR_InterfaceStrDescriptor,
};

USBD_Class_cb_TypeDef  USBD_neo_cb =
{
  usbd_neo_Init,
  0,//usbd_cdc_DeInit,
  0,//usbd_cdc_Setup,
  0,//NULL,                 /* EP0_TxSent, */
  0,//usbd_cdc_EP0_RxReady,
  0,//usbd_cdc_DataIn,
  0,//usbd_cdc_DataOut,
  0,//usbd_cdc_SOF,
  0,//NULL,
  0,//NULL,
  0,//USBD_cdc_GetCfgDesc,
};

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  0,//USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  0,//USBD_USR_DeviceConnected,
  0,//USBD_USR_DeviceDisconnected,
};
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
