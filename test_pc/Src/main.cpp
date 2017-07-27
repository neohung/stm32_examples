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
	const static int timeout=1000; // timeout in ms
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
	int res;
	pthread_testcancel();
	//if (usbhandle)
	res = usb_interrupt_read(usbhandle, ep, buf, len, 1000);
	if (res > 0){
		return true;
	}else{
		return false;
	}
}

pthread_t msg_read_handle, process_msg_in_handle, msg_write_handle, Tid2, Tid1;

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
        	if (buflen==0){
        		printf("Full, can't push to message_in_ringbuf any more\n");
        	}
        }
        Sleep(1);
    }
    //pthread_exit((void *)1234);
    return NULL;
}

unsigned char polling_data(void)
{
	unsigned char* pdata;
	//unsigned int tail;
	while(1){
    	pthread_testcancel();
		//tail =
		getNextData(&message_in_ringbuf, &pdata);
	    if (pdata){
	    		//printf("data[%d]=0x%X\n",(tail-1)&(RINGBUF_MAX_LENGTH-1),*pdata);
	    		break;
	    }else{
	    	Sleep(1);
	    }
	}
	return *pdata;
}


typedef enum _OI_Opcode {
	// Command opcodes
	 OI_OPCODE_MOTORCONTROL  = 1,
	 OI_OPCODE_ODOMUPDATE = 2,
	 OI_OPCODE_QUERY = 3,
	 OI_OPCODE_IMUQUERY = 4,
	 OI_OPCODE_IMUSTATE = 5,
} OIOpcode;
//

typedef void (*OiHandleOpcode_t)(char* arg ,unsigned char arg_len);

typedef struct _OiCmdDispatcher{
    OIOpcode                opcode;
    OiHandleOpcode_t        func;
} OiCmdDispatcher;

void oiProcessOdomUpdate(char* arg ,unsigned char arg_len)
{
	int x = arg[0] + (arg[1]<<8);
	int y = arg[2] + (arg[3]<<8);
	int theta = arg[4] + (arg[5]<<8);
	int linear_speed = arg[6] + (arg[7]<<8);
	int angular_speed = arg[8] + (arg[9]<<8);
	printf("Process oiProcessOdomUpdate, x=%d,y=%d,theta=%d,ls=%d,as=%d\n",x,y,theta,linear_speed,angular_speed);
}

void oiProcessIMUState(char* arg ,unsigned char arg_len)
{
	//printf("Process oiProcessIMUState\r\n");
	int yaw = arg[0] + (arg[1]<<8);
	int pitch = arg[2] + (arg[3]<<8);
	int roll = arg[4] + (arg[5]<<8);
	printf("Process oiProcessIMUState, yaw=%d,pitch=%d,roll=%d\n",yaw,pitch,roll);
}

OiCmdDispatcher oiOpChecker[] =
{
  {
	OI_OPCODE_ODOMUPDATE,
    oiProcessOdomUpdate,
  },
  {
	 OI_OPCODE_IMUSTATE,
     oiProcessIMUState,
   },
};

#define NUMBER_SUPPORTED_OI_CMD sizeof(oiOpChecker)/sizeof(struct _OiCmdDispatcher)
//------------------------------------

void processCommand(unsigned char Command_type_id, char* arg ,unsigned char arg_len)
{
	unsigned int i;
	/*
	printf("Command_type_id=%d\n",Command_type_id);
	printf("arg_len=%d\n",arg_len);
	for(i=0;i<arg_len;i++){
		printf("0x%X ", arg[i]);
	}
	printf("\n");
	*/
	//
	for(i=0;i<NUMBER_SUPPORTED_OI_CMD;i++)
	{
		if(oiOpChecker[i].opcode == Command_type_id)
		{
			oiOpChecker[i].func(arg, arg_len);
		}
	}
}

char param[256];
void *process_msg_in(void *arg)
{
	 int s;
	 s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	 if (s != 0){
		 printf("pthread_setcancelstate error\n");
		 exit(EXIT_FAILURE);
	 }
	// ringbuf_t* pmsg_in = (ringbuf_t*)arg;
    while (1)
    {
    	//set cancel interrupt point
    	pthread_testcancel();
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
    	processCommand(Command_type_id, param, len);
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
		//unsigned int tail =
				getNextData(&message_out_ringbuf, &pdata);
		if (pdata){
	    		//printf("write data[%d]=[%c]\n",(tail-1)&(RINGBUF_MAX_LENGTH-1),*pdata);
	    		usbhid_send(usbhandle, MY_EP_Out, (char*)pdata, 1);
		}
		Sleep(1);
	}
}

//MUTEX
pthread_mutex_t mutex_send_message=PTHREAD_MUTEX_INITIALIZER;
void send_message(char* buf,unsigned int len){
	unsigned int i;
	char crc = 0xFF;
	//int pthread_mutex_trylock(pthread_mutex_t *mutex)
	pthread_mutex_lock(&mutex_send_message);
	// Pushh Magic Number
	while (!pushElement(&message_out_ringbuf,0xEA)) {
		Sleep(1);
	}

	crc -= 0xEA;
	//len
	for(i=0;i<len;i++){
		while (!pushElement(&message_out_ringbuf,buf[i])) {
			Sleep(1);
		}
		crc -= buf[i];
	}

	//push crc
	//printf("Push crc: 0x%X \n",crc);
	while (!pushElement(&message_out_ringbuf,crc)) {
		Sleep(1);
	}
	pthread_mutex_unlock(&mutex_send_message);
	//
}

