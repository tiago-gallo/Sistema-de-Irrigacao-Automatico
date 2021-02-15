/*
Task             Core  Prio     Descrição
-------------------------------------------------------------------------------
vTaskBotao         1     1     Faz a Leitura do estado do botão
vTaskPrint         1     1     Imprime o valor do sensor o display/Broker
vTaskMQTTReceive   0     2     Recebe dados do MQTT
vTaskValvula       0     1     Faz o acionamento da eletroválvula


1- Sensor (timer) - ok
2- Task display + envia Mqtt (estado da valvula + Leitura do sensor)
3- Task Botão - ok
4- Task Mqtt Recebimento do Boker (acinamento pelo broker da valvula) - ok
5- Task Acionamento da válvula - ok 
*/

#include <Arduino.h>

#include "rtos.h"
#include "botao.h"
#include "display.h"
#include "main.h"
#include "mqtt.h"
#include "saida.h"
#include "sensor.h"

unsigned char valvula_state;
unsigned char btn_state;
char mensagem_sensor[30];

//Handle dos timers

TimerHandle_t xTimer_sensor; 

//Handles das Filas
QueueHandle_t xFila; 

//Handle das Tarefas
TaskHandle_t xTaskBotaoHandle;
TaskHandle_t xTaskPrintHandle;
TaskHandle_t xTaskMQTTReceiveHandle;
TaskHandle_t xTaskValvulaHandle;



//Funcoes
void callback_valvula(char* topic, byte* payload, unsigned int length);


//Tasks
void vTaskBotao( void *pvParameters);
void vTaskPrint(void *pvParameters);
void vTaskMQTTReceive(void *pvParameters); 
void vTaskValvula(void *pvParameters);


//Timers
void callBackTimer_sensor(TimerHandle_t pxTimer );

void rtosInit(){


    //Criação Fila
    xFila = xQueueCreate(3, sizeof(int));

    //Timers com auto-reload
    xTimer_sensor = xTimerCreate("Timer_sensor", pdMS_TO_TICKS(15000), pdTRUE, 0, callBackTimer_sensor);
    if( xTimer_sensor == NULL )
    {
      Serial.println("Não foi possível criar o timer_sensor");
      while(1);
    }
    else{
      Serial.println("Timer_sensor Criado");
    }

    //Tarefas
    xTaskCreatePinnedToCore(vTaskBotao,  "TaskBotao",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskBotaoHandle,APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTaskPrint,  "TaskPrint",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskPrintHandle,APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTaskMQTTReceive,  "TaskMQTT",  configMINIMAL_STACK_SIZE + 2048,  NULL,  2,  &xTaskMQTTReceiveHandle,PRO_CPU_NUM);  
    xTaskCreatePinnedToCore(vTaskValvula, "TaskValvula", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskValvulaHandle, PRO_CPU_NUM);
    
    
    xTimerStart(xTimer_sensor,0);
}

void callBackTimer_sensor(TimerHandle_t pxTimer ){
  int adcValue;
  adcValue = le_sensor();
  xQueueOverwrite(xFila, &adcValue);
}


/* Implemntação da vTaskBotão  */
void vTaskBotao(void *pvParameters){
  (void) pvParameters;

  init_bt();

  while(1){
      if(!verifica_bt()){
        btn_state = 1; 
      }
      else btn_state = 0;
      vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/* Implemntação da vTaskDisplay  */
void vTaskPrint(void *pvParameters ){
  (void) pvParameters;  /* Apenas para o Compilador não retornar warnings */
  int valor_recebido = 0;
  while(1){
      if(xQueueReceive(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) {//verifica se há valor na fila para ser lido. Espera 1 segundo
        //imprimeSensorDisplay(valor_recebido);
        mqttIsConected();
        sprintf(mensagem_sensor, "%d", valor_recebido);
        mqttSend_sensor(mensagem_sensor);
      }
  }
}

/*Implementação da Task de Recebimento de dados MQTT */
void vTaskMQTTReceive(void *pvParameters){
  (void) pvParameters;
  mqttInit();
  mqttReceive();
  while(1){
    mqttIsConected();
    mqttLoop();
  }
}

/* Implemntação da vTaskValvula */
void vTaskValvula(void *pvParameters){
  (void) pvParameters;
  init_valvula();
  
  while(1){
    if(valvula_state == 1) {
      acionar_valvula();
      //display
      //MQTTSend_Valvula
    }  
    else {
      desligar_valvula();
      //Display
      //MQTTSend_Valvula
    }
  }
}

void callback_valvula(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Mensagem do broker: ");
  Serial.println(topic);
 
  Serial.print("Estado da valvula:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    valvula_state = payload[i] - 48 ;
  }
  
  Serial.println();
  Serial.println("-----------------------");
 
}