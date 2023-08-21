#include "Car.h"

Car car("networkName", "networkPassword");

void setup() {
   car.initCar();
}

void loop() {
   // code to run continously
}

void w(bool button) {
   if (button == DOWN) {
      car.drive(50, 50);
   }
   if (button == UP) {
      car.drive(0, 0);
   }
}

void a(bool button) {
   // button a
}

void s(bool button) {
   // button s
}

void d(bool button) {
   // button d
}

void e(bool button) {
   // button e
}

void q(bool button) {
   // button q
}

void triangle(bool button) {
   // button triangle
}

void circle(bool button) {
   // button circle
}

void square(bool button) {
   // button square
}
