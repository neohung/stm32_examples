#ifndef QUEUE_H_
#define QUEUE_H_
#define NUM_INPUT_QUEUES 16
#define MAX_DATA_ELEMENTS_LENGTH 128

typedef struct __queue_element_t {
	unsigned short len;
	unsigned char data[MAX_DATA_ELEMENTS_LENGTH];
} queue_element_t;
typedef struct __queue_t {
	unsigned char head, tail;
	short len;
	queue_element_t queue[NUM_INPUT_QUEUES];
} queue_t;

void queueInit(queue_t *q);
//return queue length, 0 means full can't push in
unsigned int pushQueueElement(queue_t *q, queue_element_t elem);
//return tail, -1 means no element;
unsigned int getNextQueueData(queue_t *q, queue_element_t **e);

#endif /* QUEUE_H_ */
