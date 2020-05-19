#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <unistd.h>

#include <hal_defs.h>
#include <ihal.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>
#include <hih8120.h>
#include "HumTempSensor.h"



void task1( void *pvParameters );
void task2( void *pvParameters );
HumTempSensor_t HtSensor;
hih8120DriverReturnCode_t rc;
float humidity = 0.0;
float temperature = 0.0;


/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{

	xTaskCreate(task1, (const portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );

	xTaskCreate(task2, (const portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
}

/*-----------------------------------------------------------*/
void task1( void *pvParameters )
{
	#if (configUSE_APPLICATION_TASK_TAG == 1)
	// Set task no to be used for tracing with R2R-Network
	vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	#endif

	for(;;)
	{
		puts("Task1");
		PORTA ^= _BV(PA6);
		vTaskDelay(900);
	}
}

/*-----------------------------------------------------------*/
void task2( void *pvParameters )
{
	#if (configUSE_APPLICATION_TASK_TAG == 1)
	vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
	#endif


	for(;;)
	{
		puts("measuring temperature");
		temphum_meassure(HtSensor);
		temperature = tempget_measurment(HtSensor);
		humidity = humget_measurment(HtSensor);
		PORTA ^= _BV(PA5);
		vTaskDelay(1000);
	}
}

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	DDRA |= _BV(DDA6) | _BV(DDA5); 
	HtSensor = humptempsensor_create();
	rc = inithtdrivers(HtSensor);
	//rc = hih8120Meassure();
	// Initialise the trace-driver to be used together with the R2R-Network
	trace_init();
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdioCreate(ser_USART0);
	// Let's create some tasks
	create_tasks_and_semaphores();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Initialise the HAL layer and use 5 for LED driver priority
	//hal_create(5);
	// Initialise the LoRaWAN driver without down-link buffer
	//lora_driver_create(LORA_USART, NULL);
	// Create LoRaWAN task and start it up with priority 3
	//lora_handler_create(3);
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem();
	printf("Program Started!!\n");
	vTaskStartScheduler();
	
}

