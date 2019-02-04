//Controle 2 servo motores alterados como propulsão de um robô

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[S]~|   B3
         | [ ]V.ref     ___    SS/D10[S]~|   B2
    C0   | [ ]A0       / N \       D9[ ]~|   B1
    C1   | [ ]A1      /  A  \      D8[ ] |   B0
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

//------------------------------------------------------------------------------------------

#include <Servo.h>//Loads commands to create Servo objects which generate PWM signals

//==================================================
// [S] pino de controle do servo conectado ao pino 7
//==================================================
#define rightServo 10
#define leftServo 11
Servo leftDrive;  // create servo object to control a servo
Servo rightDrive; //another servo object for the left side

void setup()
{
  leftDrive.attach(leftServo);  // attaches the servo on pin 9 to the servo object
  rightDrive.attach(rightServo);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
  //example routine
  driveForward();
  driveBackward();
  turnRight();
  turnLeft();
}

//the following functions are examples, you could easily make the robot
//move on curved paths or at varying speeds by changing these numbers
//0 is full forward, 90 is stop and 180 is full reverse. The case may be the
//opposite for your build

//drives straight for 1 second
void driveForward()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(0);
  rightDrive.write(180);

  delay(5000);

  leftDrive.detach();
  rightDrive.detach();
}

//drives straight backward for 1 second
void driveBackward()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(180);
  rightDrive.write(0);

  delay(5000);

  leftDrive.detach();
  rightDrive.detach();
}

//turns right about 90 degrees
void turnRight()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(0);
  rightDrive.write(0);

  delay(450);

  leftDrive.detach();
  rightDrive.detach();
}

//turns left about 90 degrees
void turnLeft()
{
  leftDrive.attach(leftServo);  // attaches the Left Servo on pin 11 to the servo object
  rightDrive.attach(rightServo);  // attaches the Right Servo on pin 10 to the servo object

  leftDrive.write(180);
  rightDrive.write(180);

  delay(450);

  leftDrive.detach();
  rightDrive.detach();
}

//end code---------------------------------------------
