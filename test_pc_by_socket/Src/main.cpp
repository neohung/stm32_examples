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

#include <pthread.h>

#include <ringbuf.h>
unsigned char Buffer[128];
ringbuf_t message_in_ringbuf;
ringbuf_t message_out_ringbuf;

pthread_t msg_read_handle, process_msg_in_handle, msg_write_handle, Tid2, Tid1;

void *message_write(void *arg)
{
	//Sleep(10);
	int s;
	s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (s != 0){
		printf("pthread_setcancelstate error\n");
		exit(EXIT_FAILURE);
	}
	SOCKET* psConnect = (SOCKET*)arg;
	while (1)
	{
		//printf("message_write\n");
		pthread_testcancel();
		unsigned char* pdata;
		//unsigned int tail =
				getNextData(&message_out_ringbuf, &pdata);
		if (pdata){
	    		//printf("write data[%d]=[%c]\n",(tail-1)&(RINGBUF_MAX_LENGTH-1),*pdata);
	    		//usbhid_send(usbhandle, MY_EP_Out, (char*)pdata, 1);
			send(*psConnect, (char*)pdata, 1, 0);
		}
		Sleep(1);
	}
	//	closesocket(sConnect);
}

void *message_read(void *arg)
{
	 int s;
	 s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	 if (s != 0){
		 printf("pthread_setcancelstate error\n");
		 exit(EXIT_FAILURE);
	 }
	 SOCKET* psConnect = (SOCKET*)arg;
	//struct usb_dev_handle* usbhandle = (struct usb_dev_handle*)arg;
	//int MY_EP_In = 0x81;
	char buf[255];
    while (1)
    {
    	//printf("message_read\n");
    	//set cancel interrupt point
    	pthread_testcancel();
        //int len = usbhid_read(usbhandle, MY_EP_In, buf, 1);
        int len = recv(*psConnect, (char*)buf, sizeof(buf), 0);
        if (len > 0){
        	//printf("message_read: len=%d\n",len);
        	int i;
        	for (i=0;i<len;i++){
        	  //printf("Push [0x%X] into message_in_ringbuf\n", (unsigned char)buf[i]);
        	  unsigned buflen = pushElement(&message_in_ringbuf,buf[i]);
        	  if (buflen==0){
        		printf("Full, can't push to message_in_ringbuf any more\n");
        		break;
        	  }
        	}
        }
        Sleep(1);
    }
    //pthread_exit((void *)1234);
    return NULL;
}

