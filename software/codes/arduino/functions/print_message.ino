/*A função desenvolvida pega strings enviadas no código principal
e as exibe no dysplay LCD 16X2, seu formato é print_message
("string1","string2"); sendo que a string 1 utilizará a primeira
linha e s string 2 segunda linha, a lógica disso parte da ideia 
de que duas palavras podem utilizar mais de 16 caracteres(tamanha
da linha) logo eles serão divididos em duas(Cabe ao programador 
atentar-se a este detalhe), caso utize-se apenas da primeira 
linha a formato da função fica print_message("string1",""); haja 
vista que nossa função void obriga a receber duas strings(mesmo 
uma delas sendo nula.)*/

#include <LiquidCrystal.h> // declara a utilização da biblioteca LiquidCrystal

#define luz_fundo  7

//cria um objeto tipo LiquidCrystal que chamei de "lcd" nos pinos citados:
LiquidCrystal lcd(3, 4, 5, 6, 7, 8); //(RS, E, D4, D5, D6, D7)

void setup()
{
lcd.begin(16, 2); // Iniciando o objeto "lcd" de 2 linhas e 16 colunas
pinMode(luz_fundo,OUTPUT); //define o pino como saída
digitalWrite(luz_fundo,HIGH); // Liga a luz do display.
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



