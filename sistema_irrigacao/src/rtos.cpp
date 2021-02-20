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

char btn_state = 0;
int sensor_higrometro = 0;
char flag_valvula = 0;

//Handle dos timers

TimerHandle_t xTimer_sensor; 

//Handles das Filas
QueueHandle_t xFila; 

//Handle das Tarefas
TaskHandle_t xTaskBotaoHandle;
TaskHandle_t xTaskPrintHandle;
//TaskHandle_t xTaskMQTTReceiveHandle;
TaskHandle_t xTaskValvulaHandle;



//Funcoes
//void callback_valvula(char* topic, byte* payload, unsigned int length);


//Tasks
void vTaskBotao( void *pvParameters);
void vTaskPrint(void *pvParameters);
//void vTaskMQTTReceive(void *pvParameters); 
void vTaskValvula(void *pvParameters);


//Timers
void callBackTimer_sensor(TimerHandle_t pxTimer );

char retorno_btn_state(){
  return(btn_state);
}

int retorno_sensor(){
  return(sensor_higrometro);
}

void rtosInit(){


    //Criação Fila
    xFila = xQueueCreate(3, sizeof(int));

    //Timers com auto-reload
    xTimer_sensor = xTimerCreate("Timer_sensor", pdMS_TO_TICKS(1000), pdTRUE, 0, callBackTimer_sensor);
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
    xTaskCreatePinnedToCore(vTaskPrint,  "TaskPrint",   configMINIMAL_STACK_SIZE + 2048,  NULL,  2,  &xTaskPrintHandle,PRO_CPU_NUM);
    //xTaskCreatePinnedToCore(vTaskMQTTReceive,  "TaskMQTT",  configMINIMAL_STACK_SIZE + 4096,  NULL,  2,  &xTaskMQTTReceiveHandle,PRO_CPU_NUM);  
    xTaskCreatePinnedToCore(vTaskValvula, "TaskValvula", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTaskValvulaHandle, PRO_CPU_NUM);
    xTimerStart(xTimer_sensor,0);
}

void callBackTimer_sensor(TimerHandle_t pxTimer ){
  int adcValue;
  adcValue = le_sensor();
  xQueueSend(xFila,&adcValue,portMAX_DELAY);
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
      Serial.print("estado do botao: ");
      Serial.println(btn_state);      
      vTaskDelay(pdMS_TO_TICKS(1000));
  }
}


/* Implemntação da vTaskDisplay  */

void vTaskPrint(void *pvParameters ){
  (void) pvParameters;  
  int valor_recebido = 0;
  //char mensagem_sensor[30];

  while(1){
      if(xQueueReceive(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) {//verifica se há valor na fila para ser lido. Espera 1 segundo
        sensor_higrometro = valor_recebido;
        if(btn_state == 1) displayImprimeManual(valor_recebido);
        else displayImprimeAutomatico(valor_recebido);
        if (valor_recebido < 25) flag_valvula = 1;
        else if (valor_recebido > 75) flag_valvula = 0;
        Serial.print("Valor do sensor: ");
        Serial.println(valor_recebido);
      }
  }
}


/*Implementação da Task de Recebimento de dados MQTT */
/*
void vTaskMQTTReceive(void *pvParameters){
  (void) pvParameters;
  mqttInit();
  mqttReceive();
  while(1){
    mqttIsConected();
    mqttLoop();
  }
}
*/

/* Implemntação da vTaskValvula */

void vTaskValvula(void *pvParameters){
  (void) pvParameters;
  init_valvula();
  
  while(1){
    if(btn_state == 1 || flag_valvula == 1) {
      acionar_valvula();
      //envio mqtt valvula
    }  
    else if (btn_state == 0 || flag_valvula == 0){
      desligar_valvula();
      //envio mqtt valvula
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}


/*
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
*/