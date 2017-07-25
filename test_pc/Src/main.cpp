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

#include <ringbuf.h>
unsigned char Buffer[128];
ringbuf_t message_in_ringbuf;
ringbuf_t message_out_ringbuf;


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


bool usbhid_read(struct usb_dev_handle* usbhandle, int ep, char* buf, unsigned int len)
{
	const int data_len=255;
	const int timeout = 1000;
	char data[data_len];
	int res;
	res = usb_interrupt_read(usbhandle, ep, buf, len, timeout);
	if (res > 0){
		return true;
	}else{
		return false;
	}
}

pthread_t msg_read_handle, process_msg_in_handle, msg_write_handle, Tid2;

void *message_read(void *arg)
{
	 int s;
	 s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	 if (s != 0){
		 printf("pthread_setcancelstate error\n");
		 exit(EXIT_FAILURE);
	 }
	struct usb_dev_handle* usbhandle = (struct usb_dev_handle*)arg;
	int MY_EP_In = 0x81;
	char buf[255];
    while (1)
    {
    	//set cancel interrupt point
    	pthread_testcancel();
        int len = usbhid_read(usbhandle, MY_EP_In, buf, 1);
        if (len){
        	unsigned buflen = pushElement(&message_in_ringbuf,buf[0]);
        	if (len==0){
        		printf("Full, can't push to message_in_ringbuf any more\n");
        	}
        }
        Sleep(1);
    }
    //pthread_exit((void *)1234);
    return NULL;
}

void *process_msg_in(void *arg)
{
	 int s;
	 s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	 if (s != 0){
		 printf("pthread_setcancelstate error\n");
		 exit(EXIT_FAILURE);
	 }
	 ringbuf_t* pmsg_in = (ringbuf_t*)arg;
    while (1)
    {
    	//set cancel interrupt point
    	pthread_testcancel();
    	unsigned char* pdata;
    	unsigned int tail = getNextData(pmsg_in, &pdata);
    	if (pdata){
    		printf("data[%d]=0x%X\n",(tail-1)&(RINGBUF_MAX_LENGTH-1),*pdata);
    		//
    		// Process message here

    		//
    	}
    	Sleep(1);
    }
    //pthread_exit((void *)1234);
    return NULL;
}

void *message_write(void *arg)
{
	int s;
	s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (s != 0){
		printf("pthread_setcancelstate error\n");
		exit(EXIT_FAILURE);
	}
	struct usb_dev_handle* usbhandle = (struct usb_dev_handle*)arg;
	int MY_EP_Out = 0x01;
	while (1)
	{
		pthread_testcancel();
		unsigned char* pdata;
		unsigned int tail = getNextData(&message_out_ringbuf, &pdata);
		if (pdata){
	    		//printf("write data[%d]=[%c]\n",(tail-1)&(RINGBUF_MAX_LENGTH-1),*pdata);
	    		usbhid_send(usbhandle, MY_EP_Out, (char*)pdata, 1);
		}
		Sleep(1);
	}
}

void send_message(char* buf,unsigned int len){
	unsigned int i;
	char crc = 0xFF;
	// Pushh Magic Number
	while (!pushElement(&message_out_ringbuf,0xEA)) {
		Sleep(1);
	}
	crc -= 0xEA;
	//
	for(i=0;i<len;i++){
		while (!pushElement(&message_out_ringbuf,buf[i])) {
			Sleep(1);
		}
		crc -= buf[i];
	}
	//push crc
	printf("Push crc: 0x%X \n",crc);
	while (!pushElement(&message_out_ringbuf,crc)) {
		Sleep(1);
	}
	//
}

void *thread2(void *arg)
{
		struct usb_dev_handle* usbhandle = (struct usb_dev_handle*)arg;
		int MY_EP_Out = 0x01;
		char c;
		puts ("Enter text. Include a dot ('.') in a sentence to exit:");
		do {
		    c=getch();
		    //putchar(c);
		    //usbhid_send(usbhandle, MY_EP_Out, &c, 1);
		    switch (c)
		    {
		    case 'a':
		    {
		    	char str[6];
		    	int linear_speed = 0x0201;
		    	int angular_speed = 0x0403;
		    	str[0] = 0x01;  // command type id
		    	str[1] = 0x4;  // len
		    	str[2] = linear_speed & 0xFF;
		    	str[3] = (linear_speed >> 8) & 0xFF;
		    	str[4] = angular_speed & 0xFF;
		    	str[5] = (angular_speed >> 8)& 0xFF;
		    	send_message(str, sizeof(str));
		    }
		    	break;
		    default:
		    	send_message(&c,1);
		    	break;
		    }
		    Sleep(1);
		} while (c != '.');
		printf("Do Cancel\n");
		pthread_cancel(msg_read_handle);
		pthread_cancel(process_msg_in_handle);
		pthread_cancel(msg_write_handle);
		pthread_exit(NULL);
}

int main(void) {
	struct usb_dev_handle* usbhandle  = usbhid_start(0x16C0,0x05DF,0);
	//
	 ringbufInit(&message_in_ringbuf);
	 ringbufInit(&message_out_ringbuf);
	//
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&msg_read_handle, &attr, message_read,  (void*)usbhandle);
	pthread_create(&process_msg_in_handle, &attr, process_msg_in,  (void*)&message_in_ringbuf);
	pthread_create(&msg_write_handle, &attr, message_write,  (void*)usbhandle);

	pthread_create(&Tid2, &attr, thread2,  (void*)usbhandle);
	void *ret;
	pthread_join(msg_read_handle, &ret);
	pthread_join(process_msg_in_handle, &ret);
	pthread_join(msg_write_handle, &ret);
	pthread_join(Tid2, &ret);
	printf("Tid2 finish\n");
	pthread_attr_destroy(&attr);
	//pthread_kill(Tid1, 9); //SIGKILL
	puts("Finish");
	usbhid_stop(usbhandle,0);
	system("pause");
	return EXIT_SUCCESS;
}
