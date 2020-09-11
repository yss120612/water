

#ifndef _CONFIG_h
#define _CONFIG_h
#include "arduino.h"


extern const char * webpass;
extern const char * webuser;
extern const char * wifiuser;
extern const char * wifipass;
extern const char * wifiuser1;
extern const char * wifipass1;
extern const char * ntp_server;
extern const char * mqtt_server; // Имя сервера MQTT
extern const int    mqtt_port; // Порт для подключения к серверу MQTT
extern const char * mqtt_user; // Логи от сервер
extern const char * mqtt_pass; // Пароль от сервера

#define TIME_OFFSET 8

#define SENSOR1 1
#define SENSOR2 2
#define SENSOR3 3
#define SENSOR4 4

// CONNECTIONS:
#define DS1302_CLK D2
#define DS1302_DAT D1
#define DS1302_RST D0

#define PIN_VOPEN D7
#define PIN_VCLOSE D8

#define MEM_ALARM 0


#endif