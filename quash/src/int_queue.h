/*
 * FILE		: pid_queue.h
 *
 * BRIEF	: Generates declarations for deque data structure to store
 * 		  integers
 */

#ifndef __INT_QUEUE_H__
#define __INT_QUEUE_H__

#include "deque.h"

/*
 * Declare the queue data structure
 */
IMPLEMENT_DEQUE_STRUCT (pid_queue, int);

/*
 * Declare a queue for storing process ids (i.e. integers)
 */
PROTOTYPE_DEQUE (pid_queue, int);

#endif /* __INT_QUEUE_H__ */
