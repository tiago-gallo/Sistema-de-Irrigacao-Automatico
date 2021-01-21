#include "display.h"


void displayInit(){
  lcd.begin();
  lcd.backlight();
  lcd.clear();
}

void SensorDisplayManual(int valor){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Sist. Manual");
    display.setCursor(0, 1);
    /*display.println(String(valor));*/
    display.display(); 
}

void SensorDisplayAutomatico(int valor){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Sist. Automat.");
    display.setCursor(0, 1);
    /*display.println(String(valor));*/
    display.display(); 
}