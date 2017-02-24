/*
 * FILE		: Job_Queue.h
 *
 * BRIEF	: Generates declarations for deque data structure to store
 * 		  various integers, including Job ID and Process ID
 */

#ifndef __JOB_QUEUE_H__
#define __JOB_QUEUE_H__

#include "deque.h"
#include "pid_queue.h"

/*
 * Declare the queue data structure
 */
 
 typedef struct Job{
	int job_id; //used to identify current job. When added to background queue, assigned id of 1 + id of last job on queue
	int is_background_job; //int representation of boolean
	pid_queue process_queue; //the processes running as part of the job (either foreground, so wait to finish... or background, so monitor them)
} Job;
IMPLEMENT_DEQUE_STRUCT (Job_Queue, Job);

/*
 * Declare a queue for storing job data (defined type Job)
 */
PROTOTYPE_DEQUE (Job_Queue, Job);

#endif /* __JOB_QUEUE_H__ */
