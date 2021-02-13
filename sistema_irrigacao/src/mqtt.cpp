#include "mqtt.h"

#include <Arduino.h>

/*bibliotecas para MQTT*/
#include <WiFi.h>
#include "PubSubClient.h"

/* instacias para WIFI e client*/
WiFiClient espClient;
PubSubClient client(espClient);



void mqttReconect();

/* configuraçãoes da REDE e broker MQTT*/
const char* ssid = "XXXXXXXXX";
const char* password =  "xxxxxxxxxxxxxxxxxxxxxxxxx";

/* configuraçãoes do broker MQTT*/
const char* mqttServer = "io.adafruit.com";
const int mqttPort = 1883;
const char* mqttUser ="danillo_lins";
const char* mqttPassword ="aio_vyAc897VRhNE4UoP7JqOISrKqY3c";

void mqttInit(){
   
   WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi..");
   }
  Serial.println("Conectado na rede WiFi");

  client.setServer(mqttServer, mqttPort);



}

void mqttIsConected(){
    if(!client.connected()){
          mqttReconect();
    }
}

void mqttSend_sensor(char mensagem[]){
    //Envia a mensagem ao broker
    client.publish("danillo_lins/feeds/sensor", mensagem);
    Serial.print("Valor enviado sensor: ");
    Serial.println(mensagem);
}

void mqttSend_valvulaState(char mensagem[]){
    //Envia a mensagem ao broker
    client.publish("danillo_lins/feeds/valvula", mensagem);
    Serial.print("Valor enviado valvula: ");
    Serial.println(mensagem);
}



//função pra reconectar ao servido MQTT
void mqttReconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {

    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      client.subscribe("danillo_lins/feeds/acionar-valvula");
      //Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}
/*
void callback_valvula(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Mensagem do broker: ");
  Serial.println(topic);
 
  Serial.print("Estado da valvula:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  valvula_state = (char)payload;
 
  Serial.println();
  Serial.println("-----------------------");
 
}
*/

void mqttReceive(){
  client.setCallback(callback_valvula);
}

void mqttLoop(){
  client.loop();
}

void mqttSubcribe(){
  client.subscribe("danillo_lins/feeds/acionar-valvula");
}
