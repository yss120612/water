#include "MQTTclient.h"



 MqttClient::MqttClient(){

 }

 MqttClient::~MqttClient(){
     delete(client);
 }
    void  MqttClient::setup(){
        WiFiClient * wf = new WiFiClient();
        
        client = new PubSubClient(mqtt_server, mqtt_port,*wf);
    };



    void MqttClient::loop(long ms){

    };