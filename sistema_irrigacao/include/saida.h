#ifndef _SAIDA_H
#define _SAIDA_H

#include <Arduino.h>

#define LED 2
#define VALVULA 12

void init_valvula();
void acionar_valvula();
void desligar_valvula();

#endif