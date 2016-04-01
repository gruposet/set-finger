/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Bibliotecass e Configurações >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/*-----------------------inclusão de biblioteca ethernet shield--------------------*/
#include <SPI.h> 
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,14,198); // endereço IP da máquina servidor node.js
const int serverPort=7000; //porta fonte pode ser de 1-65535

// Inicializa biblioteca ethernet cliente
EthernetClient client;

/*-------------inclusão de biblioteca do sensor fingerprint----------------*/
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

/*-------------inclusão de biblioteca de LCD-----------*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);



/*-------------Definindo os pinos do LED RGB-------------------*/
#define red 26    //define o pino do LED como vermelho
#define green 24   // define o pino do LED como verde
#define blue 22   //define o pino do LED como azul


/*-------------Definindo o pino do buzzer e relé---------------*/
int const buzzer = 30;
int const rele = 28;
/*------------------------------------------------------------*/


/*-------------Inclusão de biblioteca e configuração do teclado matricial 4x3(Keypad)----------*/
#include <OnewireKeypad.h>

char KEYS[]= {
  '3','2','1',//'A',
  '6','5','4',//'B',
  '9','8','7',//'C',
  '#','0','*',//'D'
};

OnewireKeypad <Print, 12 > Keypad(Serial, KEYS, 4, 3, A3, 4700, 1000 );



/*-------------------------Declaração de variáveis de estado (flag)-------------------------*/
#include <ArduinoJson.h>
String SETFINGER_HWID = "GT-SET";       //Identificação do dispositivo SF para o servidor
String welcomeName;                     //Nome de boas-vindas que o dispositivo SF recebe do serivodr
bool AuthOk = false;                    //Viariável que identifica se o dispositivo SF foi autenticado com sucesso pelo servidor
bool waitingResponse = false;           //Variável que indica se o dispositivo SF está aguardando alguma resposta do servidor
bool waitingKeyboard = false;           //Variável que indica se o dispositivo SF está aguardando algum comando do teclado
bool waitingID = false;                 //Variável que indica se o dispositivo SF está aguardando que o servidor responda o ID a ser atribuido para a digital que será gravada no sensor
bool fullCircle = true;                 //Variável que indica se todos os passos foram executados, para retornar ao estado principal