unsigned char polling_data(ringbuf_t* pmsg_in)
{
	unsigned char* pdata;
	//unsigned int tail;
	while(1){
    	pthread_testcancel();
		//tail =
		getNextData(pmsg_in, &pdata);
	    if (pdata){
	    		//printf("data[%d]=0x%X\n",(tail-1)&(RINGBUF_MAX_LENGTH-1),*pdata);
	    		break;
	    }else{
	    	Sleep(10);
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
	 ringbuf_t* pmsg_in = (ringbuf_t*)arg;
    while (1)
    {
    	//set cancel interrupt point
    	pthread_testcancel();
    	char checksum = 0xFF;
    	do{
    		//Sleep(1);
    	}while(polling_data(pmsg_in) != 0xEA);
    	//printf("polling_data ==0xEA\n");
    	checksum -= 0xEA;
    	unsigned char Command_type_id = polling_data(pmsg_in);
    	checksum -= Command_type_id;
    	//printf("Command_type_id: %d\r\n",Command_type_id);
    	unsigned char len = polling_data(pmsg_in);
    	checksum -= len;
    	// Check len if make sense
    	if (len > 16){
    		printf("Check len=%d error, should not longer than 16, skip this\r\n", len);
    		continue;
    	}
    	//printf("len: %d\r\n",len);
    	int i;
    	for (i=0;i<len;i++){
    		param[i] = polling_data(pmsg_in);
    		checksum -= param[i];
    	}
    	char crc = polling_data(pmsg_in);
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
	pthread_exit(NULL);
	return NULL;
}
//----------------------------------

void *thread1(void *arg)
{
	 SOCKET* psConnect = (SOCKET*)arg;
		//struct usb_dev_handle* usbhandle = (struct usb_dev_handle*)arg;
		//int MY_EP_Out = 0x01;
		char c;
		int ls = 50;
		int as = 50;
		char dir = 0;
		char adir = 0;
		short linear_speed = 0;
		short angular_speed = 0;
		puts ("A dot ('.') to exit:");
		do {
		    c=getch();
		    //putchar(c);
		    switch (c)
		    {
		      case 'w': //OI_OPCODE_MOTORCONTROL
		      {
		    	dir = 1;
		    	adir = 0;
		    	char str[6];
		    	linear_speed = dir*ls;
		    	printf("linear_speed=0x%X\n",linear_speed);
		    	angular_speed = 0;
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
		   		dir = 0;
		   		adir = 0;
		   		char str[6];
		   		linear_speed = 0;
		   		angular_speed = 0;
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
		    	dir = -1;
		        adir = 0;
		    	char str[6];
		        linear_speed = dir*ls;
		        printf("linear_speed=0x%X\n",linear_speed);
		        angular_speed = 0;
		        str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		    	str[1] = 0x4;  // len
		        str[2] = linear_speed & 0xFF;
		    	str[3] = (linear_speed >> 8) & 0xFF;
		        str[4] = angular_speed & 0xFF;
		    	str[5] = (angular_speed >> 8)& 0xFF;
		    	send_message(str, sizeof(str));
		      }
		    	break;
		      case 'a':
		   	  {
		   		dir = 0;
		   		adir = 1;
		   		char str[6];
		   		linear_speed = 0;
		   		angular_speed = adir*as;
		   		printf("angular_speed=0x%X\n",angular_speed);
		   		str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		   		str[1] = 0x4;  // len
		   		str[2] = linear_speed & 0xFF;
		   		str[3] = (linear_speed >> 8) & 0xFF;
		   		str[4] = angular_speed & 0xFF;
		   		str[5] = (angular_speed >> 8)& 0xFF;
		   		send_message(str, sizeof(str));
		   	  }
		   		break;
		      case 'd':
		 	  {
		 	    dir = 0;
		 	    adir = -1;
		 	    char str[6];
		 	    linear_speed = 0;
		 		angular_speed = adir*as;
		 		printf("angular_speed=0x%X\n",angular_speed);
		 		str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		 		str[1] = 0x4;  // len
		 		str[2] = linear_speed & 0xFF;
		 		str[3] = (linear_speed >> 8) & 0xFF;
		 		str[4] = angular_speed & 0xFF;
		 		str[5] = (angular_speed >> 8)& 0xFF;
		 		send_message(str, sizeof(str));
		 	   }
		 		break;
		       case '=':
		       case '+':
		       {
		    	ls = ls+10;
		        if (ls > 100) ls = 100;
		    	as = as+10;
		    	if (as > 100) as = 100;
		    	char str[6];
		    	linear_speed = dir*ls;
		    	angular_speed = adir*as;
		    	str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		    	str[1] = 0x4;  // len
		    	str[2] = linear_speed & 0xFF;
		    	str[3] = (linear_speed >> 8) & 0xFF;
		    	str[4] = angular_speed & 0xFF;
		    	str[5] = (angular_speed >> 8)& 0xFF;
		    	send_message(str, sizeof(str));
		       }
		   		break;
		       case '-':
		       {
		 		ls = ls-10;
		 		if (ls < 0) ls = 0;
		 		as = as-10;
		 		if (as < 0) as = 0;
		 		char str[6];
		 		linear_speed = dir*ls;
		 		angular_speed = adir*as;
		 		str[0] = OI_OPCODE_MOTORCONTROL;  // command type id
		 		str[1] = 0x4;  // len
		 		str[2] = linear_speed & 0xFF;
		 		str[3] = (linear_speed >> 8) & 0xFF;
		 		str[4] = angular_speed & 0xFF;
		 		str[5] = (angular_speed >> 8)& 0xFF;
		 		send_message(str, sizeof(str));
		       }
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
		    case 'z': //OI_OPCODE_IMUQUERY
		    {
		      printf("You press z\n");
		  	  char str[6] = "abcde";
		  	  send(*psConnect, (char*)str, 6, 0);
		  	  //send_message(str, sizeof(str));
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
		pthread_cancel(Tid2);
		pthread_exit(NULL);
		return NULL;
}

SOCKET to_connection(void)
{
	printf("to_connection\n");
		Sleep(10);
#ifdef WIN32
	//----------------- Init WS2 DLL -----------------
	WSAData wsaData;
	//WORD version = MAKEWORD(2, 2);
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
	  printf("Init WS2 fail\n");
	  return 0;
	}
#endif
		//Do connection
		SOCKADDR_IN addr;
		SOCKET sConnect;
		sConnect = socket(AF_INET, SOCK_STREAM, 0);
		//set the socket I/O mode
		//if iMode = 0, blocking is enabled
		//if iMode != 0, non-blocking mode is enabled
		u_long iMode = 1;
		ioctlsocket(sConnect, FIONBIO, &iMode);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		//addr.sin_addr.s_addr = inet_addr("192.168.31.185");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(23);
		int ret;
		do {
		  ret = connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
		  if (ret){
			if( WSAGetLastError() == WSAEWOULDBLOCK){
				printf("Attempting to connect. %d\n",ret);
				ret = 0;
			}else{
				 Sleep(1000);
				 printf("Failed to connect to server.\n");
				 printf("Error: %d\n",WSAGetLastError());
				 WSACleanup();
			}
		  }
		}while(ret == SOCKET_ERROR);
		printf("connected, ret = %d\n",ret);
		return sConnect;
}

//----------------------------------------------------------------------
//#ifdef MINGW32
#ifdef WIN32
//#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
//#else
//#include <arpa/inet.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <netinet/in.h>
//#include <signal.h>
//#include <sys/socket.h>
//#include <unistd.h>
#endif
pthread_t server_tid;
void *socketserver_thread(void *arg)
{
	printf("socketserver_thread\n");
	 int* pPORT = (int*)arg;
	 int PORT = *pPORT;
#ifdef WIN32
	//----------------- Init WS2 DLL -----------------
	WSAData wsaData;
	//WORD version = MAKEWORD(2, 2);
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
	  printf("Init WS2 fail\n");
	  return NULL;
	}
#endif
	//-----------------  Create Socket ----------------
	SOCKET sListen = INVALID_SOCKET;
	sListen= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen== INVALID_SOCKET) {
	  printf("Create Socket fail\n");
	  return NULL;
	}
	//-----------------  Set SOCKADDR_IN and PORT ----------------
	SOCKADDR_IN addr;
	memset (&addr, 0, sizeof (addr)) ;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	//-----------------  BIND ----------------
	int r = bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	if (r == SOCKET_ERROR){
	  printf("Bind Port %d fail\n",PORT);
	  return NULL;
	}
	//-----------------  LISTEN ----------------
	r = listen(sListen, SOMAXCONN);
	if (r == SOCKET_ERROR){
	  printf("Listen Port %d fail\n",PORT);
	  return NULL;
	}
	SOCKET sConnect;
	struct sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	printf("Server Ready to got accetp\n");
	sConnect = accept(sListen, (SOCKADDR*)&clientAddr, &clientAddrLen);
	printf("Server got accetp\n");
		if (sConnect != INVALID_SOCKET)
		{
		  printf("server: got connection from %s\n", inet_ntoa(clientAddr.sin_addr));
		}
	//
	while(1){
		pthread_testcancel();
		unsigned char message[200];
		ZeroMemory(message, 200);
		//int n =
		recv(sConnect, (char*)message, sizeof(message), 0);
		/*
		if (n > 0){
		  int i;
		  printf("Size %d\n[ ",n);
		  for (i=0;i<n;i++)
		    printf("0x%X ",message[i]);
		  printf(" ]\n");
		}
		*/
		//(message[0] == 0xEA)
		if ((message[0] == OI_OPCODE_QUERY) && (message[1] == 0x0)&& (message[2] == 0x12))
		{
		  //printf("It is ODOM Query command\n");
		  int x = 0x0001; //mm
		  int y = 0x0002; //mm
		  int theta = 0x0003; //degree 0~360
		  int linear_speed = 0x0004; //mm
		  int angular_speed = 0x0005; //mm
		  message[0] = 0xEA;
		  message[1] = OI_OPCODE_ODOMUPDATE;
		  message[2] = 10; //len
		  message[3] = x & 0xFF;
		  message[4] = (x >> 8) & 0xFF;
		  message[5] = y & 0xFF;
		  message[6] = (y >> 8)& 0xFF;
		  message[7] = theta & 0xFF;
		  message[8] = (theta >> 8) & 0xFF;
		  message[9] = linear_speed & 0xFF;
		  message[10] = (linear_speed >> 8) & 0xFF;
		  message[11] = angular_speed & 0xFF;
		  message[12] = (angular_speed >> 8)& 0xFF;
		  int i=0;
		  char crc=0xFF;
		  for (i=0;i<13;i++){
			  crc -=  message[i];
		  }
		  message[13] = crc;
		  send(sConnect, (char*)message, 14,0);
		}
		if ((message[0] == OI_OPCODE_IMUQUERY) && (message[1] == 0x0)&& (message[2] == 0x11))
		{
			//printf("It is IMU Query command\n");
			int yaw = 0x0001; //-180~180
			int pitch = 0x0002; //-180~180
			int roll = 0x0003; //-180~180
			message[0] = 0xEA;
			message[1] = OI_OPCODE_IMUSTATE;  // command type id
			message[2] = 6;  // len
			message[3] = yaw & 0xFF;
			message[4] = (yaw >> 8) & 0xFF;
			message[5] = pitch & 0xFF;
			message[6] = (pitch >> 8)& 0xFF;
			message[7] = roll & 0xFF;
			message[8] = (roll >> 8)& 0xFF;
			int i=0;
			char crc=0xFF;
			for (i=0;i<9;i++){
			  crc -=  message[i];
			}
			message[9] = crc;
			send(sConnect, (char*)message, 10,0);
		}
		Sleep(10);
	}
}
//----------------------------------------------------------------------

int main(void) {
	 ringbufInit(&message_in_ringbuf);
	 ringbufInit(&message_out_ringbuf);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//Server Emu
	int PORT=23;
	pthread_create(&server_tid, &attr, socketserver_thread, (void*)&PORT);
	//--------------------
	//
	//Process Connection
	SOCKET s = to_connection();
	//
	pthread_create(&Tid1, &attr, thread1,  &s);
	pthread_create(&msg_write_handle, &attr, message_write,  &s);
	pthread_create(&msg_read_handle, &attr, message_read,  &s);
	pthread_create(&process_msg_in_handle, &attr, process_msg_in,  (void*)&message_in_ringbuf);
	//-------------------------------------------------------------------------
	pthread_create(&Tid2, &attr, send_query_thread,  NULL);
	//------------------------------------------------------------------------
	void *ret;
	pthread_join(process_msg_in_handle, &ret);
	pthread_join(msg_read_handle, &ret);
	pthread_join(msg_write_handle, &ret);
	pthread_join(Tid2, &ret);
	pthread_join(Tid1, &ret);
	printf("Tid1 finish\n");
	pthread_attr_destroy(&attr);
	//pthread_kill(Tid1, 9); //SIGKILL
	puts("Finish");
	pthread_mutex_destroy(&mutex_send_message);
	system("pause");
	return EXIT_SUCCESS;
}
