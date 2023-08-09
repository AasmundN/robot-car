#pragma once

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> //library change: https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/WebResponseImpl.h#L62
#include <WiFi.h>

#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LINE 0
#define PROX 1

#define UP 0
#define DOWN 1

#define BLACK 0
#define WHITE 1

class Car {
 private:
   const char *ssid;
   const char *password;

 public:
   Car(char *ssid, char *password);
   static void notifyClients();
   static void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
   static void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                       void *arg, uint8_t *data, size_t len);
   static void initWebSocket();
   void initCar(bool color);
};

double readNTC();
int readLine();
int readProx();
int readEncoders();
int readAngleX();
int readAngleY();
int readAngleZ();
int getReadTime();
void sendData(int graph, double data);

void drive(int leftSpeed, int rightSpeed);

void w(bool knapp);
void a(bool knapp);
void s(bool knapp);
void d(bool knapp);
void q(bool knapp);
void e(bool knapp);

void triangle(bool knapp);
void circle(bool knapp);
void square(bool knapp);
