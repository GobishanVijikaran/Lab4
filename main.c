#include <cmsis_os2.h> 
#include <stdio.h> 
#include <lpc17xx.h> 
#include "random.h"  
#include <stdbool.h>

// Initializing Message Queues
osMessageQueueId_t queue1;
osMessageQueueId_t queue2;

// Initializing variables that will hold stats for queue 1
int q1Overflow, q1Put, q1Received = 0;

// Initializing variables that will hold stats for queue 2
int q2Overflow, q2Put, q2Received = 0;

// Initializing variables for print statements
int seconds = 0, previous_seconds = 0;

void client(void *arg){
	// wait a certain period of time and then send a message to one of the two queues, alternating 
	// between them. Repeats indefinitely
	int currentQueue = 1;
	int messageQueuePut;
	
	// infinite polling
	while(true){
		// waits random period of time (next_event() from random.c). 9hz is the average arrival rate at each (so 18 for 2 queues)
		// to use next_event () need to convert seconds to ticks and divide by the average rate and then right shift by 16
		osDelay((next_event()* osKernelGetTickFreq() / (9*2)) >> 16);
		int message = 57;
		
		if(currentQueue == 1){
			// sending int message to the queue using try semantics (args: the queue, message, priority and timeout)
			messageQueuePut = osMessageQueuePut(queue1, &message, 0, 0);
			currentQueue = 2;
		}
		else if (currentQueue == 2){
			messageQueuePut = osMessageQueuePut(queue2, &message, 0, 0);
			currentQueue = 1;
		}
			
			// for osMessageQueuePut() returns osErrorResource if there is not enough space 
		if(messageQueuePut == osErrorResource) {
				if(currentQueue == 1){
					q1Overflow ++;
				} else {
				  q2Overflow ++;
			}
		}
		
		else if (messageQueuePut == osOK) { 
			if (currentQueue == 1) {
				q1Put++;
			}
			else {
				q2Put++;
			}
			osThreadYield();//pass control to next thread	
		}
	}
} 

void server(void *arg){
	// start by determing which queue it's watching then wait for a random period of time before 
	// retrieving a message from its queue. Repeats indefinitely
	
	int currentQueue = (int)arg; 
	int message = 23; // message to put in queue 
	
	//infinite polling 
	while(true){
		// waits for queue service based on average queue service rate (10hz) & random period of time 
		osDelay((next_event()*osKernelGetTickFreq()/10) >> 16); 
		
		if(currentQueue == 1){
			
			// receive a message from a queue with blocking use osMessageQueueGet() (args: queue, message, buffer to record the priority of message, timeout)
			// logical statement: if received successfully (osOK) , received ++ 
			if((osMessageQueueGet(queue1, &message, NULL, osWaitForever) == osOK)){
				q1Received++; 
			}
		}
		
		else{
			if ((osMessageQueueGet(queue2, &message, NULL, osWaitForever) == osOK)){
				q2Received++; 
			}
		}		
		// pass control of thread
		osThreadYield(); 
	}
}

void monitor_update_time(void *arg)  //change variable names
{
	while(true)
	{
		seconds++;
		osDelay(osKernelGetTickFreq()); //delay by 1 second (simulated)
	}
}

void monitor (void *arg) {
	double tick_freq = osKernelGetTickFreq();
	while(true){
		if(seconds>previous_seconds) //will run every 1 simulated second
		{
			previous_seconds=seconds;
			printf("# of Messages Sent for Queue 1: %d\n", q1Put);
			printf("# of Messages Sent for Queue 2: %d\n", q2Put);
			printf("\n");
			printf("# of messages received successfully for Queue 1: %d\n", q1Received); 
			printf("# of messages received successfully for Queue 2: %d\n", q2Received); 
			printf("\n");
			printf("# of Overflows for Queue 1: %d\n", q1Overflow);
			printf("# of Overflows for Queue 2: %d\n", q2Overflow);
			printf("\n");
			
			printf("Timed elapsed: %d \n", osKernelGetTickCount()/20);
			osDelay(osKernelGetTickFreq());
		}
	}
}

int main(){
	// init the Kernel
	osKernelInitialize(); 
	
	// init queues 
	queue1 = osMessageQueueNew(10, sizeof(int), NULL);
	queue2 = osMessageQueueNew(10, sizeof(int), NULL);
	
	// init threads
	osThreadNew(client, NULL, NULL);
	osThreadNew(server, (void*)1, NULL); 
	osThreadNew(server, (void*)2, NULL);
	osThreadNew(monitor, NULL, NULL);
	osThreadNew(monitor_update_time,NULL,NULL);
	
	// start the Kernel
	osKernelStart(); 
}
