// Dette er eksempelkode 2, som er ment som en litt mer avansert måte å fullføre oppgavene på.

// Merk at koden her kun er ment som et eksempel og ikke en fasit.
// Det er mange måter å løse oppgavene på.

#include "Car.h"

Car car("ruter_cot", "ESP_32_is_best");
// Car car("iPhone", "testPassord");

void setup() {
  car.initCar(BLACK); //BLACK for svart linje og WHITE for hvit linje
}

//lag variabler her
bool linemode = false;  //variabel for å skru av og på linjefølger

int speed = 50; //variabel som lagrer hastigheten vi vil kjøre med

int leftSpeed = 0; //variabel som lagrer hastigheten venstre belte kjører med akkurat nå
int rightSpeed = 0; //variabel som lagrer hastigheten høyre belte kjører med akkurat nå

void loop() { //ikke fjern denne linjen!

  //skriv kode for å sende data her
  sendData(1,speed); //send hastigheten til graf 1
  sendData(2,readLine()); //send linjedata til graf 2
  sendData(3,readProx()); //send avstandsdata til graf 3
  
  if(linemode == true) { //sjekk om linjefølger skal være på
    //skriv linjefølger-kode her
    drive(75+readLine()/2, 75-readLine()/2); //kjør med varierende hastighet basert på dataen fra linjefølgeren
  } else {  //om linjefølger ikke skal være på så gjør dette
    drive(leftSpeed, rightSpeed); //kjør med hastigheten bestemt av de to variablene
  }
  
}

//dette er koden for kjøre framover
//å implentere kjøringen på denne måten gjør at man kan trykke inn flere knapper og komninere hastighetene
//for eksempel, om man trykker w og s samtidig vil bilen stå stille
void w(bool button) { //ikke fjern denne linjen!
  if(button==DOWN) {
    leftSpeed += speed;  //legg til hastighetsvariabelen speed til leftSpeed
    rightSpeed += speed;  //legg til hastighetsvariabelen speed til rightSpeed
  }
  if(button==UP) {
    leftSpeed -= speed; //trekk fra hastighetsvariabelen speed til leftSpeed
    rightSpeed -= speed;  //trekk fra hastighetsvariabelen speed til rightSpeed
  }
}

void a(bool button) { //ikke fjern denne linjen!
  //BEGYNN HER!
  if(button==DOWN) { 
    leftSpeed -= speed;
    rightSpeed += speed;
  }
  if(button==UP) {
    leftSpeed += speed;
    rightSpeed -= speed;
  }
}

void s(bool button) { //ikke fjern denne linjen!
  //skriv kode her
  if(button==DOWN) {
    leftSpeed -= speed;
    rightSpeed -= speed;
  }
  if(button==UP) {
    leftSpeed += speed;
    rightSpeed += speed;
  }
}

void d(bool button) { //ikke fjern denne linjen!
  //skriv kode her
  if(button==DOWN) {
    leftSpeed += speed;
    rightSpeed -= speed; 
  }
  if(button==UP) {
    leftSpeed -= speed;
    rightSpeed += speed;
  }
}

void e(bool button) { //ikke fjern denne linjen!
  //skriv kode her
  if(button==DOWN) {
    leftSpeed += speed;
  }
  if(button==UP) {
    leftSpeed -= speed;
  }
}

void q(bool button) { //ikke fjern denne linjen!
  //skriv kode her
  if(button==DOWN) {
    rightSpeed += speed; 
  }
  if(button==UP) {
    rightSpeed -= speed;
  }
}

void triangle(bool button) { //ikke fjern denne linjen!
  //skriv kode her
  if(button == UP) return; //om knappen slippes opp så avslutt kodesnutten her. Altså vil ikke linjene under kjøres
  linemode = !linemode; //endre linemode-variabelen til det motsatte av det den er nå. For eksempel, om linemode nå er lik false vil den bli byttet til true
  leftSpeed = 0;  //sett leftSpeed til 0
  rightSpeed = 0; //sett rightSpeed til 0
}

void circle(bool button) { //ikke fjern denne linjen!
  //skriv kode her
  if(button) speed-=10; //trekk fra 10 fra hastigheten
}

void square(bool button) { //ikke fjern denne linjen!
  //skriv kode her
  if(button) speed+=10; //legg til 10 til hastigheten
}
