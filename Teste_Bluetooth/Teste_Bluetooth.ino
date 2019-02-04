//Programa :Testa Bluetooth que Toca Buzina e Música.

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[Z] |   B4
         | [ ]3.3V           MOSI/D11[ ]~|   B3
         | [ ]V.ref     ___    SS/D10[ ]~|   B2
    C0   | [ ]A0       / N \       D9[ ]~|   B1
    C1   | [ ]A1      /  A  \      D8[L] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[B]~|   D6
    C4   | [ ]A4/SDA               D5[B]~|   D5
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

#include <SoftwareSerial.h>

//=========================================
// [L] Criando variável para o led vermelho
//=========================================
int LedEstado = 8;

//========================================================
// [B] Bluetooth
// Definição de um objeto SoftwareSerial.
// Usaremos os pinos 5 e 6, como RX e TX, respectivamente.
// Isto evita o erro do Avrdude
//========================================================
SoftwareSerial serial(5, 6);

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

#define LedEstadoCycle 100U
unsigned long LedEstadoLastMillis = 0;
boolean LedEstadoState = false;
char cmd; //Armazena os caracteres enviados pela portal Serial

void setup()
{
  // put your setup code here, to run once:

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
  // put your main code here, to run repeatedly:

  // Verifica se chegou algum caractere pela porta serial.
  if (serial.available())
  {
    //==============================================
    // Variável 'cmd' recebe o valor da porta Serial
    //==============================================
    cmd = serial.read();

    switch (cmd)
    {
      case 'A' : //Acende o LED
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Acende o LED");
        digitalWrite(LedEstado, HIGH);
        break;

      case 'a' : //Apaga o LED
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Apaga o LED");
        digitalWrite(LedEstado, LOW);
        break;

      case 'P' : //Se 'P' for recebido, Pisca o Led de Estado
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Pisca LED");
        piscaLed();
        break;

      case 'B' : //Se 'B' for recebido, Toca a Buzina
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Play the Buzzer");

        //Play the Buzzer
        playBuzzer();

        break;

      case 'M' : //Se 'M' for recebido, Toca a Música
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Play the Music");

        //Play Super Mario Theme
        playSuperMarioTheme();
        break;
    }
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

void playBuzzer()
{
  /*
    o número 12 indica que o pino positivo do buzzer está na porta 12
    o número 300 é a frequência que será tocada
    o número 300 é a duração do som
  */

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

void playSuperMarioTheme()
{
  //for para tocar as 6 primeiras notas, começando no 0 ate 6 ++ incrementado
  for (int nota = 0; nota < 6; nota++)
  {
    int duracaodanota = duracaodasnotas[nota];
    tone(buzzer, melodia[nota], duracaodanota);

    //pausa depois das notas
    int pausadepoisdasnotas[] = {150, 300, 300, 100, 300, 550, 575};
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
