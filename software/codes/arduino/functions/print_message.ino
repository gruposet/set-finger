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
print_message("Usuario","GT-SET");  // a primeira e a segunda string entre aspas("") refere ao conteúdo que será apresentado na primeira e segunda linha do display lcd
delay(2000); 
}


/*A função print_message(); consiste em pegar duas strings e depois imprimí-las nas linhas 1 e 2 do display lCD 16X2 */
void print_message(String row1, String row2){
      lcd.clear(); // limpa o conteúdo no dysplay LCD
      lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
      lcd.print(row1);
      lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
      lcd.print(row2);
}



