//============================================================================
// Name        : libusb_example.cpp
// Author      : neo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <conio.h> //getch()
#include <lusb0_usb.h>

#include <pthread.h>

unsigned char Buffer[128];

usb_dev_handle *open_dev(unsigned short MY_VID, unsigned short MY_PID)
{
    struct usb_bus *bus;
    struct usb_device *dev;
	printf("open..\n");

    for (bus = usb_get_busses(); bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next)
        {
			printf("show device %x, %x\n",dev->descriptor.idVendor,dev->descriptor.idProduct);
            if (dev->descriptor.idVendor == MY_VID
                    && dev->descriptor.idProduct == MY_PID)
            {
                return usb_open(dev);
            }
        }
    }
    return NULL;
}

struct usb_dev_handle* usbhid_start(unsigned short PID, unsigned short VID, int INTF)
{
		usb_init(); // initialize the library
		usb_find_busses(); // find all busses
		usb_find_devices(); // find all connected devices
		struct usb_dev_handle* usbhandle = NULL;
		if (!(usbhandle = open_dev(PID, VID)))
		{
		        printf("error opening device: \n%s\n", usb_strerror());
		        exit(0);
		}
		else
		{
		    printf("success: device %04X:%04X opened\n", PID, VID);
		}
	    //
		//int MY_INTF = 0;
		if (usb_claim_interface(usbhandle, INTF) < 0)  //dfu_bInterfaceNumber
		{
			printf("error claiming interface #%d:\n%s\n", INTF, usb_strerror());
			usb_close(usbhandle);
			exit(0);
		}
		return usbhandle;
}

void usbhid_stop(struct usb_dev_handle* usbhandle, int INTF)
{
	usb_release_interface(usbhandle, INTF);
	usb_close(usbhandle);
}

void usbhid_send(struct usb_dev_handle* usbhandle, int ep, char* buf, unsigned int len)
{
	const static int timeout=5000; // timeout in ms
	int res = usb_interrupt_write(usbhandle, ep, buf, len, timeout);
	if( res < 0 )
	{
	     perror("USB interrupt write");
	     usbhid_stop(usbhandle,0);
	     system("pause");
	     exit(0);
	}
}


void usbhid_read(struct usb_dev_handle* usbhandle, int ep, char* buf, unsigned int len)
{
	const int data_len=255;
	char data[data_len];
	int res;
	res = usb_interrupt_read(usbhandle, ep, buf, len, -1);
	//printf( "\nusb_interrupt_read size=%d \n [ ",res );
	//for(int i=0;i<res;i++) {
	//	printf("0x%X ", data[i]);
	//}
	//printf("]\n");
}

void *thread1(void *arg)
{
	struct usb_dev_handle* usbhandle = (struct usb_dev_handle*)arg;
	int MY_EP_In = 0x81;
	char buf[255];
    while (1)
    {
        //printf("T1T1T1T1T1T1T1T1T1T1T1\n");
        //fflush(stdout);
        usbhid_read(usbhandle, MY_EP_In, buf, 1);
        printf("buf[0]=[%c]\n",buf[0]);
        Sleep(1);
    }
    //pthread_exit((void *)1234);
    return NULL;
}

int main(void) {
	struct usb_dev_handle* usbhandle  = usbhid_start(0x16C0,0x05DF,0);
	//
	int MY_EP_Out = 0x01;

	pthread_t Tid1, Tid2;
	pthread_create(&Tid1, NULL, thread1,  (void*)usbhandle);
	char c;
	puts ("Enter text. Include a dot ('.') in a sentence to exit:");
	do {
	    c=getch();
	    //putchar(c);
	    usbhid_send(usbhandle, MY_EP_Out, &c, 1);
	} while (c != '.');
	//pthread_cancel(Tid1);
	pthread_kill(Tid1, 9);
	void *ret;
	pthread_join( Tid1, &ret);
	//pthread_kill(Tid1, 9); //SIGKILL
	puts("Finish");
	usbhid_stop(usbhandle,0);
	system("pause");
	return EXIT_SUCCESS;
}

/*
int main(void) {
	puts("test");
	usb_init(); // initialize the library
	usb_find_busses(); // find all busses
	usb_find_devices(); // find all connected devices
	struct usb_dev_handle* usbhandle;
	if (!(usbhandle = open_dev(0x16C0, 0x05DF)))
	{
	        printf("error opening device: \n%s\n", usb_strerror());
	        return 0;
	}
	else
	{
	    printf("success: device %04X:%04X opened\n", 0x16C0, 0x05DF);
	}
    //
	int MY_INTF = 0;
	if (usb_claim_interface(usbhandle, MY_INTF) < 0)  //dfu_bInterfaceNumber
	{
		printf("error claiming interface #%d:\n%s\n", MY_INTF, usb_strerror());
		usb_close(usbhandle);
		exit(0);
	}

	int res;
	int MY_EP_In = 0x81;
	int MY_EP_Out = 0x01;
	//
	const int data_len=255;
	char data[data_len];
	res = usb_interrupt_read(usbhandle, MY_EP_In, data, data_len, -1);
	printf( "\nusb_interrupt_read size=%d \n [ ",res );
	for(int i=0;i<res;i++) {
		printf("0x%X ", data[i]);
	}
	printf("]\n");

	const static int timeout=5000; // timeout in ms
	const static int str_len = 2;
	char str[str_len];
	str[0] = 0x11;
	str[1] = 0x22;

	res = usb_interrupt_write(usbhandle, MY_EP_Out, str, str_len, timeout);
	if( res < 0 )
	{
	     perror("USB interrupt write");
	}

	//printf("Dats=0x%X\n",data[0]);
*/
	/*
	#define USB_ENDPOINT_IN         MY_EP
    #define USB_TYPE_CLASS          (0x01 << 5)
    #define USB_RECIP_INTERFACE        0x01
    #define HID_REPORT_GET          0x01
	char recv_data[5];
	#define USB_TIMEOUT     10000
	int recv_len = usb_control_msg(usbhandle,
	USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
	                               HID_REPORT_GET,
	                               0x300,
	                                 0,
	                               recv_data,5, USB_TIMEOUT);
	printf( "\nusb_interrupt_read %d \n",recv_len );
		for(int i=0;i<recv_len;i++) {
			printf("0x%X ", recv_data[i]);
		}
		printf("\n");
		printf("Report_ID=%d, X=%d,Y=%d\n",recv_data[0],recv_data[2],recv_data[3]);
   */
/*
	puts("Finish");
	usb_release_interface(usbhandle, MY_INTF);
	usb_close(usbhandle);
	system("pause");
	return EXIT_SUCCESS;
}
*/
