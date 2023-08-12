#include "Car.h"

Car car("ruter_cot", "ESP_32_is_best");

void setup() {
   car.initCar();
}

bool turnResistMode = false;
int startAngle = 0;

int speed = 50;

int leftSpeed = 0;
int rightSpeed = 0;

void loop() {
   car.sendData(1, car.data[GYRO].value);
   car.sendData(2, car.data[ENCODERS].value);
   car.sendData(3, car.data[PROXIMITY].value);

   if (turnResistMode == true) {
      int angle = car.data[GYRO].value;
      int turnspeed = (angle)*100 / 180;
      car.drive(turnspeed * 10, -turnspeed * 10);
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
   turnResistMode = !turnResistMode;

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
