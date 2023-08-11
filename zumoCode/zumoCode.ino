#include <Wire.h>
#include <Zumo32U4.h>
// https://www.pololu.com/docs/0J63

// pins
#define LED_PIN 14
#define ENA 10 // Motor 1
#define IN1 16 // Wheel direction 1
#define ENB 9  // Motor 2
#define IN2 15 // Wheel direction 2

#define MOTOR_LOWER_LIMIT 30

int maxSpeed = 200;
bool lineColor;

// acceleration
int actual_l_val, actual_r_val, ideal_l_val, ideal_r_val;
int acc_const = 50;

unsigned long prevAccMillis, lastSpeedChange, prevReadMillis;
int accPerSec = 100;
int readPerSec = 18;

// sensor placeholders
int8_t leftProxVal, rightProxVal, linePos;
int16_t leftEncoderVal, rightEncoderVal;
const int lineSensorValues[5];

// Gyro
int32_t gyroOffset, gyroAngleZ;
uint16_t lastGyroUpdate = 0;
#define CALIBRATE_COUNT 4096

// utility
const unsigned int microSecPerByte = ceil(10.0 / 115200.0 * 100000);
int8_t speedValues[2];

// init sensors
Zumo32U4LineSensors lineSensors;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Encoders encoders;
Zumo32U4IMU imu;

void calibrateLineSensors() {
   Serial.println("Calibrating!");
   lineSensors.resetCalibration();
   for (uint8_t i = 0; i < 120; i++) {
      if (i > 30 && i <= 90) {
         drive(-100, 100);
      } else {
         drive(100, -100);
      }
      lineSensors.calibrate();
   }
   Serial.println("Calibrate done!");
   drive(0, 0);
}

void calibrateGyro() {
   // calibrate Z angle of the gyroscope
   gyroOffset = 0;
   for (int i = 0; i < CALIBRATE_COUNT; i++) {
      while (!imu.gyroDataReady())
         ;
      imu.readGyro();
      gyroOffset += imu.g.z;
   }
   gyroOffset /= CALIBRATE_COUNT;
}

void updateAngle() {
   if (imu.gyroDataReady()) {
      uint16_t m = micros();
      uint16_t dt = m - lastGyroUpdate;
      lastGyroUpdate = m;
      imu.readGyro();
      gyroAngleZ += (((int64_t)((imu.g.z - gyroOffset) * dt)) * 14680064 / 17578125);
   }
}

void drive(int8_t l_val, int8_t r_val) {
   digitalWrite(IN1, l_val > 0 ? 0 : 1);
   digitalWrite(IN2, r_val > 0 ? 0 : 1);

   l_val = abs(l_val);
   r_val = abs(r_val);

   l_val = l_val > MOTOR_LOWER_LIMIT ? (maxSpeed / 100) * l_val : 0;
   r_val = r_val > MOTOR_LOWER_LIMIT ? (maxSpeed / 100) * r_val : 0;

   analogWrite(ENA, l_val);
   analogWrite(ENB, r_val);
}

void accelerate(int8_t l_val, int8_t r_val) {
   ideal_r_val = r_val;
   ideal_l_val = l_val;
}

void setup() {
   drive(0, 0);

   lineSensors.initFiveSensors();
   proxSensors.initFrontSensor();

   Serial1.begin(115200);
   Serial.begin(115200);

   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, LOW);

   Wire.begin();
   imu.init();
   imu.enableDefault();
   imu.configureForTurnSensing();
   calibrateGyro();

   delay(100);
   buzzer.play(">g32>>>>>c32");
}

void loop() {
   // acceleration
   if ((millis() - prevAccMillis) > (1000 / accPerSec)) {

      if (actual_l_val < ideal_l_val)
         actual_l_val += acc_const;

      if (actual_r_val < ideal_r_val)
         actual_r_val += acc_const;

      if (actual_l_val > ideal_l_val)
         actual_l_val -= acc_const;

      if (actual_r_val > ideal_r_val)
         actual_r_val -= acc_const;

      if (abs(actual_r_val - ideal_r_val) <= acc_const)
         actual_r_val = ideal_r_val;

      if (abs(actual_l_val - ideal_l_val) <= acc_const)
         actual_l_val = ideal_l_val;

      drive(actual_l_val, actual_r_val);

      prevAccMillis = millis();
   }

   if (Serial1.available()) {
      char received = (char)Serial1.read();
      Serial.println(received);

      switch (received) {
      case 'k': // Kjør!
         while (Serial1.available() < 2) {
            delayMicroseconds(microSecPerByte);
         }

         for (int i = 0; i < 2; i++) {
            if (Serial1.available()) {
               speedValues[i] = Serial1.read();
            }
         }

         accelerate(speedValues[0], speedValues[1]);
         break;

      case 'c': // kalibrer linjefølger
         lineColor = false;
         calibrateLineSensors();
         break;

      case 'C': // kalibrer linjefølger
         lineColor = true;
         calibrateLineSensors();
         break;

      case 'w': // connected to wifi
         digitalWrite(LED_PIN, HIGH);
         break;

      default:
         break;
      }

      if (Serial1.available() >= 20) {
         while (Serial1.available() > 10) {
            Serial1.read();
         }
      }
   }

   // read gyroscope and estimate angle
   updateAngle();

   // read data
   if ((millis() - prevReadMillis) > (1000 / readPerSec)) {

      // read and send proximity data
      proxSensors.read();
      leftProxVal = proxSensors.countsFrontWithLeftLeds();
      rightProxVal = proxSensors.countsFrontWithRightLeds();
      Serial1.write(leftProxVal + rightProxVal);

      // raed and send line data
      linePos = (int8_t)((lineSensors.readLine(lineSensorValues, QTR_EMITTERS_ON, lineColor) / 20) - 100);
      Serial1.write(linePos);

      // read and send avg of left and right encoders
      leftEncoderVal = encoders.getCountsAndResetLeft();
      rightEncoderVal = encoders.getCountsAndResetRight();
      Serial1.write((leftEncoderVal + rightEncoderVal) / 4); // send avg devided by two

      // send Z angle
      Serial1.write((int8_t)(((gyroAngleZ >> 16) * 360) >> 17));

      // send and update read time
      Serial1.write(millis() - prevReadMillis);
      prevReadMillis = millis();
   }
}
