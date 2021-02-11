/*
Task             Core  Prio     Descrição
-------------------------------------------------------------------------------
vTaskSensor        1     1     faz a leitura do sensor analógico
vTaskPrint         1     1     Imprime o valor do sensor o display
vTaskMQTT          0     2     Publica valor do Sensor em tópico MQTT
*/


xTimerHandle xTimer;
QueueHandle_t xFila; 

TaskHandle_t xTaskPrintHandle;
TaskHandle_t xTaskMQTTHandle;
TaskHandle_t xTaskSensorHandle;

void vTaskSensor(void *pvParameters );
void vTaskPrint(void *pvParameters);
void vTaskMQTT(void *pvParameters); 
void vTaskTeclado( void *pvParameters );

void callBackTimer(TimerHandle_t pxTimer );

void rtosInit(){

    xFila = xQueueCreate(1, sizeof(int));
    xTimer = xTimerCreate("TIMER",pdMS_TO_TICKS(2000),pdTRUE, 0, callBackTimer);

    xTaskCreatePinnedToCore(vTaskSensor,  "TaskADC",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskSensorHandle,APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTaskPrint,  "TaskPrint",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskPrintHandle,APP_CPU_NUM);
    xTaskCreatePinnedToCore(vTaskMQTT,  "TaskMQTT",  configMINIMAL_STACK_SIZE + 2048,  NULL,  2,  &xTaskMQTTHandle,PRO_CPU_NUM);  
    xTimerStart(xTimer,0);
}


/* impementação da TaskADC */
void vTaskSensor(void *pvParameters ){
  (void) pvParameters;
  
  int adcValue;
  while(1){
    adcValue = sensorRead();
    xQueueOverwrite(xFila, &adcValue);/* envia valor atual de count para fila*/
    vTaskDelay(pdMS_TO_TICKS(1000)); /* Aguarda 5000 ms antes de uma nova iteração*/
  }
}


/* Implemntação da vTaskDisplay  */
void vTaskPrint(void *pvParameters ){
  (void) pvParameters;  /* Apenas para o Compilador não retornar warnings */
  int valor_recebido = 0;
  while(1){
      if(xQueueReceive(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) {//verifica se há valor na fila para ser lido. Espera 1 segundo
        imprimeSensorDisplay(valor_recebido);
      }
  }
}


/*Implementação da Task MQTT */
void vTaskMQTT(void *pvParameters){
  (void) pvParameters;
  char mensagem[30];
  int valor_recebido = 0;

  while(1){
    
      if(xQueueReceive(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) { //verifica se há valor na fila para ser lido. Espera 1 segundo
        mqttIsConected();
        sprintf(mensagem, "%d", valor_recebido);
        mqttSend(mensagem);       
        vTaskDelay(15000);
      }
  }

}

void callBackTimer(TimerHandle_t pxTimer ){

}
