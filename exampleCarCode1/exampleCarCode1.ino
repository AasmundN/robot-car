// Dette er eksempelkode 1, som er ment som en enkel måte å fullføre oppgavene
// på.

// Merk at koden her kun er ment som et eksempel og ikke en fasit.
// Det er mange måter å løse oppgavene på.

#include "Car.h"

Car car("nettverksnavn", "nettverkspassord");

void setup() {
   car.initCar(BLACK); // BLACK for svart linje og WHITE for hvit linje
}

// lag variabler her
bool linemode = false; // variabel for å skru av og på linjefølger
int linjetall;         // variabel for å lagre dataen fra linjesensorene

void loop() { // ikke fjern denne linjen!

   // skriv kode for å sende data her
   sendData(1, readNTC());  // send temperaturdata til graf 1
   sendData(2, readProx()); // send avstandsdata til graf 2

   if (linemode == true) { // sjekk om linjefølgeren skal være på
      // skriv linjefølger-kode her
      linjetall = readLine(); // avles og lagre linjedata i variabelen linjetall

      sendData(3, linjetall); // send linjedata til graf 3

      if (linjetall < -10) { // sjekk om linjen er til venstre for bilen
         drive(0, 60);       // sving til venstre
      }
      if (linjetall > 10) { // sjekk om linjen er til høyre for bilen
         drive(60, 0);      // sving til høyre
      }
      if (linjetall > -10 && linjetall < 10) { // sjekk om linje er under bilen
         drive(60, 60);                        // kjør rett fram
      }
   }
}

// dette er koden for kjøre framover
void w(bool button) { // ikke fjern denne linjen!
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      drive(100, 100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      drive(0, 0);
   }
}

void a(bool button) { // ikke fjern denne linjen!
   // BEGYNN HER!
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      drive(-100, 100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      drive(0, 0);
   }
}

void s(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      drive(-100, -100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      drive(0, 0);
   }
}

void d(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      drive(100, -100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      drive(0, 0);
   }
}

void e(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      drive(100, 0);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      drive(0, 0);
   }
}

void q(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      // dette skjer når knappen trykkes ned
      drive(0, 100);
   }
   if (button == UP) {
      // dette skjer når knappen slippes opp
      drive(0, 0);
   }
}

void triangle(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      linemode = true; // skru på linjefølging
   }
}

void circle(bool button) { // ikke fjern denne linjen!
   // skriv kode her
   if (button == DOWN) {
      linemode = false; // skru av linjefølging
      drive(0, 0);      // stopp bilen
   }
}

void square(bool button) { // ikke fjern denne linjen!
   // skriv kode her
}
