#include <cmsis_os2.h> 
#include <stdio.h> 
#include <lpc17xx.h> 
#include "random.h"  

// Initializing Message Queues
osMessageQueueId_t queue1;
osMessageQueueId_t queue2;

// Initializing variables that will hold stats for queue 1
int q1Overflow, q1Put, q1Received = 0;

// Initializing variables that will hold stats for queue 2
int q2Overflow, q2Put, q2Received = 0;

void client(void *arg){
}

void server(void *arg){
}

void monitor(void *arg){
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