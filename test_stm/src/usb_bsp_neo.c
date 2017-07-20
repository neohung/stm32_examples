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
#include "usbd_def.h"

#include "usbd_core.h" // for USB_OTG_CONFIGURED, USBD_OK
#include "queue.h"
extern queue_t data_in;
//extern uint8_t Buffer[];
uint8_t Buffer[256];

//======================================(1) Start =======================================
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

void USB_OTG_BSP_mDelay (const uint32_t msec)
{

    USB_OTG_BSP_uDelay(msec * 1000);    

}
//======================================(1) End =======================================
//======================================(2) Start =======================================
void USBD_USR_Init(void)
{
  //printf("USBD_USR_Init\r\n");
}

void USBD_USR_DeviceReset(uint8_t speed )
{
 switch (speed)
 {
   case USB_OTG_SPEED_HIGH:
     break;

  case USB_OTG_SPEED_FULL:
	  //printf("USBD_USR_DeviceReset FULL\r\n");
     break;
 default:
     break;

 }
}
void USBD_USR_DeviceSuspended(void)
{
	//printf("USBD_USR_DeviceSuspended\r\n");
}
void USBD_USR_DeviceResumed(void)
{
	//printf("USBD_USR_DeviceResumed\r\n");
}

void USBD_USR_DeviceConfigured (void)
{
}


void USBD_USR_DeviceConnected (void)
{
}

void USBD_USR_DeviceDisconnected (void)
{
}

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};
//======================================(2) End =======================================

//======================================(3) Start =======================================
extern void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len);

#define USBD_LANGID_STRING            	0x409
#define USBD_MANUFACTURER_STRING      	"STMicroelectronics"
#define USBD_PRODUCT_FS_STRING        	"Combo USB HID device in FS mode"
#define USBD_SERIALNUMBER_FS_STRING   	"00000000011C"
#define USBD_CONFIGURATION_FS_STRING  	"HID Config"
#define USBD_INTERFACE_FS_STRING      	"HID Interface"

extern __ALIGN_BEGIN uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ] __ALIGN_END ;

#define USB_SIZ_STRING_LANGID                   4
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID] __ALIGN_END =
{
     USB_SIZ_STRING_LANGID,
     USB_DESC_TYPE_STRING,
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING),
};

//--------------------------------(4) Start----------------------------------------
#define USB_SIZ_DEVICE_DESC                     18
//0x01 means USB_DESC_TYPE_DEVICE
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
//0x16c0 = 5824
#define USBD_VID                     	0x16c0
//0x5df = 1503
#define USBD_PID                     	0x5df
//#define USBD_CFG_MAX_NUM                1
//#define USB_OTG_MAX_EP0_SIZE                 64
__ALIGN_BEGIN uint8_t USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] __ALIGN_END =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB 0x00,0x02 or 0x10,0x01*/
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    USB_OTG_MAX_EP0_SIZE,      /*bMaxPacketSize*/
    LOBYTE(USBD_VID),           /*idVendor*/
    HIBYTE(USBD_VID),           /*idVendor*/
    LOBYTE(USBD_PID),           /*idVendor*/
    HIBYTE(USBD_PID),           /*idVendor*/
    0x00,                       /*bcdDevice rel. 2.00 = 0x00,0x02*/
    0x02,
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_CFG_MAX_NUM            /*bNumConfigurations*/
  } ; /* USB_DeviceDescriptor */
//--------------------------------(4) End----------------------------------------

