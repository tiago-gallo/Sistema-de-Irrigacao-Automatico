#include "saida.h"

void init_valvula(){
    pinMode(LED,OUTPUT);
    pinMode(VALVULA,OUTPUT);
}

void acionar_valvula(){
    digitalWrite(VALVULA,HIGH);
}

void desligar_valvula(){
    digitalWrite(VALVULA,LOW);
}