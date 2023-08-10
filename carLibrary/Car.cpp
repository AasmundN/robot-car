// some inspiration: https://microcontrollerslab.com/esp32-websocket-server-arduino-ide-control-gpios-relays/
// and https://RandomNerdTutorials.com/esp32-websocket-server-arduino/

#include "Car.h"
#include "HTML.h"

#define port 80

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define RXD2 16
#define TXD2 17

AsyncWebServer server(port);
AsyncWebSocket ws("/ws");

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long prevDataMillis[3];
int dataPerSec = 10;

unsigned long prevDriveMillis = 0;
int drivePerSec = 50;

bool isCalibrated, lineColor = false;

TaskHandle_t secondCore;

Car::Car(char *ssid, char *password) : ssid{ssid}, password{password} {};

void writeDisplay(String str, int displayLine) {
   display.setCursor(0, displayLine * 8);
   display.println(str);
   display.display();
}

void Car::drive(int leftSpeed, int rightSpeed) {
   if ((millis() - prevDriveMillis) < (1000 / drivePerSec) && (leftSpeed || rightSpeed))
      return;

   if (leftSpeed > 100) {
      leftSpeed = 100;
   } else if (leftSpeed < -100) {
      leftSpeed = -100;
   }

   if (rightSpeed > 100) {
      rightSpeed = 100;
   } else if (rightSpeed < -100) {
      rightSpeed = -100;
   }

   // Sender til bilen
   Serial2.write('k');
   Serial2.write(leftSpeed);
   Serial2.write(rightSpeed);

   prevDriveMillis = millis();
}

void Car::sendData(int graph, double data) {
   if ((millis() - prevDataMillis[graph - 1]) < (1000 / dataPerSec))
      return;
   if (graph < 1)
      graph = 1;
   if (graph > 3)
      graph = 3;
   data = floor(data * 10) / 10;
   ws.textAll(String(graph) + String(data));
   prevDataMillis[graph - 1] = millis();
}

void Car::calibrateLine() {
   if (!isCalibrated) {
      if (lineColor)
         Serial2.write('C');
      else
         Serial2.write('c');
      isCalibrated = true;
   }
}

void Car::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {

   AwsFrameInfo *info = (AwsFrameInfo *)arg;

   if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

      data[len] = 0;

      switch (*(char *)data) {
      case 'f':
         triangle(DOWN);
         break;

      case 'g':
         circle(DOWN);
         break;

      case 'h':
         square(DOWN);
         break;

      case 'q':
         q(DOWN);
         break;

      case 'w':
         w(DOWN);
         break;

      case 'e':
         e(DOWN);
         break;

      case 'a':
         a(DOWN);
         break;

      case 's':
         s(DOWN);
         break;

      case 'd':
         d(DOWN);
         break;

      case 'F':
         triangle(UP);
         break;

      case 'G':
         circle(UP);
         break;

      case 'H':
         square(UP);
         break;

      case 'Q':
         q(UP);
         break;

      case 'W':
         w(UP);
         break;

      case 'E':
         e(UP);
         break;

      case 'A':
         a(UP);
         break;

      case 'S':
         s(UP);
         break;

      case 'D':
         d(UP);
         break;

      case ' ':

         break;

      default:
         Serial.println("Bad package");
         break;
      }
   }
}

void Car::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                  void *arg, uint8_t *data, size_t len) {
   switch (type) {
   case WS_EVT_CONNECT:
      Serial.printf("WebSocket-klient #%u koblet til med IP: %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
   case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
   case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
   case WS_EVT_PONG:
   case WS_EVT_ERROR:
      break;
   }
}

void Car::initWebSocket() {
   ws.onEvent(onEvent);
   server.addHandler(&ws);
}

void secondCoreLoop(void *pvParameters) {

   static Car *car = (Car *)pvParameters;

   for (;;) {
      ws.cleanupClients();
      if (Serial2.available() >= 5) {
         for (int i = 0; i < 5; i++) {
            // encoder and gyro values are devided by two on the zumo
            if (i == ENCODERS || i == GYRO)
               car->data[i].value = 2 * Serial2.read();

            car->data[i].value = Serial2.read();
            car->data[i].flag = true;
         }

         while (Serial2.available())
            Serial2.read();
      }
   }
}

String processor(const String &var) {
   if (var == "GRAPH_NAME")
      return WiFi.macAddress();
   return String();
}

void Car::initCar(bool color) {

   lineColor = color;

   Serial.begin(115200);
   Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

   xTaskCreatePinnedToCore(
       secondCoreLoop,
       "secondCore",
       10000,
       this,
       1,
       &secondCore,
       1);

   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
         ;
   }

   delay(2000);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   writeDisplay("Skjerm klar", 1);

   // Connect to Wi-Fi
   WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Kobler til WIFI...");
      display.clearDisplay();
      writeDisplay("Kobler til WIFI...", 1);
   }

   // Print ESP Local IP Address
   Serial.print("Koblet til internett med IP: ");
   Serial.println(WiFi.localIP());
   //  Serial.println(":" + static_cast<String>(port));
   display.clearDisplay();
   writeDisplay("Koblet til med IP: ", 1);
   display.print(WiFi.localIP());
   //  display.println(":" + static_cast<String>(port));
   // display.println("WiFi: " + static_cast<String>(ssid)); //Seb la inn
   display.display();

   initWebSocket();

   // Route for root / web page
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html, processor);
   });

   // Start server
   server.begin();

   // notify car that everything is ready
   Serial2.write('w');
   Serial.println("Bil klar!");
}
