/*----------------------------------------------------------------------
MIT License

Copyright (c) 2021 bbuhrow

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
----------------------------------------------------------------------*/

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue_t * newQueue(uint32_t sz)
{
	Queue_t *Q = (Queue_t *)malloc(sizeof(Queue_t));
	Q->Q = (uint32_t *)malloc(sz * sizeof(uint32_t));
	Q->sz = sz;
	Q->head = 0;
	Q->tail = 0;
	Q->len = 0;
	return Q;
}

void enqueue(Queue_t *Q, uint32_t e)
{
	Q->Q[Q->tail++] = e;
	Q->len++;

	if (Q->tail == Q->sz)
	{
		Q->tail = 0;
	}

	if (Q->len >= Q->sz)
	{
		printf("warning: Q overflowed\n");
		exit(1);
	}
	return;
}

uint32_t dequeue(Queue_t *Q)
{
	uint32_t e = -1;

	if (Q->len > 0)
	{
		e = Q->Q[Q->head];
		Q->head++;
		if (Q->head == Q->sz)
		{
			Q->head = 0;
		}
		Q->len--;
	}
	else
	{
		printf("warning: attempted to dequeue from an empty queue\n");
	}

	return e;
}

uint32_t peekqueue(Queue_t *Q)
{
	uint32_t e = -1;
	if (Q->len > 0)
	{
		e = Q->Q[Q->head];
	}
	return e;
}

void clearQueue(Queue_t *Q)
{
	free(Q->Q);
	Q->len = 0;
	Q->sz = 0;
	Q->head = 0;
	Q->tail = 0;
}
