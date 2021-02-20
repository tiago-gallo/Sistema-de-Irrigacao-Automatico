#ifndef _RTOS_H_
#define _RTOS_H_

#include "mqtt.h"
#include "sensor.h"
#include "display.h"
#include "saida.h"
#include "main.h"

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


void rtosInit();
char retorno_btn_state();
int retorno_sensor();

#endif
