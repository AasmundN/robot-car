#include "Car.h"

Car car("ruter_cot", "ESP_32_is_best");

void setup() {
   car.initCar();
}

#define ENCODER_VALUES_PER_MOTOR_TURN 12.0
#define MOTOR_TURNS_PER_WHEEL_TURN 75.0
#define MILLIMETERS_PER_WHEEL_TURN 122.52211349

int speed = 50;

int leftSpeed = 0;
int rightSpeed = 0;

float carSpeed = 0;

void loop() {

   if (car.data[ENCODERS].flag) {
      carSpeed = car.data[ENCODERS].value / car.data[READ_TIME].value;          // encodervalue/ms
      carSpeed /= (ENCODER_VALUES_PER_MOTOR_TURN * MOTOR_TURNS_PER_WHEEL_TURN); // wheelturns/ms
      carSpeed *= MILLIMETERS_PER_WHEEL_TURN;                                   // m/s
      carSpeed *= 100;                                                          // cm/s

      car.data[ENCODERS].flag = false;
      car.sendData(1, carSpeed);
      car.sendData(2, car.data[ENCODERS].value);
      car.sendData(3, car.data[READ_TIME].value);
   }

   car.drive(leftSpeed, rightSpeed);
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
}

void circle(bool button) {

   if (button)
      speed -= 10;
}

void square(bool button) {

   if (button)
      speed += 10;
}
