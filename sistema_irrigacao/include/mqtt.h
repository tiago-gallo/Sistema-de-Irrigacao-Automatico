#ifndef _MQTT_H_
#define _MQTT_H_


#include <Arduino.h>

void mqttInit();
void mqttIsConected();
void mqttSend_sensor(char mensagem[]);
void mqttSend_valvulaState(char mensagem[]);
void callback_valvula(char* topic, byte* payload, unsigned int length);
void mqttReceive();
void mqttLoop();
void mqttSubcribe();
#endif