String userName; //armazena o nome fornecido pelo servidor até

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SETUP >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void setup(){
  Serial.begin(115200);
  finger.begin(57600);// Define a taxa de dados para a porta serial do sensor

  Keypad.SetHoldTime(50);  // Key held time in ms
  Keypad.SetDebounceTime(30); // Key Debounce time in ms
  
  lcd.begin (16, 2);
  lcd.setBacklight(HIGH);

  pinMode(red, OUTPUT);   //define red Pin como saída
  pinMode(green, OUTPUT);//define green Pin como saída
  pinMode(blue, OUTPUT);//define bluePin como saída

  pinMode(buzzer, OUTPUT); //define pino do buzzer como saída
  pinMode(rele, OUTPUT); //define pino do relé como saída

  Ethernet.begin(mac);
  Serial.begin(115200);
  while(!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  delay(1000);
  serverConnect();
  

}


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LOOP >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void loop(){
  
  if(AuthOk){

    if(waitingKeyboard){            //Função usada para o menu de administrador, ela recebe os comandos do teclado e executa uma função a
      char tecla = Keypad.Key_State();   //partir de uma tecla específica
      Serial.println(tecla);
      if (tecla == 3){
         tecla=leitura();
         Serial.print("tecla: ");
         Serial.println(tecla);
         if(tecla == (char)49){        //Se for pressionada a tecla 1, o relé é acionado.
            Serial.print("opendoor");
            Serial.print(tecla);
            openDoor();
            fullCircle = true;
        }
        if(tecla == (char)50){
          registerFinger(0);        //Se for pressionada a tecla 2, a função de registro de usuário é chamada
        }
        waitingKeyboard = false;    //No final de cada ciclo acima, a variável de flag do teclado é setada como falsa.
      }
    }
    if(fullCircle && !waitingKeyboard && !waitingID){
      lcd.clear(); // limpa o conteúdo no dysplay LCD
      lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
      lcd.print("BEM-VINDO(A) AO");
      lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
      lcd.print(welcomeName);
      fullCircle = false;
    }
    if(!waitingResponse && !waitingKeyboard && !waitingID){     //Estado padrão da tranca, sempre executado enquanto a tranca estiver ociosa
      int fingerID = readFinger();
      if(fingerID){
        sendFingerID(fingerID);
        waitingResponse = true;
      }
    }
  }


  

  readTCPStream();              //Chamada de função que lê constantemente o que é enviado pelo servidor

  if (!client.connected()) {    //É executado quando a conexão entre a tranca e o servidor é perdida
      Serial.println("Disconnected!");
      serverConnect();

  }
}

/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||FUNÇÕES|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/


void serverConnect() {  //estabelece uma conexão TCP com o servidor e envia a mensagem de "boas-vindas"
  client.stop();
  AuthOk = false;
  Serial.println("Connecting...");
  printMessage("CONECTANDO...","");
  if(client.connect(serverIP, serverPort)) {
    Serial.println("Connected!");

    StaticJsonBuffer<64> jsonBuffer;                    //Etapa de criação de uma string JSON
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "conn";
    root["hwid"] = SETFINGER_HWID;
    char buffer[64];
    root.printTo(buffer, sizeof(buffer)); //transforma um objeto json em uma string json e armazena na variavel buffer
    client.print(buffer);
    
    fullCircle = false;
    waitingResponse = false;
    waitingKeyboard = false;
  } else {
    printMessage("ERRO", "ID: #0001");
    Serial.println("Connection failed!");
    redColorAlert();
  }
}





/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Grava digitais no sensor fingerprint >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
uint8_t addFinger(uint8_t id) {
  Serial.println("Procurando sensor...");
  if(finger.verifyPassword()) {
    Serial.println("Sensor FingerPrint Encontrado!");
    delay(1000);
  }
  else {
    Serial.println("Nenhum sensor foi encontrato. Conecte um sensor!");
    delay(1000);
    addFinger(0);
  }
  uint8_t digital;
  
  
  lcd.clear(); // limpa o conteúdo no dysplay LCD
  lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
  lcd.print("Coloque o dedo");
  delay(1000);
  
  digital= finger.getImage();
  while(digital != FINGERPRINT_OK) {
    digital= finger.getImage();
  }
  
  digital = finger.image2Tz(1);
  if(digital != FINGERPRINT_OK)
    return -1; //retorna -1 e sai da função addFinger() somente se a conversão da imagem da primeira digital não tiver ocorrido com sucesso
  
  digital= finger.getImage();
  while(digital != FINGERPRINT_OK) {
     digital= finger.getImage();
  }
  
  //converte a imagem da segunda digital lida em arquivo caracter
  digital = finger.image2Tz(2);
  if(digital != FINGERPRINT_OK)  return -1; //retorna -1 e sai da função addFinger() somente se a conversão da imagem da segunda digital não tiver ocorrido com sucesso
  //criação do modelo para a digital lida
  
  digital = finger.createModel();
  if(digital != FINGERPRINT_OK) {
    lcd.clear(); // limpa o conteúdo no dysplay LCD
    lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
    lcd.print("TENTE NOVAMENTE!");
    client.print("{\"type\":\"registerfail\"}");
    return -1;
  }
  //Serial.println("MODELO CRIADO COM SUCESSO. DIGITAIS COMPATIVEIS");
  delay(1000);
  //gravação do modelo da digital
  
  digital = finger.storeModel(id);
  
  lcd.clear(); // limpa o conteúdo no dysplay LCD
  lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
  lcd.print("Usuario OK");
  client.print("{\"type\":\"registerok\"}");
  greenColorAlert();
  delay(1000);
  return 0;
  
}



/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Leitura e autenticação de digitais >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
int readFinger() {
  //Serial.println("Procurando sensor...");
  if(finger.verifyPassword()) {
    //Serial.println("Sensor FingerPrint Encontrado!");
  }
  else {
    //Serial.println("Nenhum sensor foi encontrato. Conecte um sensor!");
  }
  uint8_t digital;
  //Serial.println("Aguardando uma digital valida. Coloque seu dedo...");
  digital= finger.getImage();
  if(digital != FINGERPRINT_OK) {
    digital= finger.getImage();
  }

  if(digital == FINGERPRINT_OK) {
      digital = finger.image2Tz();
      if(digital != FINGERPRINT_OK)
        return -1; //retorna -1 e sai da função addFinger() somente se a conversão da imagem da digital não tiver ocorrido com sucesso
      digital = finger.fingerFastSearch();
      if(digital != FINGERPRINT_OK){
        //Serial.println("DIGITAL NAO ENCONTRADA. TENTE NOVAMENTE!");
        return -1; //retorna -1 e sai da função addFinger() somente se a digital for reconhecida entre as digitais presentes no sensor
      }
    return finger.fingerID;
  }
  return 0;
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Exclusão de digitais >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
uint8_t deleteFinger(uint8_t id) {
  uint8_t digital;
  digital = finger.deleteModel(id);
  if (digital == FINGERPRINT_OK) {
    Serial.print("Digital ID:");
    Serial.print(id);
    Serial.println(" Excluida com Sucesso!");
  } else {
    Serial.print("ERRO!NÃO FOI POSSIVEL EXCLUIR A DIGITAL ID: ");
    Serial.println(id);
  }
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Imprime mensagem no display LCD >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void printMessage(String row1, String row2) {
      lcd.clear(); // limpa o conteúdo no dysplay LCD
      lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
      lcd.print(row1);
      lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
      lcd.print(row2);
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Led e Buzzer - sinal alerta vermelho >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void redColorAlert() { //significa usuário não autorizado ou acesso negado
  short int i=0;
  for (i=0;i<2;i++){
      digitalWrite(blue,LOW);
      digitalWrite(green,LOW);
      digitalWrite(red,HIGH);
      tone(buzzer, 1400, 1000);
      delay(1000);
      digitalWrite(red,LOW);
      delay(100);
    }
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Led e Buzzer - sinal de alerta verde >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void greenColorAlert() { //significa usuário autorizado ou tudo ok!
  short int i=0;
  for (i=0;i<2;i++){
      digitalWrite(blue,LOW);
      digitalWrite(red,LOW);
      digitalWrite(green,HIGH);
      tone(buzzer, 750, 500);
      delay(500);
      digitalWrite(green,LOW);
      delay(50);
    }
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Led e Buzzer - sinal de alerta azul >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void blueColorAlert() { //significa usuário admin autorizado ou conexão com o servidor.
  short int i=0;
  for (i=0;i<2;i++) {
     digitalWrite(green,LOW);
     digitalWrite(red,LOW);
     digitalWrite(blue,HIGH);
     tone(buzzer, 500, 300);
     delay(25);
     digitalWrite(blue,LOW);
     delay(250);
  }
}




void handleConnEvents(const char* msg, String name){ //trata a mensagem de conexão recebida do servidor
    if(strcmp(msg, "fail") == 0){                    //Executado quando há falha de autenticação com o servidor
          printMessage("ERRO", "ID: #0002");
          redColorAlert();
          Serial.println("Auth fail! Reconnecting...");
          delay(5000);
          AuthOk = false;
          client.stop();
        }
        else if(strcmp(msg, "ok") == 0){            //Executado quando a autenticação ocorrer com sucesso
          Serial.println("Auth OK! Ready to use...");
          printMessage("BEM-VINDO(A) AO", name);
          AuthOk = true;
        }
}


void readTCPStream(){ //lê o JSON enviado pelo servidor e armazena num array de char para ser tratado pela lib
    char stream[128]; //também trata as mensagens recebidas direcionando para cada função específica
    bool flag = false;
    int j = 0;
    if(client.available()){                         //Limpa o array da mensagem
      for(int i=0; i<128; i++)
        stream[i]=(char)0;
    }
    while(client.available() && !flag) {            //Armazena no array o conteudo recebido do servidor
        stream[j] = client.read();
        char lastChar = stream[j];
        j++;
        if(client.available()==0 || lastChar == 125) // 125 --> }
            flag = !flag;
    }
    if(flag){                                       //Executado quando a variável flag é verdadeira, ou seja, quando há conteúdo lido
      Serial.println(stream);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(stream);
      const char* type = root["type"];
      if(strcmp(type, "conn") == 0){                //Trata mensagens do tipo 'conn'
        const char* auth = root["auth"];
        const char* name = root["name"];
        welcomeName = name;
        handleConnEvents(auth, name);
      }
      if(strcmp(type, "ping") == 0){                //Trata os ping's enviados pelo servidor e responde
        client.print("{\"type\":\"pong\"}");
      }
      if(strcmp(type, "auth") == 0){                //Trata mensagens do tipo 'auth'
        const char* auth = root["auth"];
        if(strcmp(auth, "ok") == 0){                    //Executado se a autenticação foi feita com sucesso
          const char* name = root["name"];
          int admin = root["admin"];
          openMenu(name, admin);
          waitingResponse = false;
          fullCircle = true;
        }
        else if(strcmp(auth, "fail") == 0){             //Executado se a autenticação não foi feita com sucesso
          Serial.println("AUTH FAIL");
          printMessage("USUARIO NAO", "AUTORIZADO");
          redColorAlert();
          waitingResponse = false;
          fullCircle = true;
        }
      }
      if(strcmp(type, "register") == 0){                //Trata mensagens do tipo 'register'
        int id = root["id"];
        registerFinger(id);
        waitingResponse = false;
        fullCircle = true;
        lcd.clear(); 
        lcd.setCursor(0,0); 
        lcd.print("SAIU. E AGORA?");


        if(AuthOk){

        if(waitingKeyboard){            //Função usada para o menu de administrador, ela recebe os comandos do teclado e executa uma função a

        char tecla = Keypad.Key_State();   //partir de uma tecla específica
        Serial.println(tecla);
        if (tecla == 3){
        tecla=leitura();
        Serial.print("tecla: ");
        Serial.println(tecla);
        if(tecla == (char)49){        //Se for pressionada a tecla 1, o relé é acionado.
            Serial.print("opendoor");
            Serial.print(tecla);
            openDoor();
            fullCircle = true;
        }
        if(tecla == (char)50){
          registerFinger(0);        //Se for pressionada a tecla 2, a função de registro de usuário é chamada
        }
        waitingKeyboard = false;    //No final de cada ciclo acima, a variável de flag do teclado é setada como falsa.
        }
        }
        if(fullCircle && !waitingKeyboard && !waitingID){
        lcd.clear(); // limpa o conteúdo no dysplay LCD
        lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
        lcd.print("BEM-VINDO(A) AO");
        lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
        lcd.print(welcomeName);
        fullCircle = false;
        }
        if(!waitingResponse && !waitingKeyboard && !waitingID){     //Estado padrão da tranca, sempre executado enquanto a tranca estiver ociosa
        int fingerID = readFinger();
        if(fingerID){
        sendFingerID(fingerID);
        waitingResponse = true;
        }
        }
        }




  







        
      }
      flag = false;
    }
}

void sendFingerID(int id){              //Função que envia o ID lido pelo sensor
  if(id){
    StaticJsonBuffer<64> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "fingerid";
    root["id"] = id;
    char buffer[64];
    root.printTo(buffer, sizeof(buffer));
    client.print(buffer);
  }
}

void openDoor(){
  lcd.clear(); // limpa o conteúdo no dysplay LCD
  lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
  lcd.print("BEM-VINDO(A)");
  lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
  lcd.print(userName);
  digitalWrite(rele, HIGH);
  delay(500);
  digitalWrite(rele, LOW);
  greenColorAlert();
}

void openMenu(String name, int admin){   //Menu do administrador
  userName = name;
  if(!admin)
    openDoor();
  else{
    printMessage("1- ENTRAR", "2- CADASTRAR");
    waitingKeyboard = true;
    blueColorAlert();
  }
}

void registerFinger(int id){            //Função que envia o pedido de ID ao servidor e chama a função de cadastro do sensor com o ID recebido
  if(!waitingID){
    client.write("{\"type\":\"addfinger\"}");
    waitingID = true;
  }
  else{
    waitingID = false;
    addFinger(id);
    lcd.clear(); 
    lcd.setCursor(0,0); 
   // lcd.print("SAIU. E AGORA?");
  }
}





char leitura(){
  char keypress = Keypad.Getkey();  // put value of key pressed in variable 'keypress'
  while ((Keypad.Key_State())){}  // Stay here while Key is held down
  Serial.println("TODAS AS TECLAS:");
  Serial.println(keypress);
  if (keypress != (char)49 && keypress != (char)50){
    leitura();
  }
  return keypress;
}
