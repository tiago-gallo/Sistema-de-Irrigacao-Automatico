/*
Trabalho final - Sistema-de-Irrigacao-Automatico

Autores:      Tiago Gallo Faria - RA200014441
              Danillo Lins      - RA200013897
              Lucas Ramalho     - RA080042306
Professor:    Fabio Souza
Disciplina:   Sistemas Operacionais e RTOS de Sistemas Embarcados Instituição
Instituição:  Centro Universitario Salesiano de São Paulo
*/


#include "main.h"

char valvula_recebido = 0;
char valvula_recebido_last = 0;
int sensor_higrometro_recebido = 0;
char mensagem_valvula[30];
char mensagem_sensor[30];
int delay_sensor = 0;


void setup() {
  Serial.begin(9600);
  rtosInit();
  displayInit();  //inicia display
  init_bt();
  init_valvula();
  mqttInit();
}

void loop() {
  
        valvula_recebido = retorno_valvula();
        delay_sensor++;
        if (valvula_recebido != valvula_recebido_last){
          mqttIsConected();
          sprintf(mensagem_valvula, "%d", valvula_recebido);
          mqttSend_valvulaState(mensagem_valvula);
          vTaskDelay(pdMS_TO_TICKS(1000));
          valvula_recebido_last = valvula_recebido;
        }

        if(delay_sensor > 150){
          sensor_higrometro_recebido = retorno_sensor();
          mqttIsConected();
          sprintf(mensagem_sensor, "%d", sensor_higrometro_recebido);
          mqttSend_sensor(mensagem_sensor);
          delay_sensor = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
}