//
void *send_query_thread(void *arg)
{
	while (1)
	{
		pthread_testcancel();
		char str[2];
		str[0] = OI_OPCODE_QUERY;  // command type id
		str[1] = 0x0;  // len
		send_message(str, sizeof(str));
		Sleep(500);
		//char str[2];
		str[0] = OI_OPCODE_IMUQUERY;  // command type id
		str[1] = 0x0;  // len
		send_message(str, sizeof(str));
		Sleep(500);
	}
}
//----------------------------------

void *thread2(void *arg)
{
		//struct usb_dev_handle* usbhandle = (struct usb_dev_handle*)arg;
		//int MY_EP_Out = 0x01;
		char c;
		int ls = 50;
		puts ("A dot ('.') to exit:");
		do {
		    c=getch();
		    //c='c';
		    //putchar(c);
		    switch (c)
		    {
		    case 'w': //OI_OPCODE_MOTORCONTROL
		    {
		    	char str[6];
		    	int linear_speed = ls;
		    	int angular_speed = 0;
		    	str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		    	str[1] = 0x4;  // len
		    	str[2] = linear_speed & 0xFF;
		    	str[3] = (linear_speed >> 8) & 0xFF;
		    	str[4] = angular_speed & 0xFF;
		    	str[5] = (angular_speed >> 8)& 0xFF;
		    	send_message(str, sizeof(str));
		    }
		    	break;
		    case 's': //OI_OPCODE_MOTORCONTROL stop
		   		    {
		   		    	char str[6];
		   		    	int linear_speed = 0;
		   		    	int angular_speed = 0;
		   		    	str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		   		    	str[1] = 0x4;  // len
		   		    	str[2] = linear_speed & 0xFF;
		   		    	str[3] = (linear_speed >> 8) & 0xFF;
		   		    	str[4] = angular_speed & 0xFF;
		   		    	str[5] = (angular_speed >> 8)& 0xFF;
		   		    	send_message(str, sizeof(str));
		   		    }
		   		    	break;
		    case 'x': //OI_OPCODE_MOTORCONTROL
		    		    {
		    		    	char str[6];
		    		    	int linear_speed = -ls;
		    		    	int angular_speed = 0;
		    		    	str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		    		    	str[1] = 0x4;  // len
		    		    	str[2] = linear_speed & 0xFF;
		    		    	str[3] = (linear_speed >> 8) & 0xFF;
		    		    	str[4] = angular_speed & 0xFF;
		    		    	str[5] = (angular_speed >> 8)& 0xFF;
		    		    	send_message(str, sizeof(str));
		    		    }
		    		    break;
		    case '+':
		    				ls = ls+10;
		    				if (ls > 100) ls = 100;
		    				printf("ls=%d\n",ls);
		   		    		    break;
		    case '-':
		 		    		ls = ls-10;
		 		    		if (ls < 0) ls = 0;
		 		    		printf("ls=%d\n",ls);
		 		   		    break;

		    case 'b': //OI_OPCODE_QUERY
		    {
		    	char str[2];
		    	str[0] = OI_OPCODE_QUERY;  // command type id
		    	str[1] = 0x0;  // len
		    	send_message(str, sizeof(str));
		    }
		    	break;
		    case 'c': //OI_OPCODE_IMUQUERY
		   	{
		   		char str[2];
		   		str[0] = OI_OPCODE_IMUQUERY;  // command type id
		   		str[1] = 0x0;  // len
		   		send_message(str, sizeof(str));
		   	}
		   		break;

		    default:
		    	send_message(&c,1);
		    	break;
		    }
		    Sleep(50);
		} while (c != '.');
		printf("Do Cancel\n");
		pthread_cancel(process_msg_in_handle);
		pthread_cancel(msg_read_handle);
		pthread_cancel(msg_write_handle);
		pthread_cancel(Tid1);
		pthread_exit(NULL);
		return NULL;
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
	// read HID data and push into message_in_ringbuf
	pthread_create(&msg_read_handle, &attr, message_read,  (void*)usbhandle);
	pthread_create(&msg_write_handle, &attr, message_write,  (void*)usbhandle);
	// Process message_in_ringbuf and parse command
	pthread_create(&process_msg_in_handle, &attr, process_msg_in,  (void*)&message_in_ringbuf);

	pthread_create(&Tid2, &attr, thread2,  (void*)usbhandle);

	pthread_create(&Tid1, &attr, send_query_thread,  (void*)usbhandle);

	void *ret;
	pthread_join(process_msg_in_handle, &ret);
	pthread_join(msg_read_handle, &ret);
	pthread_join(msg_write_handle, &ret);
	pthread_join(Tid1, &ret);
	pthread_join(Tid2, &ret);
	printf("Tid2 finish\n");
	pthread_attr_destroy(&attr);
	//pthread_kill(Tid1, 9); //SIGKILL
	puts("Finish");
	usbhid_stop(usbhandle,0);
	pthread_mutex_destroy(&mutex_send_message);
	system("pause");
	return EXIT_SUCCESS;
}
