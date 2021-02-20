#ifndef _DISPLAY_H_
#define _DISPLAY_H_

    #include <Arduino.h>

    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>
    #include <sensor.h>

    void displayInit();
    void displayImprimeAutomatico(int valorD);
    void displayImprimeManual(int valorD);

#endif //_MAIN_H_
