#include <ringbuf.h>
void ringbufInit(ringbuf_t *r)
{
	r->head = 0;
	r->tail = 0;
	r->len = 0;
}

#include <stdio.h>
//return length, 0 means full can't push data in
unsigned int pushElement(ringbuf_t *r, unsigned char data)
{

	if (r->len == RINGBUF_MAX_LENGTH)  {
		//printf("ringbuf: pushElement overflow\n");
		return 0; // ring buffer overflow
	}
	r->data[r->head] = data;
	r->head = (r->head + 1) & (RINGBUF_MAX_LENGTH - 1);
	//printf("ringbuf: pushElement len=%d\n",r->len+1);
	return ++r->len;
}

//return tail, -1 means no pdata;
unsigned int getNextData(ringbuf_t *r, unsigned char **ppdata)
{
	//unsigned char tail = r->tail;
	if (r->len >0) {
		*ppdata= &r->data[r->tail];
		r->tail = (r->tail + 1) & (RINGBUF_MAX_LENGTH - 1);
		r->len--;
		return r->tail;
	}
	*ppdata = 0;
	//printf("getNextData: pdata=0\n");
	return -1;
}
