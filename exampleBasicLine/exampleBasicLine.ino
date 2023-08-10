// Dette er eksempelkode 1, som er ment som en enkel måte å fullføre oppgavene
// på.

// Merk at koden her kun er ment som et eksempel og ikke en fasit.
// Det er mange måter å løse oppgavene på.

#include "Car.h"

Car car("ruter_cot", "ESP_32_is_best");

void setup() {
   car.initCar(BLACK); // BLACK for svart linje og WHITE for hvit linje
}

// lag variabler her
bool linemode = false; // variabel for å skru av og på linjefølger
int linjetall;         // variabel for å lagre dataen fra linjesensorene

void loop() { // ikke fjern denne linjen!

   // skriv kode for å sende data her
   car.sendData(1, car.data[PROXIMITY].value); // send avstandsdata til graf 1

   if (linemode == true) { // sjekk om linjefølgeren skal være på
      // skriv linjefølger-kode her
      linjetall = car.data[LINE].value; // avles og lagre linjedata i variabelen linjetall

      car.sendData(2, linjetall); // send linjedata til graf 2

      if (linjetall < -10) { // sjekk om linjen er til venstre for bilen
         car.drive(0, 60);   // sving til venstre
      }
      if (linjetall > 10) { // sjekk om linjen er til høyre for bilen
         car.drive(60, 0);  // sving til høyre
      }
      if (linjetall > -10 && linjetall < 10) { // sjekk om linje er under bilen
         car.drive(60, 60);                    // kjør rett fram
      }
   }
}

// dette er koden for kjøre framover
void w(bool button) { // ikke fjern denne linjen!
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      car.drive(100, 100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      car.drive(0, 0);
   }
}

void a(bool button) { // ikke fjern denne linjen!
   // BEGYNN HER!
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      car.drive(-100, 100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      car.drive(0, 0);
   }
}

void s(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      car.drive(-100, -100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      car.drive(0, 0);
   }
}

void d(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      car.drive(100, -100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      car.drive(0, 0);
   }
}

void e(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      car.drive(100, 0);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      car.drive(0, 0);
   }
}

void q(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      car.drive(0, 100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      car.drive(0, 0);
   }
}

void triangle(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      car.calibrateLine();
      linemode = true; // skru på linjefølging
   }
}

void circle(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      linemode = false; // skru av linjefølging
      car.drive(0, 0);  // stopp bilen
   }
}

void square(bool button) { // ikke fjern denne linjen!
   // skriv kode her
}
