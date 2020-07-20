#include <cmsis_os2.h> 
#include <stdio.h> 
#include <lpc17xx.h> 
#include "random.h" 
#include "lfsr113.h" 

void client(void *arg){
	// wait a random period of time and then send a message to one of the two queues, alternating 
	// between them. Repeats indefinitely
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