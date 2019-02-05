//------------------------------------------------------------------------------------------
// Esse é o código do robô
// certifique-se de incluir as Bibliotecas do NewPing e do Servo antes de fazer o upload. Caso contrário, ele mostrará erro
// Controle 2 servo motores alterados como propulsão de um robô usando Servo

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
    C3   | [ ]A3       \_0_/       D6[B]~|   D6
    C4   | [D]A4/SDA               D5[B]~|   D5
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
#include <Servo.h>//Loads commands to create Servo objects which generate PWM signals
#include <SoftwareSerial.h>

//====================
// [D] Distance Sensor
//====================
#define TRIGGER_PIN  A4
#define ECHO_PIN     A5
#define MIN_DISTANCE 20
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//========================================================
// [B] Bluetooth
// Definição de um objeto SoftwareSerial.
// Usaremos os pinos 5 e 6, como RX e TX, respectivamente.
// Isto evita o erro do Avrdude
//========================================================
#define RX 5
#define TX 6
SoftwareSerial serial(RX, TX);

//=========================================
// [L] Criando variável para o led vermelho
//=========================================
int LedEstado = 8;

//==================================================
// [S] pino de controle do servo conectado ao pino 7
//==================================================
#define rightServo 10
#define leftServo 11
Servo leftDrive;  // create servo object to control a servo
Servo rightDrive; //another servo object for the left side

//================================
// [Z] Buzzer conectado ao pino 12
//================================
int buzzer = 12;

//=======================
// melodia do MARIO THEME
//=======================
int melodia[] = {660, 660, 660, 510, 660, 770, 380};

//=====================
// duração de cada nota
//=====================
int duracaodasnotas[] = {100, 100, 100, 100, 100, 100, 100};

//pausa depois das notas
int pausadepoisdasnotas[] = {150, 300, 300, 100, 300, 550, 575};

//=================
// Varáveis Globais
//=================
const long interval = 20; // interval at which to blink (milliseconds)
int autoMode = 0; //Indica quando o modo autônomo está ativado.
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
  serial.begin(9600);

  //==============================
  // Pino 12 do arduino como saída
  //==============================
  pinMode(buzzer, OUTPUT);
}

void loop()
{
  //Criando uma variável do tipo caracter para receber o comando
  static char cmd;

  if (autoMode == 0) //MODO BLUETOOTH
  {
    //Variável 'cmd' recebe o valor da porta Serial
    cmd = serial.read();

    pausa(interval);

    switch (cmd)
    {
      case 'A' : //Se 'A' for recebido, define o modo autônomo
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Autonomous Mode");

        // Define Modo Autônomo
        autoMode = 1;
        break;

      case 'a' : //Se 'a' for recebido, define o modo manual
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Manual Mode");

        // Define Modo Manual
        autoMode = 0;

        //Move Stop
        moveStop();

        //Play the Buzzer
        playBuzzer();
        break;

      case 'F' : //Se 'F' for recebido, vai para Frente
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Forward");

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        // Move Forward
        driveForward();
        break;

      case 'T' : //Se 'T' for recebido, vai para Trás
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Backward");

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        //Move Backward
        driveBackward();
        break;

      case 'E' : //Se 'E' for recebido, vira para Esquerda
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Left");

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        //Move Left
        turnLeft();
        break;

      case 'D' : //Se 'D' for recebido, vira para Direita
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Right");

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        //Move Right
        turnRight();
        break;

      case 'P' : //Se 'P' for recebido, Pára o Movimento
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Stop the Move");

        //Apaga o LED.
        digitalWrite(LedEstado, LOW);

        //Move Stop
        moveStop();
        break;

      case 'B' : //Se 'B' for recebido, Toca a Buzina
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Play the Buzzer");

        //Play the Buzzer
        playBuzzer();
        break;

      case 'M' : //Se 'M' for recebido, Toca a Música
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Play the Music");

        //Play Super Mario Theme
        playSuperMarioTheme();
        break;
    }
  }
  else if (autoMode == 1) //MODO AUTÔNOMO
  {
    //Variável 'cmd' recebe o valor da porta Serial
    cmd = serial.read();

    switch (cmd)
    {
      case 'A' : //Se 'A' for recebido, define o modo Autônomo
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Autonomous Mode");

        // Define Modo Autônomo
        autoMode = 1;
        break;

      case 'a' : //Se 'a' for recebido, define o modo manual
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Manual Mode");

        // Define Modo Manual
        autoMode = 0;

        //Move Stop
        moveStop();

        //Play the Buzzer
        playBuzzer();
        break;
    }
  }

  if (autoMode == 1)
  {
    digitalWrite(LedEstado, HIGH); //Acende o LED quando o robô entrar  no modo autônomo.
    obstacleavoidance();
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

void obstacleavoidance()
{
  distance = readPing();

  if (distance >= MAX_DISTANCE || distance <= 2)
  {
    Serial.println("Out of range");
    //playBuzzer();
    piscaLed();
    driveBackward();

    pausa(100);

    leftDrive.detach();
    rightDrive.detach();
  }
  else if (distance <= MIN_DISTANCE)
  {
    Serial.print(distance);
    Serial.println(" cm");

    leftDrive.detach();
    rightDrive.detach();

    turnRight();

    pausa(600);

    leftDrive.detach();
    rightDrive.detach();
  }
  else
  {
    Serial.print(distance);
    Serial.println(" cm");

    driveForward();
  }
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

void moveStop()
{
  //Move Stop
  leftDrive.detach();
  rightDrive.detach();
}

//drives straight forward
void driveForward()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(0);
  rightDrive.write(180);
}

//drives straight backward
void driveBackward()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(180);
  rightDrive.write(0);
}

//turns right about 90 degrees
void turnRight()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(180);
  rightDrive.write(180);
}

//turns left about 90 degrees
void turnLeft()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(0);
  rightDrive.write(0);
}

void playBuzzer()
{
  /*
    o número 12 indica que o pino positivo do buzzer está na porta 12
    o número 300 é a frequência que será tocado
    o número 300 é a duração do som
  */
  //aqui sai o som
  tone(buzzer, 300, 300);

  //delay(500)
  pausa(500);

  //aqui sai o som
  tone(buzzer, 100, 300);

  //delay(500);
  pausa(500);

  //aqui sai o som
  tone(buzzer, 900, 300);

  //delay(500);
  pausa(500);
}

void playSuperMarioTheme()
{
  //for para tocar as 6 primeiras notas começando no 0 até 6 ++ incrementado
  for (int nota = 0; nota < 6; nota++)
  {
    int duracaodanota = duracaodasnotas[nota];
    tone(buzzer, melodia[nota], duracaodanota);
    pausa(pausadepoisdasnotas[nota]);
  }

  noTone(buzzer);
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
//end code---------------------------------------------
