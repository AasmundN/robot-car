#include "Car.h"

Car car("networkName", "networkPassword");

void setup() {
   car.initCar();
}

bool linemode = false;
int linjetall;

void loop() {

   car.sendData(1, car.data[PROXIMITY].value);

   if (linemode == true) {

      linjetall = car.data[LINE].value;

      car.sendData(2, linjetall);

      if (linjetall < -10) {
         car.drive(0, 60);
      }
      if (linjetall > 10) {
         car.drive(60, 0);
      }
      if (linjetall > -10 && linjetall < 10) {
         car.drive(60, 60);
      }
   }
}

void w(bool button) {
   if (button == DOWN) {

      car.drive(100, 100);
   }
   if (button == UP) {

      car.drive(0, 0);
   }
}

void a(bool button) {

   if (button == DOWN) {

      car.drive(-100, 100);
   }
   if (button == UP) {

      car.drive(0, 0);
   }
}

void s(bool button) {

   if (button == DOWN) {

      car.drive(-100, -100);
   }
   if (button == UP) {

      car.drive(0, 0);
   }
}

void d(bool button) {

   if (button == DOWN) {

      car.drive(100, -100);
   }
   if (button == UP) {

      car.drive(0, 0);
   }
}

void e(bool button) {

   if (button == DOWN) {

      car.drive(100, 0);
   }
   if (button == UP) {

      car.drive(0, 0);
   }
}

void q(bool button) {

   if (button == DOWN) {

      car.drive(0, 100);
   }
   if (button == UP) {

      car.drive(0, 0);
   }
}

void triangle(bool button) {

   if (button == DOWN) {
      car.calibrateLine(BLACK);
      linemode = true;
   }
}

void circle(bool button) {

   if (button == DOWN) {
      linemode = false;
      car.drive(0, 0);
   }
}

void square(bool button) {
}
