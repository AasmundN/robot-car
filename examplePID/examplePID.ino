#include "Car.h"

Car car("ruter_cot", "ESP_32_is_best");

void setup() {
   car.initCar();
}

bool linemode = false;

int speed = 50;

int leftSpeed = 0;
int rightSpeed = 0;

float P = 1;
float I = 0.000001;
float D = 0.001;

int eprev = 0;
float i = 0;
int der = 0;
int err = 0;

float u;

float sat(float x, float maxlim, float minlim) {
   if (x > maxlim)
      return maxlim;
   if (x < minlim)
      return minlim;
   return x;
}

void loop() {

   car.sendData(1, car.data[PROXIMITY].value);
   car.sendData(2, car.data[ENCODERS].value);

   if (linemode == true) {
      car.sendData(3, car.data[LINE].value);

      eprev = err;
      err = car.data[LINE].value;
      i = sat(i + I * err, 100, -100);
      der = err - eprev;

      u = sat(P * err + i + D * der, 100, -100);

      if (u > 0) {
         car.drive(100, 100 - abs(u));
      } else {
         car.drive(100 - abs(u), 100);
      }
   } else {
      car.drive(leftSpeed, rightSpeed);
   }
}

void w(bool button) {
   if (button == DOWN) {
      leftSpeed += speed;
      rightSpeed += speed;
   }
   if (button == UP) {
      leftSpeed -= speed;
      rightSpeed -= speed;
   }
}

void a(bool button) {

   if (button == DOWN) {
      leftSpeed -= speed;
      rightSpeed += speed;
   }
   if (button == UP) {
      leftSpeed += speed;
      rightSpeed -= speed;
   }
}

void s(bool button) {

   if (button == DOWN) {
      leftSpeed -= speed;
      rightSpeed -= speed;
   }
   if (button == UP) {
      leftSpeed += speed;
      rightSpeed += speed;
   }
}

void d(bool button) {

   if (button == DOWN) {
      leftSpeed += speed;
      rightSpeed -= speed;
   }
   if (button == UP) {
      leftSpeed -= speed;
      rightSpeed += speed;
   }
}

void e(bool button) {

   if (button == DOWN) {
      leftSpeed += speed;
   }
   if (button == UP) {
      leftSpeed -= speed;
   }
}

void q(bool button) {

   if (button == DOWN) {
      rightSpeed += speed;
   }
   if (button == UP) {
      rightSpeed -= speed;
   }
}

void triangle(bool button) {

   if (button == UP)
      return;
   linemode = !linemode;

   if (linemode)
      car.calibrateLine(BLACK);
   leftSpeed = 0;
   rightSpeed = 0;
}

void circle(bool button) {

   if (button)
      speed -= 10;
}

void square(bool button) {

   if (button)
      speed += 10;
}
