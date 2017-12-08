#ifndef RINGBUF_H_
#define RINGBUF_H_

#define RINGBUF_MAX_LENGTH 128

typedef struct __ringbuf_element_t{
    unsigned char data[RINGBUF_MAX_LENGTH];
    unsigned int head;
    unsigned int tail;
    unsigned int len;
} ringbuf_t;


void ringbufInit(ringbuf_t *r);

//return length, 0 means full can't push data in
unsigned int pushElement(ringbuf_t *r, unsigned char data);

//return tail, -1 means no pdata;
unsigned int getNextData(ringbuf_t *r, unsigned char **ppdata);

#endif
