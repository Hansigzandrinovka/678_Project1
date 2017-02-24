/*
 * FILE		: Job_Queue.c
 *
 * BRIEF	: Generates functions for operating on an Job_Queue data
 * 		  structure, which holds Jobs, defined in Job_Queue.h
 */

#include "Job_Queue.h"

/*
 *Use the prototype macro to place all the int_queue related functions here
 */
IMPLEMENT_DEQUE (Job_Queue, Job);
