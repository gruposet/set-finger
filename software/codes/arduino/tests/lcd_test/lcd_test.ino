#include <LiquidCrystal.h> // declara a utilização da biblioteca LiquidCrystal

#define Luz_Fundo  7

//cria um objeto tipo LiquidCrystal que chamei de "lcd" nos pinos citados:
LiquidCrystal lcd(3, 4, 5, 6, 7, 8); //(RS, E, D4, D5, D6, D7)

void setup()
{
lcd.begin(16, 2); // Iniciando o objeto "lcd" de 2 linhas e 16 colunas
pinMode(Luz_Fundo,OUTPUT); //define o pino como saída
digitalWrite(Luz_Fundo,HIGH); // Liga a luz do display.
}

void loop()
{
lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
lcd.print(" engenheirando! ");
lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
lcd.print(" :)  ");
lcd.print(millis()/1000); // escreve os segundos passados.
}