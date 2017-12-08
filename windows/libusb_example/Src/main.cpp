//============================================================================
// Name        : libusb_example.cpp
// Author      : neo
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>

#include <lusb0_usb.h>


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
	int MY_EP = 0x81;
	//
	const int data_len=255;
	char data[data_len];
	res = usb_interrupt_read(usbhandle, MY_EP, data, data_len, -1);
	printf( "\nusb_interrupt_read %d \n",res );
	for(int i=0;i<res;i++) {
		printf("0x%X ", data[i]);
	}
	printf("\n");
	printf("Report_ID=%d, X=%d,Y=%d\n",data[0],data[2],data[3]);

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

	puts("Finish");
	usb_release_interface(usbhandle, MY_INTF);
	usb_close(usbhandle);
	system("pause");
	return EXIT_SUCCESS;
}