uint8_t *  USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length)
{
  *length = sizeof(USBD_DeviceDesc);
  return USBD_DeviceDesc;
}
uint8_t *  USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length)
{
  *length =  sizeof(USBD_LangIDDesc);
  return USBD_LangIDDesc;
}
uint8_t *  USBD_USR_ManufacturerStrDescriptor( uint8_t speed , uint16_t *length)
{
  USBD_GetString ((uint8_t*)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

uint8_t *  USBD_USR_ProductStrDescriptor( uint8_t speed , uint16_t *length)
{
  USBD_GetString ((uint8_t*)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}
uint8_t *  USBD_USR_SerialStrDescriptor( uint8_t speed , uint16_t *length)
{
  USBD_GetString ((uint8_t*)USBD_SERIALNUMBER_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}
uint8_t *  USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length)
{
  USBD_GetString ((uint8_t*)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}
uint8_t *  USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length)
{
  USBD_GetString ((uint8_t*)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}
USBD_DEVICE USR_desc =
{
  USBD_USR_DeviceDescriptor,
  USBD_USR_LangIDStrDescriptor,
  USBD_USR_ManufacturerStrDescriptor,
  USBD_USR_ProductStrDescriptor,
  USBD_USR_SerialStrDescriptor,
  USBD_USR_ConfigStrDescriptor,
  USBD_USR_InterfaceStrDescriptor,
};

//======================================(3) End =======================================

//======================================(5) Start =======================================

#define USB_HID_CONFIG_DESC_SIZ       34+7
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE           0x21

#define HID_IN_EP                    0x81
#define HID_OUT_EP                   0x01
#define HID_IN_PACKET                16
#define HID_OUT_PACKET               16
#include "hid_report_desc.h"
__ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, // bLength: Configuration Descriptor size
  USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType: 0x02=Configuration Descriptor
  USB_HID_CONFIG_DESC_SIZ,           // Total Size Low bytes=34
  0x00,                              // Total Size Hi bytes
  0x01,         //bNumInterfaces: 1 interface
  0x01,         //bConfigurationValue: Configuration value
  0x00,         //iConfiguration: Index of string descriptor describing the configuration
  0xE0,         //bmAttributes: bus powered and Support Remote Wake-up
  0x32,         //MaxPower 100 mA: this current is used for detecting Vbus
  /************** Interface Descriptor(Joystick Mouse) ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_INTERFACE_DESCRIPTOR_TYPE,   //bDescriptorType: Interface descriptor type=0x04
  0x00,         //bInterfaceNumber: Number of Interface
  0x00,         //bAlternateSetting: Alternate setting
  0x02,         //bNumEndpoints: 1 EP
  0x03,         //bInterfaceClass: 0x03=HID
  0x00,   /*0x01*/      //bInterfaceSubClass : 1=BOOT, 0=no boot
  0x00,   /*0x02*/      //nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
  0,            //iInterface: Index of string descriptor
  /******************** HID Descriptor (Joystick Mouse) ********************/
  /* 18 */
  0x09,         //bLength: HID Descriptor size
  HID_DESCRIPTOR_TYPE, //bDescriptorType: 0x21=HID
  0x11,         //bcdHID Low Byte: HID Class Spec release number=v1.11
  0x01,         //bcdHID Hi Byte
  0x00,         //bCountryCode: Hardware target country
  0x01,         //bNumDescriptors: Number of HID class descriptors to follow
  0x22,         //bDescriptorType 0x22=Report Type
  HID_REPORT_DESC_SIZE, //wItemLength Low Byte: Total length of Report descriptor
  0x00,                 //wItemLength Hi Byte
  /******************** Endpoint Descriptor ********************/
  /* 27 */
  0x07,          //bLength: Endpoint Descriptor size
  USB_ENDPOINT_DESCRIPTOR_TYPE, //bDescriptorType: 0x05=EP
  HID_IN_EP,     //bEndpointAddress: Endpoint Address (IN) 0x81
  0x03,          //bmAttributes: Interrupt endpoint=0x03
  HID_IN_PACKET, //wMaxPacketSize Low Byte: 4 Byte max
  0x00,          //wMaxPacketSize Hi Byte
  0x0A,          //bInterval: Polling Interval (10 ms)
  /* 34 */
  /******************** Endpoint Descriptor ********************/
   0x07,          //bLength: Endpoint Descriptor size
   USB_ENDPOINT_DESCRIPTOR_TYPE, //bDescriptorType: 0x05=EP
   HID_OUT_EP,     //bEndpointAddress: Endpoint Address (IN) 0x81
   0x03,          //bmAttributes: Interrupt endpoint=0x03
   HID_OUT_PACKET, //wMaxPacketSize Low Byte: 4 Byte max
   0x00,          //wMaxPacketSize Hi Byte
   0x0A,          //bInterval: Polling Interval (10 ms)
   /* 41 */
} ;



static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
  *length = sizeof (USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}

//======================================(5) End =======================================

//======================================(6) Start =======================================
extern uint32_t DCD_EP_Open(USB_OTG_CORE_HANDLE *pdev , uint8_t ep_addr,uint16_t ep_mps,uint8_t ep_type);
extern uint32_t  DCD_EP_Flush (USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
extern uint32_t DCD_EP_Close(USB_OTG_CORE_HANDLE *pdev , uint8_t  ep_addr);

#define USB_OTG_EP_INT                           3
static uint8_t  USBD_HID_Init (void  *pdev,
                               uint8_t cfgidx)
{
  printf("USBD_HID_Init\r\n");
  /* Open EP IN */
  DCD_EP_Open(pdev,HID_IN_EP,HID_IN_PACKET, USB_OTG_EP_INT);
  /* Open EP OUT */
  DCD_EP_Open(pdev, HID_OUT_EP, HID_OUT_PACKET, USB_OTG_EP_INT);
  /* Prepare Out endpoint to receive next packet */
  DCD_EP_PrepareRx(pdev, HID_OUT_EP, (uint8_t*)(Buffer), HID_OUT_PACKET);
  return USBD_OK;
}

static uint8_t  USBD_HID_DeInit (void  *pdev,
                                 uint8_t cfgidx)
{
  /* Close HID EPs */
  DCD_EP_Close (pdev , HID_IN_EP);
  DCD_EP_Close (pdev , HID_OUT_EP);
  return USBD_OK;
}

static uint8_t  USBD_HID_DataIn (void  *pdev,
                              uint8_t epnum)
{
  //printf("USBD_HID_DataIn\r\n");
  DCD_EP_Flush(pdev, HID_IN_EP);
  return USBD_OK;
}

static uint8_t  USBD_HID_DataOut (void  *pdev,
                              uint8_t epnum)
{
	uint16_t USB_RecData_Cnt;
	if (epnum == HID_OUT_EP)
	{
		USB_RecData_Cnt = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;
		//printf("USBD_HID_DataOut for HID_OUT_EP, read size=%d\r\n", USB_RecData_Cnt);
		if (((USB_OTG_CORE_HANDLE*)pdev)->dev.device_status == USB_OTG_CONFIGURED )
		{
			//printf("USBD_HID_DataOut: USB_OTG_CONFIGURED\r\n");
			int i;
			printf("Buffer[%d]:\r\n",USB_RecData_Cnt);
			queue_element_t elem;
			for(i=0;i<USB_RecData_Cnt;i++){
				printf("0x%X ",Buffer[i]);
				elem.data[i] = Buffer[i];
			}
			elem.len = USB_RecData_Cnt;
			printf("\r\n");
			while (!pushQueueElement(&data_in,  elem)) {
				//printf("\nout buffer full!!!\n");
				 osDelay(5);
			}
		}else{
			printf("USBD_HID_DataOut: Not USB_OTG_CONFIGURED\r\n");
		}

		// Prepare Out endpoint to receive next packet
		DCD_EP_PrepareRx(pdev, HID_OUT_EP, (uint8_t*)(Buffer), HID_OUT_PACKET);
	}
  return USBD_OK;
}

typedef enum
{
  HID_REQ_IDLE = 0,
  HID_REQ_GET_REPORT_DESC,
  HID_REQ_GET_HID_DESC,
  HID_REQ_SET_IDLE,
  HID_REQ_SET_PROTOCOL,
  HID_REQ_SET_REPORT,
}
HID_CtlState;

#define HID_REPORT_DESC               0x22
#define USB_HID_DESC_SIZ              9
#include "hid_report_desc.h"

void send_descriptor(void  *pdev, USB_SETUP_REQ *req)
{
	  uint16_t len = 0;
	  uint8_t  *pbuf = NULL;
	  printf("req->wValue >> 8 == 0x%x\r\n",req->wValue >> 8);
	  if( req->wValue >> 8 == HID_REPORT_DESC)
	  {
		len = MIN(HID_REPORT_DESC_SIZE , req->wLength);
		pbuf = HID_ReportDesc;
	  }
	  else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
	  {
		pbuf = USBD_HID_CfgDesc + 0x12;
		len = MIN(USB_HID_DESC_SIZ , req->wLength);
	  }
	  USBD_CtlSendData (pdev, pbuf,len);
}

#define HID_REQ_SET_IDLE              0x0A
#define HID_REQ_GET_REPORT            0x01


extern void neo(void);
void neo1(void)
{

}

static void USBD_ClrFeature(USB_OTG_CORE_HANDLE  *pdev,  USB_SETUP_REQ *req)
{
	printf("USBD_ClrFeature: pdev->dev.device_status=%d,req->wValue=%d\r\n",pdev->dev.device_status,req->wValue);
	switch (pdev->dev.device_status)
	{
	case USB_OTG_ADDRESSED:
	case USB_OTG_CONFIGURED:
		//USB_FEATURE_EP_HALT=0
		if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
		    {
		      pdev->dev.DevRemoteWakeup = 0;
		      //pdev->dev.class_cb->Setup (pdev, req);
		      printf("req->bmRequest & USB_REQ_TYPE_MASK= 0x%x, req->bRequest=0x%x\r\n ",req->bmRequest & USB_REQ_TYPE_MASK,req->bRequest);
		      USBD_CtlSendStatus(pdev);
		    }
	default :
	     USBD_CtlError(pdev , req);
	    break;
	}

}

__ALIGN_BEGIN static uint32_t  USBD_HID_IdleState __ALIGN_END = 0;
static uint8_t  USBD_HID_Setup (void  *pdev,
                                USB_SETUP_REQ *req)
{
	// see HID1_11.pdf sect 7.2 and http://vusb.wikidot.com/driver-api
	//0x20,then 0x00
	  printf("USBD_HID_Setup (req->bmRequest & USB_REQ_TYPE_MASK):0x%x, req->bRequest:%d\r\n",(req->bmRequest & USB_REQ_TYPE_MASK),req->bRequest);
	  switch (req->bmRequest & USB_REQ_TYPE_MASK)
	  {
		  case USB_REQ_TYPE_CLASS :
		    switch (req->bRequest)
		    {
		      case HID_REQ_GET_REPORT:
		    	printf("(req->wValue & 0xFF=%d\r\n",req->wValue & 0xFF);
		        if ((req->wValue & 0xFF) == 1){

		    	}else if ((req->wValue & 0xFF) == 2){

		    	}else if ((req->wValue & 0xFF) == 3){

		    	}else if ((req->wValue & 0xFF) == 4){

		    	}
		        //USBD_CtlError (pdev, req);
		        //return USBD_FAIL;
		    	break;

		      case HID_REQ_SET_IDLE:
		    	USBD_HID_IdleState = 0;
		    	USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
		    	printf("USBD_HID_IdleState=%d\r\n",USBD_HID_IdleState);
		    	break;
			  default:
		      USBD_CtlError (pdev, req);
		      return USBD_FAIL;
		    }
		    break;

		  case USB_REQ_TYPE_STANDARD:
		    switch (req->bRequest)
		    {
		    case USB_REQ_GET_DESCRIPTOR:

		    	send_descriptor(pdev,req);
		    	break;
		    case  USB_REQ_GET_INTERFACE :
		    	break;
		    case USB_REQ_SET_INTERFACE :
		  		break;
		    case USB_REQ_CLEAR_FEATURE:
		        USBD_ClrFeature (pdev , req);
		        break;
		    default:
		   	  break;
			}
		    break;
	  }
	  return USBD_OK;
}
USBD_Class_cb_TypeDef  USBD_neo_cb =
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL, /*EP0_TxSent*/
  NULL, /*EP0_RxReady*/
  USBD_HID_DataIn, /*DataIn*/
  USBD_HID_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,
  USBD_HID_GetCfgDesc,
};


uint8_t USBD_HID_SendReport     (USB_OTG_CORE_HANDLE  *pdev,
                                 uint8_t *report,
                                 uint16_t len)
{
  if (pdev->dev.device_status == USB_OTG_CONFIGURED )
  {
    DCD_EP_Tx (pdev, HID_IN_EP, report, len);
  }
  return USBD_OK;
}
//======================================(6) End =======================================
