#include "sensor.h"

int le_sensor(){
    int n = 10;
    unsigned int value = 0;
    while( n != 0){
        value += analogRead(SENSOR);
        n--;    
    }
    value /= 10;   
    return map(value,0,4095,0,100);
}
