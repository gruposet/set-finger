/*>>>>>>>>>>>>>>>>>>>>>>>Bibliotecas e pinos<<<<<<<<<<<<<<<<<<<*/
/*-----------------------Definindo ethernet--------------------*/
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,14,22); // IP Adress to our Server
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
/*---Definindo biblioteca ArduinoThread e threads de KeepAlive e Fingerprint----*/



/*-------------Definindo os pinos do LED RGB-------------------*/
#define red 26    //define o pino do LED como vermelho
#define green 24   // define o pino do LED como verde
#define blue 22   //define o pino do LED como azul    
/*-------------Definindo o pino do buzzer e relé---------------*/
int const buzzer = 30;
int const rele = 28;
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
/*--------------------------------------------------*/
#include <String.h>
#include <ArduinoJson.h>
String SETFINGER_HWID = "GT-SET";
String welcomeName;
bool AuthOk = false;
bool waitingResponse = false;
bool fullCircle = true;
bool waitingKeyboard = false;
bool waitingID = false;
String lastName;
int lastID;

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>SETUP<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void setup(){
  Serial.begin(115200); 
  finger.begin(57600);// Define a taxa de dados para a porta serial do sensor

  lcd.begin(16, 2); //objeto "lcd" de 2 linhas e 16 colunas
  pinMode(luz_fundo,OUTPUT); //define o pino como saída
  digitalWrite(luz_fundo,HIGH); // Liga a luz do display.

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


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>LOOP<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void loop(){
  if(AuthOk){

    if(waitingKeyboard){
      char key = keypad.getKey();
      if (key != NO_KEY){
        if(key == (char)49){
            openDoor();
            fullCircle = true;
        }
        if(key == (char)50){
          registerFinger(0);
        }
        waitingKeyboard = false;
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
    if(!waitingResponse && !waitingKeyboard && !waitingID){
      int fingerID = readFinger();
      if(fingerID){
        Serial.println(fingerID);
        sendFingerID(fingerID);
        waitingResponse = true;
      }
    }
  }

  readTCPStream();
  
  if (!client.connected()) {
      Serial.println("Disconnected!");
      serverConnect();
  }
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>funções<<<<<<<<<<<<<<<<<<<<<<<<<*/
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
  Serial.println("Aguardando uma digital valida. Coloque seu dedo...");
  delay(1000);  
  
  digital= finger.getImage(); 
  while(digital != FINGERPRINT_OK) {
    digital= finger.getImage();
  }
  digital = finger.image2Tz(1);
  if(digital != FINGERPRINT_OK)  
    return -1; //retorna -1 e sai da função addFinger() somente se a conversão da imagem da primeira digital não tiver ocorrido com sucesso
   
  Serial.println("Retire o dedo");
  delay(3000);

  Serial.println("Aguardando uma digital valida. Coloque seu dedo novamente...");
  delay(1000);
    
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
    Serial.println("ERRO! DIGITAIS INCOMPATIVEIS");
    return -1; //retorna -1 e sai da função addFinger() somente se as digitais não forem compativeis e o modelo não tenha sido criado com sucesso
  }
    
  Serial.println("MODELO CRIADO COM SUCESSO. DIGITAIS COMPATIVEIS");
  delay(1000); 

  //gravação do modelo da digital
  digital = finger.storeModel(id);
  if(digital != FINGERPRINT_OK)  
    return -1; //retorna -1 e sai da função addFinger() somente se a gravação do modelo não tiver ocorrido com sucesso
  Serial.println("DIGITAL ADICIONADA COM SUCESSO");
  delay(1000);
 
}

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

void printMessage(String row1, String row2) {
      lcd.clear(); // limpa o conteúdo no dysplay LCD
      lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
      lcd.print(row1);
      lcd.setCursor(0,1); // seta para linha 1, ou seja, a linha de baixo
      lcd.print(row2);
}

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

void greenColorAlert() { //significa usuário autorizado ou tudo ok!
  short int i=0;
  for (i=0;i<2;i++){
      digitalWrite(blue,LOW);
      digitalWrite(green,HIGH);
      digitalWrite(red,LOW);
      tone(buzzer, 750, 500);
      delay(500);
      digitalWrite(green,LOW);
      delay(50);
    }
}

void blueColorAlert() { //significa usuário admin autorizado ou conexão com o servidor.
  short int i=0;
  for (i=0;i<2;i++) {
     digitalWrite(blue,HIGH);
     digitalWrite(green,LOW);
     digitalWrite(red,LOW);
     tone(buzzer, 500, 300);
     delay(25);
     digitalWrite(blue,LOW);
     delay(250);
  }
}

void handleConnEvents(const char* msg, String name){ //trata a mensagem de conexão recebida do servidor 
    if(strcmp(msg, "fail") == 0){
          printMessage("ERRO", "ID: #0002");
          redColorAlert();
          Serial.println("Auth fail! Reconnecting...");
          delay(5000);
          AuthOk = false;
          client.stop();
        }
        else if(strcmp(msg, "ok") == 0){
          Serial.println("Auth OK! Ready to use...");
          printMessage("BEM-VINDO(A) AO", name);
          AuthOk = true;
        }
}

void serverConnect() {  //estabelece uma conexão TCP com o servidor e envia a mensagem de "boas-vindas"
  client.stop();
  AuthOk = false;
  Serial.println("Connecting...");
  printMessage("CONECTANDO...","");
  if(client.connect(serverIP, serverPort)) {
    Serial.println("Connected!");
    fullCircle = false;
    waitingResponse = false;
    waitingKeyboard = false;
    StaticJsonBuffer<64> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "conn";
    root["hwid"] = SETFINGER_HWID;
    char buffer[64];
    root.printTo(buffer, sizeof(buffer));
    client.print(buffer);
  } else {
    printMessage("ERRO", "ID: #0001");
    Serial.println("Connection failed!");
    redColorAlert();
  }
}

void readTCPStream(){ //lê o JSON enviado pelo servidor e armazena num array de char para ser tratado pela lib
    char stream[128]; //também trata as mensagens recebidas direcionando para cada função específica
    bool flag = false;
    int j = 0;
    if(client.available()){
      for(int i=0; i<128; i++)
        stream[i]=(char)0;
    }
    while(client.available() && !flag) {
        stream[j] = client.read();
        char lastChar = stream[j];
        j++;
        if(client.available()==0 || lastChar == 125)
          flag = !flag;
    }
    if(flag){
      Serial.println(stream);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(stream);
      const char* type = root["type"];
      if(strcmp(type, "conn") == 0){
        const char* auth = root["auth"];
        const char* name = root["name"];
        welcomeName = name;
        handleConnEvents(auth, name);
      }
      if(strcmp(type, "ping") == 0){
        client.print("{\"type\":\"pong\"}"); 
      }
      if(strcmp(type, "auth") == 0){
        const char* auth = root["auth"];
        if(strcmp(auth, "ok") == 0){
          const char* name = root["name"];
          int admin = root["admin"];
          openMenu(name, admin);
          waitingResponse = false;
          fullCircle = true;
        }
        else if(strcmp(auth, "fail") == 0){
          Serial.println("AUTH FAIL");
          printMessage("USUARIO NAO", "AUTORIZADO");
          redColorAlert();
          waitingResponse = false;
          fullCircle = true;
        }
      }
      if(strcmp(type, "register") == 0){
        int id = root["id"];
        registerFinger(id);
      }
      flag = !flag;
    }
}

void sendFingerID(int id){
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
  lcd.print(lastName);
  digitalWrite(rele, HIGH);
  delay(250);
  digitalWrite(rele, LOW);
  greenColorAlert();  
}

void openMenu(String name, int admin){
  lastName = name;
  if(!admin)
    openDoor();
  else{
    printMessage("1- ENTRAR", "2- CADASTRAR");
    waitingKeyboard = true;
    blueColorAlert();
  }
}

void registerFinger(int id){
  if(!waitingID){
    client.write("{\"type\":\"addfinger\"}");
    waitingID = true;
  }
  else{
    waitingID = false;
    addFinger(id);
    
  } 
  fullCircle = true; 
}


