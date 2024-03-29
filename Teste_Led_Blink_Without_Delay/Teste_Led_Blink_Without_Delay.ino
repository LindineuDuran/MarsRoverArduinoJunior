/* Blink without Delay

  Turns on and off a light emitting diode (LED) connected to a digital
  pin, without using the delay() function. This means that other code
  can run at the same time without being interrupted by the LED code.

  The circuit:
  LED attached from pin 8 to ground.
  Note: on most Arduinos, there is already an LED on the board
  that's attached to pin 13, so no hardware is needed for this example.

  created 2005
  by David A. Mellis
  modified 8 Feb 2010
  by Paul Stoffregen
  modified 11 Nov 2013
  by Scott Fitzgerald
  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
*/

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[ ]~|   B3
         | [ ]V.ref     ___    SS/D10[ ]~|   B2
    C0   | [ ]A0       / N \       D9[ ]~|   B1
    C1   | [ ]A1      /  A  \      D8[L] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [ ]A4/SDA               D5[ ]~|   D5
    C5   | [ ]A5/SCL               D4[ ] |   D4
         | [ ]A6              INT1/D3[ ]~|   D3
         | [ ]A7              INT0/D2[ ] |   D2
         | [ ]5V                  GND[ ] |
    C6   | [ ]RST                 RST[ ] |   C6
         | [ ]GND   5V MOSI GND   TX1[ ] |   D0
         | [ ]Vin   [ ] [ ] [ ]   RX1[ ] |   D1
         |          [ ] [ ] [ ]          |
         |          MISO SCK RST         |
         | NANO-V3                       |
         +-------------------------------+

         http://busyducks.com/ascii-art-arduinos
*/

// constants won't change. Used here to set a pin number :
const int ledPin = 8; // the number of the LED pin
// constants won't change :
const long interval = 1000; // interval at which to blink (milliseconds)

void setup()
{
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, HIGH);

  pausa(1000);

  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, LOW);

  pausa(1000);
}

void pausa(unsigned int milisegundos)
{
  volatile unsigned long compara = 0;
  volatile int contador = 0;
  do
  {
    if (compara != millis())
    {
      contador++;
      compara = millis();
    }
  } while (contador <= milisegundos);
  return;
}
