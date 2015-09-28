/*>>>>>>>>>>>>>>>>>>>>>>>Bibliotecas e pinos<<<<<<<<<<<<<<<<<<<*/
/*-----------------------Definindo ethernet--------------------*/
#include <SPI.h>
#include <Ethernet.h>
/byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,14,141); // IP Adress to our Server
const int serverPort=7000;
// Inicializa biblioteca ethernet cliente
EthernetClient client;
/*-------------Definindo biblioteca fingerprint----------------*/
#include <Adafruit_Fingerprint.h>
//decisão de biblioteca a ser utilizada de acordo com a versão da IDE
#if ARDUINO >= 100
 #include <SoftwareSerial.h>
 SoftwareSerial mySerial(11, 12);
#else
 #include <NewSoftSerial.h>
 NewSoftSerial mySerial(11, 12);
#endif
//Declaração do objeto para comunicação.
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
/*-------------Definindo biblioteca LCD e seus pinos-----------*/
#include <LiquidCrystal.h> // declara a utilização da biblioteca LiquidCrystal
#define luz_fundo  7
//cria um objeto tipo LiquidCrystal que chamei de "lcd" nos pinos citados:
LiquidCrystal lcd(3, 4, 5, 6, 7, 8); //(RS, E, D4, D5, D6, D7)
/*-------------Definindo os pinos do LED RGB-------------------*/
#define red 26    //define o pino do LED como vermelho
#define green 24 // define o pino do LED como verde
#define blue 22 //define o pino do LED como azul    
/*-------------Definindo o pino do buzzer---------------------*/
int const buzzer = 30;
/*------------------------------------------------------------*/
/*-------------Definindo teclado 4x3(Keypad) e pinos----------*/
#include <Keypad.h>
const byte ROWS = 4; //4 linhas
const byte COLS = 3; //3 colunas
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}};
byte rowPins[ROWS] = {44, 42, 40, 38}; //define os pinos de linha do teclado
byte colPins[COLS] = {36, 34, 32}; //define os pinos de coluna do teclado
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>SETUP<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void setup()
{
Serial.begin(9600); 
finger.begin(57600);// Define a taxa de dados para a porta serial do sensor

lcd.begin(16, 2); //objeto "lcd" de 2 linhas e 16 colunas
pinMode(luz_fundo,OUTPUT); //define o pino como saída
digitalWrite(luz_fundo,HIGH); // Liga a luz do display.

pinMode(red, OUTPUT); 	//define red Pin como saída
pinMode(green, OUTPUT);//define green Pin como saída
pinMode(blue, OUTPUT);//define bluePin como saída

pinMode(buzzer, OUTPUT); //define pino do buzzer como saída
	
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>LOOP<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void loop()
{
	
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>funções<<<<<<<<<<<<<<<<<<<<<<<<<*/
//função print_message(), imprime duas strings on LCD screen
void print_message(String row1, String row2){
      lcd.clear(); // limpa o conteúdo no dysplay LCD
      lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
      lcd.print(row1);
      lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
      lcd.print(row2);
}
//função redColorAlert para ligar LED vermelho e um som padrão
void redColorAlert(){ //significa usuário não autorizado ou acesso negado
  short int i=0;
  for (i=0;i<5;i++){
    	digitalWrite(blue,LOW);
    	digitalWrite(green,LOW);
    	digitalWrite(red,HIGH);
    	tone(buzzer, 1400, 1000);
    	delay(1000);
    	digitalWrite(red,LOW);
    	delay(100);
  	}
}
//função greenColorAlert para ligar LED verde e um som padrão
void greenColorAlert(){ //significa usuário autorizado ou tudo ok!
  short int i=0;
  for (i=0;i<5;i++){
  		digitalWrite(blue,LOW);
  		digitalWrite(green,HIGH);
  		digitalWrite(red,LOW);
  		tone(buzzer, 750, 500);
  		delay(1000);
  		digitalWrite(green,LOW);
  		delay(100);
   	}
}
//função blueColorAlert para ligar LED azul e um som padrão
void blueColorAlert(){ //significa usuário admin autorizado ou conexão com o servidor.
  short int i=0;
  for (i=0;i<5;i++){
  	 digitalWrite(blue,HIGH);
  	 digitalWrite(green,LOW);
  	 digitalWrite(red,LOW);
  	 tone(buzzer, 500, 300);
  	 delay(100);
  	 digitalWrite(blue,LOW);
  	 delay(1000);
 	}
}
