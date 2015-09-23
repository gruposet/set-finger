/* Foram criadas três funções para utilização do LED e BUZZER.
As funções foram criadas com o objetivo de informar por meio de
BIPS e a cor do LED que tipo de ação foi executada na tranca.
As três funções facilitam o desenvolvimento do main code pois não
é necessário enviar parâmetros (se tornaria cansativo), efetuando
somente a chamada da função.
Por exemplo, se um usuário for autenticado correntamente utilize
a função greenColorAlert que irá acender o LED verde e acionar o
BUZZER para indicar a autorização.

Os pinos informados abaixo são os que estão de acordo com o esque-
mático e funcionando corretamente.

Definições:

greenColorAlert - Pode ser usada e chamada por "greenColorAlert();"
Desenvolvida para informar que o usuário foi autenticado para entrada,
cadastrado efetivado, ou alguma ação que precise uma resposta positiva.

redColorAlert - Pode ser usada e chamada por "redColorAlert();"
Desenvolvida para informar que alguma ação não foi autenticada ou
executada corretamente, quando por exemplo, o usuário não tiver
autorização (não cadastrado) para entrar, algum problema de conexão,
cadastro não efetivado com sucesso ou qualquer ação negativa.

blueColorAlert - Pode ser usada e chamada por "blueColorAlert();"
Desenvolvida para informar que o ADMIN, conexão com o servidor, ou
algum processo que está sendo processado ou em andamento.*/

/*Definindo os pinos do LED RGB*/
#define red 26    
#define green 24   
#define blue 22      
/*-----------------------------*/

/*Definindo o pino do buzzer*/
int const buzzer = 30;
/*-------------------------*/

void setup()
{
  pinMode(red, OUTPUT); //sets redPin as output
  pinMode(green, OUTPUT);//sets greenPin as output
  pinMode(blue, OUTPUT);//sets bluePin as output
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
 
}

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
