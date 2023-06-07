#include "Car.h"

//nettverksnavn og nettverkspassord byttes ut med navn og passord for det aktuelle nettverket
Car car("nettverksnavn", "nettverkspassord");

void setup() { 
  //BLACK for svart linje og WHITE for hvit linje
  car.initCar(BLACK);
}

//lag variabler her
bool linemode = false;


void loop() { //ikke fjern denne linjen!

  //skriv kode for å sende data her

  
  if(linemode == true) {
    //skriv linjefølger-kode her
    
  }
}

//dette er koden for kjøre framover
void w(bool button) { //ikke fjern denne linjen!
  if(button==DOWN) {
    //dette skjer når knappen trykkes ned
    drive(50,50); 
  }
  if(button==UP) {
    //dette skjer når knappen slippes opp
    drive(0,0);
  }
}

void a(bool button) { //ikke fjern denne linjen!
  //BEGYNN HER!

}

void s(bool button) { //ikke fjern denne linjen!
  //skriv kode her

}

void d(bool button) { //ikke fjern denne linjen!
  //skriv kode her

}

void e(bool button) { //ikke fjern denne linjen!
  //skriv kode her

}

void q(bool button) { //ikke fjern denne linjen!
  //skriv kode her

}

void triangle(bool button) { //ikke fjern denne linjen!
  //skriv kode her

}

void circle(bool button) { //ikke fjern denne linjen!
  //skriv kode her

}

void square(bool button) { //ikke fjern denne linjen!
  //skriv kode her

}
