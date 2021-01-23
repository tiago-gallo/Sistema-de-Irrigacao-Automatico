#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void displayInit();
void SensorDisplayManual(int valor);
void SensorDisplayAutomatico(int valor)


#endif
