#include <display.h>
#include <sensor.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void displayInit(){
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void displayImprimeAutomatico(int valorD){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Automatico");
    lcd.setCursor(0,1);
    lcd.print("% umidade:");
    lcd.setCursor(10,1);
    lcd.print(valorD);
    lcd.print("%");
    delay(500);
}

void displayImprimeManual(int valorD){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Manual");
    lcd.setCursor(0,1);
    lcd.print("% umidade:");
    lcd.setCursor(10,1);
    lcd.println(valorD);
    delay(500);
}