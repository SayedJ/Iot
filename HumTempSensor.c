#include <stdlib.h>
#include <string.h>
#include "HumTempSensor.h"
#include <hih8120.h>

typedef struct HumTempSensor {
	float temperature;
	float humidity;
} HumTempSensor;


HumTempSensor_t humptempsensor_create() {
	HumTempSensor_t _newsensor = calloc(sizeof(HumTempSensor), 1);

	if (NULL == _newsensor) {
		return NULL;
	}
	


	return _newsensor;
}


hih8120DriverReturnCode_t inithtdrivers(HumTempSensor_t self) {
	hih8120DriverReturnCode_t rc;
	rc = hih8120Create();
	rc = hih8120Wakeup();
	return rc;
}

void tempsensor_destroy(HumTempSensor_t self) {
	if (NULL != self) {
		free(self);
	}
}

hih8120DriverReturnCode_t temphum_meassure(HumTempSensor_t self) {
	hih8120DriverReturnCode_t rc;
	rc = hih8120Meassure();
	self->temperature = hih8120GetTemperature();
	self->humidity = hih8120GetHumidity();
}


float humget_measurment(HumTempSensor_t self) {
	return self->humidity;
}

float tempget_measurment(HumTempSensor_t self) {
	return self->temperature;
}


