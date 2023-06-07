#include <Zumo32U4.h>

//https://www.pololu.com/docs/0J63

const int ENA = 10; //Motor 1
const int IN1 = 16; //Wheel direction 1
const int ENB = 9; //Motor 2
const int IN2 = 15; //Wheel direction 2

int actual_l_val = 0;
int actual_r_val = 0;

int ideal_l_val = 0;
int ideal_r_val = 0;

int acc_const = 50;
int accPerSec = 100;
unsigned long prevAccMillis = 0;

int maxSpeed = 190;
bool isCalibrated = false;
bool lineColor = false;

int encoderValLeft, encoderValRight;
float stallThresholdLeft = 100;
float stallThresholdRight = 100;

bool prevEncoderVals[3] = {0,0,0};
bool emergancyStop = false;
unsigned long lastStop = 0;
int stopDuration = 2;

const int lineSensorValues[5] = {0,0,0,0,0};

const unsigned int microSecPerByte = ceil(10.0 / 115200.0 * 100000);

unsigned long lastSpeedChange = 0;
int speedChangePerSec = 2;

unsigned long prevReadMillis = 0;
int readPerSec = 20; //må være mindre enn 71, da hver read tar 13-14ms

unsigned long prevStallMillis = 0;
int stallPerSec = 4;

Zumo32U4LineSensors lineSensors;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Encoders encoders;

int8_t data[2];
int8_t pos;

void calibrateLineSensors() {
  for(uint8_t i = 0; i < 120; i++) {
    if (i > 30 && i <= 90) {
      drive(-100,100);
    } else {
      drive(100,-100);
    }
    lineSensors.calibrate();
  }

  drive(0,0);
}

void drive(int8_t l_val, int8_t r_val) {
  digitalWrite(IN1, l_val>0?0:1);
  digitalWrite(IN2, r_val>0?0:1);

  l_val = (maxSpeed/100)*abs(l_val);
  r_val = (maxSpeed/100)*abs(r_val);

  analogWrite(ENA, l_val);
  analogWrite(ENB, r_val);
}

void accelerate(int8_t l_val, int8_t r_val){
  if(abs(l_val-ideal_l_val)>25||abs(r_val-ideal_r_val)>25) {
    for(auto val : prevEncoderVals) val = false;
    lastSpeedChange = millis();
  }
  ideal_r_val = r_val;
  ideal_l_val = l_val;  
}

void setup() {
  drive(0,0);

  lineSensors.initFiveSensors();
  proxSensors.initFrontSensor();

  Serial1.begin(115200);
  Serial.begin(115200);

  delay(100);
  buzzer.play(">g32>>>>>c32");
}

void loop() {

  if(prevEncoderVals[0]&&prevEncoderVals[1]&&prevEncoderVals[2]&&!emergancyStop) {
    Serial.println("Emergancystop on!");
    emergancyStop = true;
    lastStop = millis();
    lastSpeedChange = millis();
    drive(0,0);
  }

  if(emergancyStop&&(millis()-lastStop)>(1000*stopDuration)) {
    Serial.println("Emergancystop off!");
    emergancyStop = false;
    for(auto val : prevEncoderVals) val = false;
  }

  if((millis()-prevAccMillis)>(1000/accPerSec)&&!emergancyStop) {

    if(actual_l_val < ideal_l_val) 
      actual_l_val += acc_const;
    
    if(actual_r_val < ideal_r_val) 
      actual_r_val += acc_const;
    
    if(actual_l_val > ideal_l_val) 
      actual_l_val -= acc_const;
    
    if(actual_r_val > ideal_r_val) 
      actual_r_val -= acc_const;
    

    if(abs(actual_r_val - ideal_r_val) <= acc_const) 
      actual_r_val = ideal_r_val;
    
    if(abs(actual_l_val - ideal_l_val) <= acc_const) 
      actual_l_val = ideal_l_val;
    

    drive(actual_l_val, actual_r_val);

    prevAccMillis = millis();
    
  } else if(emergancyStop) {
    drive(0,0);
  }

  if((millis()-prevStallMillis)>(1000/stallPerSec)&&(millis()-lastSpeedChange)>(1000/speedChangePerSec)&&!emergancyStop) {

    encoderValLeft = sqrt(pow(encoders.getCountsAndResetLeft(),2));
    encoderValRight = sqrt(pow(encoders.getCountsAndResetRight(),2));

    stallThresholdLeft = ((2*abs(actual_l_val)-150)+abs(2*abs(actual_l_val)-150))/2+100;
    stallThresholdRight = ((2*abs(actual_r_val)-150)+abs(2*abs(actual_r_val)-150))/2+100;

    prevEncoderVals[2] = prevEncoderVals[1];
    prevEncoderVals[1] = prevEncoderVals[0];
    
    if((encoderValLeft<stallThresholdLeft&&actual_l_val&&ideal_l_val)
      ||(encoderValRight<stallThresholdRight&&actual_r_val&&ideal_r_val))
      prevEncoderVals[0] = true;
    else
      prevEncoderVals[0] = false;

    Serial.print(encoderValLeft);
    Serial.print("  ");
    Serial.print(stallThresholdLeft);
    Serial.print("  ");
    Serial.print(encoderValRight);
    Serial.print("  ");
    Serial.print(stallThresholdRight);
    Serial.print("  ");
    Serial.print(prevEncoderVals[0]);
    Serial.print("  ");
    Serial.print(prevEncoderVals[1]);
    Serial.print("  ");
    Serial.println(prevEncoderVals[2]);  

    prevStallMillis = millis();
  }

  if(Serial1.available()) {
    char received = (char)Serial1.read();
    //Serial.print("recieved command: ");
    //Serial.println(received); //Print data to Serial Monitor

    switch(received) {
      case 'k': //Kjør!
        //Serial.println('k');
        while(Serial1.available() < 2){
          delayMicroseconds(microSecPerByte);
        }

        for(int i = 0; i < 2; i++){
          if(Serial1.available()){
            data[i] = Serial1.read();
            //Serial.print(data[i]);
          }
        }

        // Serial.print(data[0]);
        // Serial.print("  ");
        // Serial.println(data[1]);
        accelerate(data[0], data[1]);
        break;

      case 'c': //kalibrer linjefølger
        //Serial.println('c');
        lineColor = false;
        if(!isCalibrated) {
          calibrateLineSensors();
          isCalibrated = true;
        }
        break;

      case 'C': //kalibrer linjefølger
        //Serial.println('C');
        lineColor = true;
        if(!isCalibrated) {
          calibrateLineSensors();
          isCalibrated = true;
        }
        break;

      default:
        break;
    }

    if(Serial1.available() >= 20){
      while(Serial1.available() > 10) {
        Serial1.read();
      }
    }
  }

  if((millis()-prevReadMillis)>(1000/readPerSec)) {
    //Dette tar 13-14ms

    prevReadMillis = millis();

    proxSensors.read();
    int8_t leftValue = proxSensors.countsFrontWithLeftLeds();
    int8_t rightValue = proxSensors.countsFrontWithRightLeds();
    Serial1.write(leftValue+rightValue);

    //pos = (int8_t)((lineSensors.readLine(lineSensorValues)/20)-100); //svart linje
    pos = (int8_t)((lineSensors.readLine(lineSensorValues,QTR_EMITTERS_ON,lineColor)/20)-100);
    //Serial.println(pos);
    Serial1.write(pos);

    //Serial.println(prevReadMillis);
  }
}
