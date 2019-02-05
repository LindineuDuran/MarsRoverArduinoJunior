// Esse é o código do robô
// certifique-se de incluir as Bibliotecas do NewPing e do Servo antes de fazer o upload. Caso contrário, ele mostrará erro
// Controle 2 motores DC usando Ponte H L298N

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[Z] |   B4
         | [ ]3.3V           MOSI/D11[S]~|   B3
         | [ ]V.ref     ___    SS/D10[S]~|   B2
    C0   | [ ]A0       / N \       D9[ ]~|   B1
    C1   | [ ]A1      /  A  \      D8[L] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [D]A4/SDA               D5[ ]~|   D5
    C5   | [D]A5/SCL               D4[ ] |   D4
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

#include <NewPing.h>
#include <Servo.h>

//====================
// [D] Distance Sensor
//====================
#define TRIGGER_PIN  A4
#define ECHO_PIN     A5
#define MIN_DISTANCE 20
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//================================
// [Z] Buzzer conectado ao pino 12
//================================
int buzzer = 12;

//=============================================================
// [S] pinos de controle dos servos conectados aos pino 10 e 11
//=============================================================
#define rightServo 10
#define leftServo 11
Servo leftDrive;  // create servo object to control a servo
Servo rightDrive; //another servo object for the left side

//=========================================
// [L] Criando variável para o led vermelho
//=========================================
int LedEstado = 8;

float duration, distance;
int iterations = 5;

void setup()
{
  //=============================
  // Pino 4 do arduino como saída
  //=============================
  pinMode(LedEstado, OUTPUT);

  //===========================================
  // Sets the baud for serial data transmission
  //===========================================
  Serial.begin(9600);

  //==============================
  // Pino 12 do arduino como saída
  //==============================
  pinMode(buzzer, OUTPUT);
}

void loop()
{
  distance = readPing();

  if (distance >= MAX_DISTANCE || distance <= 2)
  {
    Serial.println("Out of range");
    //playBuzzer();
    piscaLed();
    driveBackward();
  }
  else if (distance <= MIN_DISTANCE)
  {
    Serial.print(distance);
    Serial.println(" cm");

    leftDrive.detach();
    rightDrive.detach();

    turnRight();
  }
  else
  {
    Serial.print(distance);
    Serial.println(" cm");

    driveForward();
  }
}

int readPing()
{
  duration = sonar.ping_median(iterations);

  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance = (duration / 2) * 0.0343;

  return distance;
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

void playBuzzer()
{
  //==============
  //aqui sai o som
  //==============
  tone(buzzer, 300, 300);
  pausa(500);

  //==============
  //aqui sai o som
  //==============
  tone(buzzer, 100, 300);
  pausa(500);

  //==============
  //aqui sai o som
  //==============
  tone(buzzer, 900, 300);
  pausa(500);
}

void piscaLed()
{
  digitalWrite(LedEstado, HIGH); //Acende o LED
  pausa(200);
  digitalWrite(LedEstado, LOW); //Apaga o LED
  pausa(200);
  digitalWrite(LedEstado, HIGH); //Acende o LED
  pausa(200);
  digitalWrite(LedEstado, LOW); //Apaga o LED
  pausa(200);
}

//drives straight for 1 second
void driveForward()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(0);
  rightDrive.write(180);
}

//drives straight backward for 1 second
void driveBackward()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(180);
  rightDrive.write(0);

  pausa(100);
}

//turns right about 90 degrees
void turnRight()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(180);
  rightDrive.write(180);

  pausa(600);

  leftDrive.detach();
  rightDrive.detach();
}

//turns left about 90 degrees
void turnLeft()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(0);
  rightDrive.write(0);

  pausa(600);

  leftDrive.detach();
  rightDrive.detach();
}
