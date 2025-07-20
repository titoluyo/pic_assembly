#include <SoftwareSerial.h>

#define LA 2
#define LB 3
#define LC 4
#define LD 5
#define LE 6
#define LF 7
#define LG 8



SoftwareSerial BTserial(11, 10); // RX | TX

char dato;

void setup()
{
   pinMode(LA, OUTPUT);
   pinMode(LB, OUTPUT);
   pinMode(LC, OUTPUT);
   pinMode(LD, OUTPUT);
   pinMode(LE, OUTPUT);
   pinMode(LF, OUTPUT);
   pinMode(LG, OUTPUT);
   Serial.begin(38400);
   BTserial.begin(38400);
   Serial.println("Bluetooth send and receive test.");
}

void loop()
{
   if (Serial.available())
   {
      BTserial.write(Serial.read());
   }
   if (BTserial.available())
   {
      dato = BTserial.read();
      Serial.print(char(dato));
      // LED1
      if (dato == 'A') {
         digitalWrite(LA, LOW);
      }
      if (dato == 'B') {
         digitalWrite(LB, LOW);
      }
      if (dato == 'C') {
         digitalWrite(LC, LOW);
      }
      if (dato == 'D') {
         digitalWrite(LD, LOW);
      }
      if (dato == 'E') {
         digitalWrite(LE, LOW);
      }
      if (dato == 'F') {
         digitalWrite(LF, LOW);
      }
      if (dato == 'G') {
         digitalWrite(LG, LOW);
      }
   }
}