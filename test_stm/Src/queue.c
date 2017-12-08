#include <queue.h>
#include <stdio.h>

void queueInit(queue_t *q)
{
	q->head = 0;
	q->tail = 0;
	q->len = 0;
}
unsigned int pushQueueElement(queue_t *q, queue_element_t elem)
{
	if (q->len == NUM_INPUT_QUEUES){
		printf("pushQueueElement overflow\r\n");
		return 0; // queue overflow
	}
	//q->queue[q->head] = elem;
	//q->queue[q->head].data
	q->queue[q->head].len = elem.len;
	int i;
	for(i=0;i<q->queue[q->head].len;i++){
		q->queue[q->head].data[i] = elem.data[i];
	}

	q->head = (q->head + 1) & (NUM_INPUT_QUEUES - 1);
	return ++q->len;
}
//return tail, -1 means no element;
unsigned int getNextQueueData(queue_t *q, queue_element_t **e)
{
	//unsigned char tail = q->tail;
	if (q->len >0) {
		*e= &q->queue[q->tail];
		q->tail = (q->tail+1) & (NUM_INPUT_QUEUES - 1);
		q->len--;
		return q->tail;
	}
	//printf("getNextQueueData none\r\n");
	*e = 0;
	return -1;
}
