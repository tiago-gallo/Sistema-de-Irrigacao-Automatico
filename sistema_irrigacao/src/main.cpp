/*
Trabalho final - Sistema-de-Irrigacao-Automatico

Autores:      Tiago Gallo Faria - RA200014441
              Danillo Lins      - RA200013897
Professor:    Fabio Souza
Disciplina:   Sistemas Operacionais e RTOS de Sistemas Embarcados Instituição
Instituição:  Centro Universitario Salesiano de São Paulo
*/


#include "main.h"

void setup() {
  Serial.begin(9600);
  rtosInit();
  displayInit();  //inicia display
  init_bt();
  init_valvula();
  mqttInit();
}

void loop() {
  
}