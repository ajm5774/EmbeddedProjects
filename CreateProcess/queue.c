<<<<<<< HEAD
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "queue.h"

int maxDepth = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void enqueue(conQueue *q, Customer * cust)
{
	pthread_mutex_lock(&mutex);
	if (q->count == 0)
	{
		q->first = cust;
		q->last = cust;
	}
	else {
		q->last->behind = cust;
		q->last = cust;
	}
	q->count++;

	if(q->count > q->maxDepth)
	{
		q->maxDepth = q->count;
	}
	pthread_mutex_unlock(&mutex);
}

Customer* dequeue(conQueue *q)
{
	Customer * cust = 0;
	pthread_mutex_lock(&mutex);
	if(q->count == 0)
	{
		pthread_mutex_unlock(&mutex);
		return cust;
	}
	else if (q->count == 1)
	{
		cust = q->first;
		q->first = 0;
		q->last = 0;
	}
	else {
		cust = q->first;
		q->first = q->first->behind;
	}
	q->count--;
	pthread_mutex_unlock(&mutex);
	return cust;
}

void print_queue(conQueue *q)
{
	pthread_mutex_lock(&mutex);
	Customer * cust = q->first;

	while (cust) {
		printf("%d \n", cust->id);
		cust = cust->behind;
	}
	pthread_mutex_unlock(&mutex);
}

// Used for testing code
/*
int main() {
	Customer* cust;
	Customer cust0 = {0, 0, 0, 0};
	Customer cust1 = {1, 0, 0, 0};

	conQueue queue = {0, 0, 0};
	assert (queue.count == 0);

	enqueue(&queue, &cust0);
	enqueue(&queue, &cust1);
	assert (queue.count == 2);

	print_queue(&queue);

	cust = dequeue(&queue);
	assert (cust->id == 0);
	assert (queue.count == 1);
	cust = dequeue(&queue);
	assert (cust->id == 1);
	assert (queue.count == 0);
}
*/

=======
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "queue.h"

int maxDepth = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void enqueue(conQueue *q, Customer * cust)
{
	pthread_mutex_lock(&mutex);
	if (q->count == 0)
	{
		q->first = cust;
		q->last = cust;
	}
	else {
		q->last->behind = cust;
		q->last = cust;
	}
	q->count++;

	if(q->count > maxDepth)
		maxDepth = q->count;
	pthread_mutex_unlock(&mutex);
}

Customer* dequeue(conQueue *q)
{
	Customer * cust = 0;
	pthread_mutex_lock(&mutex);
	if(q->count == 0)
	{
		pthread_mutex_unlock(&mutex);
		return cust;
	}
	else if (q->count == 1)
	{
		cust = q->first;
		q->first = 0;
		q->last = 0;
	}
	else {
		cust = q->first;
		q->first = q->first->behind;
	}
	q->count--;
	pthread_mutex_unlock(&mutex);
	return cust;
}

void print_queue(conQueue *q)
{
	pthread_mutex_lock(&mutex);
	Customer * cust = q->first;

	while (cust) {
		printf("%d \n", cust->id);
		cust = cust->behind;
	}
	pthread_mutex_unlock(&mutex);
}

// Used for testing code
/*
int main() {
	Customer* cust;
	Customer cust0 = {0, 0, 0, 0};
	Customer cust1 = {1, 0, 0, 0};

	conQueue queue = {0, 0, 0};
	assert (queue.count == 0);

	enqueue(&queue, &cust0);
	enqueue(&queue, &cust1);
	assert (queue.count == 2);

	print_queue(&queue);

	cust = dequeue(&queue);
	assert (cust->id == 0);
	assert (queue.count == 1);
	cust = dequeue(&queue);
	assert (cust->id == 1);
	assert (queue.count == 0);
}
*/

>>>>>>> 7a45798c5ed9dba6cef3f6cc29f864e3972417fd
