#include <cmsis_os2.h> 
#include <stdio.h> 
#include <lpc17xx.h> 
#include "random.h" 
#include "lfsr113.h" 

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