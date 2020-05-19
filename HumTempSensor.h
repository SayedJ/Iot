#pragma once
#include <hih8120.h>

typedef struct HumTempSensor* HumTempSensor_t;


HumTempSensor_t humptempsensor_create();

hih8120DriverReturnCode_t inithtdrivers(HumTempSensor_t self);

void tempsensor_destroy(HumTempSensor_t self);

hih8120DriverReturnCode_t temphum_meassure(HumTempSensor_t self);

float tempget_measurment(HumTempSensor_t self);

float humget_measurment(HumTempSensor_t self);

float tempget_measurment(HumTempSensor_t self);