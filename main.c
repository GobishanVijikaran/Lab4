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

void client(void *arg){
	// wait a certain period of time and then send a message to one of the two queues, alternating 
	// between them. Repeats indefinitely
	int currentQueue = 1;
	int messageQueuePut;
	
	while(true){
		osDelay((next_event()* osKernelGetTickFreq() / (9*2)) >> 16);
		int message = 57;
		
		if(currentQueue == 1){
			messageQueuePut = osMessageQueuePut(queue1, &message, 0, 0);
			
			if(messageQueuePut == osErrorResource) {
				q1Overflow += 1;
			} else {
				q1Put += 1;
			}
			
			currentQueue = 2;
		}
		else {
			messageQueuePut = osMessageQueuePut(queue2, &message, 0, 0);
			
			if(messageQueuePut == osErrorResource) {
				q2Overflow += 1;
			} else {
				q2Put += 1;
			}
			
			currentQueue = 1;
		}
}

void server(void *arg){
	// start by determing which queue it's watching then wait for a random period of time before 
	// retrieving a message from its queue. Repeats indefinitely
}

void monitor(void *arg){
	// runs once a second and should print the elasped time in seconds as well as other info 
	
	// total messages sent successfully 
	// total messages received 
	// total overflows (when osMessageQueuePut returns osErrorResource
}

int main(){
	// init the Kernel
	osKernelInitialize(); 
	
	// init queues 
	
	// init threads
	osThreadNew(client, NULL, NULL); 
	osThreadNew(server, NULL, NULL); 
	osThreadNew(server, NULL, NULL); 
	osThreadNew(monitor, NULL, NULL); 
	
	// start the Kernel
	osKernelStart(); 
}