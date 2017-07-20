#include <queue.h>

void queueInit(queue_t *q)
{
	q->head = 0;
	q->tail = 0;
	q->len = 0;
}
unsigned int pushQueueElement(queue_t *q, queue_element_t elem)
{
	if (q->len == NUM_INPUT_QUEUES)  return 0; // queue overflow
	q->queue[q->head] = elem;
	q->head = ++q->head & (NUM_INPUT_QUEUES - 1);
	return ++q->len;
}
//return tail, -1 means no element;
unsigned int getNextQueueData(queue_t *q, queue_element_t **e)
{
	unsigned char tail = q->tail;
	if (q->len >0) {
		*e= &q->queue[q->tail];
		q->tail = ++q->tail & (NUM_INPUT_QUEUES - 1);
		q->len--;
		return q->tail;
	}
	*e = 0;
	return -1;
}