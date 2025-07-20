#include <SoftwareSerial.h>

#define LED1 2
#define LED2 4



SoftwareSerial BTserial(11, 10); // RX | TX

char dato;

void setup()
{
   pinMode(LED1, OUTPUT);
   pinMode(LED2, OUTPUT);
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
      if (dato == '0') {
         digitalWrite(LED1, LOW);
      }
      if (dato == '1') {
         digitalWrite(LED1, HIGH);
      }
      // LED2
      if (dato == '3') {
         digitalWrite(LED2, LOW);
      }
      if (dato == '4') {
         digitalWrite(LED2, HIGH);
      }

      // LED 1 & 2
      if (dato == 'B') {
         digitalWrite(LED1, LOW);
         digitalWrite(LED2, LOW);
      }
      if (dato == 'A') {
         digitalWrite(LED1, HIGH);
         digitalWrite(LED2, HIGH);
      }
   }
}