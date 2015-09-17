#include <Adafruit_Fingerprint.h>
#if ARDUINO >= 100
 #include <SoftwareSerial.h>
#else
 #include <NewSoftSerial.h>
#endif

int getFingerprintIDez();

// pin #11 is IN from sensor (GREEN wire)
// pin #12 is OUT from arduino  (WHITE wire)
#if ARDUINO >= 100
SoftwareSerial mySerial(12, 11);//primeiro fiobranco depois verde
#else
NewSoftSerial mySerial(12, 11);
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);//Declaracao do objeto para comunicacao.


uint8_t getFingerprintEnroll(uint8_t id);

/*--------------------------End FingerPrint--------------------------*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,14,202); // IP Adress to our Server
const int serverPort=7000;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to
EthernetClient client;

/*-----------------------End Network--------------------------------*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

/*-----------------------End LCD------------------------------------*/
#include <Keypad.h>
const byte rows = 4;
const byte cols  = 3;
char teclas[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte linha_pino[] = {22, 24, 26, 28}; //Pinos do arduino que estao conectados as linhas do teclado
byte coluna_pino[] = {30, 32, 34};//Pinos do arduino que estao conectados as colunas do teclado
//Cria o objeto, teclado
Keypad teclado = Keypad( makeKeymap(teclas), linha_pino, coluna_pino, rows, cols);

/*----------------------End Teclado----------------------------------*/

#include <EEPROM.h>

/*----------------------End EEPROM-----------------------------------*/

#define redPin 42      //Pin 39
#define greenPin 40    //Pin 40
#define bluePin 38      //Pin 41

/*----------------------End LED RGB-----------------------------------*/
#define travaPin 53 //Pin 33

/*----------------------Thread---------------------------------------*/
#include <Thread.h>
#include <ThreadController.h>

ThreadController cpu;
Thread thServer;

void serverOn();
/*----------------------End Thread-----------------------------------*/
int const buzzer = 34;
void setup() {
  pinMode(redPin, OUTPUT); //sets redPin as output
  pinMode(greenPin, OUTPUT);//sets greenPin as output
  pinMode(bluePin, OUTPUT);//sets bluePin as output
  pinMode(travaPin, OUTPUT);//sets travaPin as output
  pinMode(buzzer, OUTPUT);
  lcd.begin(16, 2);
  finger.begin(57600);
  Serial.flush();
  // start the serial for debugging
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    lcd.clear();
    lcd.print("Erro [1]");
    lcd.setCursor(0, 1);
    lcd.print("Reincie");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(500);

  // if you get a connection to the Server
  if (!client.connect(serverIP, serverPort)) {
    lcd.clear();
    lcd.print("Aguardando");
    lcd.setCursor(0, 1);
    lcd.print("Servidor...");
  }
  thServer.setInterval(0);
  thServer.onRun(serverOn);

  cpu.add(&thServer);
}

int gerenciador_codigo = 0;
bool cadastro_usuario=true;
bool usuario_set = true;
String novo_usuario = "";
String string_user = "";

void loop(){
  cpu.run();
  //Para apagar os dados do arduino e do fingerprint, descomentar as linha abaixo e carregar o codigo, depois recomentar.
  //finger.emptyDatabase();
  //EEPROM.write(0, 2);
  if(usuario_set){
    lcd.clear();
    lcd.print("Usuario");
    lcd.setCursor(0, 1);
    lcd.print("GT-SET");
    usuario_set = false;
  }
  int finger_user;
  do{
    finger_user = getFingerprintIDez();
    if(!client.connected()){
      lcd.clear();
      lcd.print("Servidor fora");
      lcd.setCursor(0, 1);
      lcd.print("do ar");
      while(true)
        ;
    }
  }while(finger_user == -1);

  //Enquanto nada for detectado no sensor o codigo fica preso a parte superior.
  if (finger_user == -3){
    lcd.clear();
    lcd.print("Usuario nao");
    lcd.setCursor(0, 1);
    lcd.print("Encontrado");
    tone(buzzer, 1000, 100);
    corVermelha();
    tone(buzzer, 1000, 100);
    delay(250);
    usuario_set = true;
  }
  if(finger_user != -2 && finger_user != -3 ){
    float inicio = millis();
    float fim = 0;
    string_user += "0."; //Matricula e zero, logo nao sera feito nada no servidor
    string_user += finger_user;
    string_user += ".0"; //Avisa que nao para cadastro
    client.print(string_user); string_user = ""; //Zerar apos o envio
    while(!client.available()){
      fim = millis();
      int timeout = fim - inicio;
      if(timeout > 3000){ //timeout de 3s
        lcd.clear();
        lcd.print("Servidor nao");
        lcd.setCursor(0, 1);
        lcd.print("Responde");
        delay(1000);
        usuario_set = true;
        break;
      }
    }
    if(client.available()){
      char usuario = client.read();
      if(usuario == '1'){
        bool rodando_admin = true;
        bool opcoes_admin = true;
        bool exclusao_usuario = true;
        while(rodando_admin){
          corAmarela();
          if(opcoes_admin){
	    tone(buzzer, 1000, 100);
            lcd.clear();
            lcd.print("1 - Entrar     *");
            lcd.setCursor(0, 1);
            lcd.print("2 - Cadastrar");
            opcoes_admin=false;
          }
          char tecla = teclado.getKey();
            if (tecla != NO_KEY){
              opcoes_admin = true;
              rodando_admin = false;
              usuario_set = true;
              switch(tecla){
                case '1':
                  corVerde();
                  abrir();
                  lcd.clear();
                  lcd.print("Seja bem");
                  lcd.setCursor(0, 1);
                  lcd.print("Vindo");
                  tone(buzzer, 1000, 100);
		  delay(1000);
                  incolor();
                  break;
		case '2':
		  regUser();
                  incolor();
                  break;
                case '#':
                  corAmarela();
                  lcd.clear();
                  lcd.print("3 - Excluir    #");
                  while(exclusao_usuario){
                    char key_user1 = teclado.getKey();
                      if (key_user1 != NO_KEY){
                        if(isDigit(key_user1)){
                          if (key_user1 == '3'){
                            rmUser();
                            incolor();

                          }
                          else {
                            lcd.clear();
                            lcd.print("Opcao");
                            lcd.setCursor(0, 1);
                            lcd.print("Inexistente");
                            corVermelha();

                          }
                          exclusao_usuario = false;
                        }
                      }
                  }

                  delay(800);
                  break;
                default:
                  lcd.clear();
                  lcd.print("Opcao");
                  lcd.setCursor(0, 1);
                  lcd.print("Inexistente");
                  corVermelha();
              }
            }
        }
      }
      if(usuario == '0'){
        corVerde();
        abrir();
        lcd.clear();
        lcd.print("Seja");
        lcd.setCursor(0, 1);
        lcd.print("Bem vindo");
        tone(buzzer, 1000, 100);
        delay(1000);
        incolor();
        usuario_set = true;
      }
    }
  }
}

void serverOn(){
  int i=0;
}

//Função criada para remoção de usuário.

void rmUser(){
  uint8_t rmID = -1;
  while(true){
    lcd.clear();
    lcd.print("Coloque o dedo");
    rmID = getFingerprintIDez();
    if(rmID != -1 && rmID !=-2 && rmID !=-3 && rmID != 255 ){
      deleteFingerprint(rmID);
      string_user += "0."; //Matricula e zero, logo nao sera feito nada no servidor
      string_user += rmID;
      string_user += ".2"; //2 to remove. dict[1:sign up, 0:query]
      client.print(string_user); string_user = "";
      lcd.clear();
      lcd.print("Usuario");
      lcd.setCursor(0,1);
      lcd.print("Removido");
      delay(2000);
      break;
    }
  }
}

//Função criada para registro de usuário.

void regUser(){
  gerenciador_codigo = 0;
  cadastro_usuario = true;
  lcd.clear();
  lcd.print("Registro Usuario:");
  lcd.setCursor(0, 1);
  while(cadastro_usuario){
    char key_user = teclado.getKey();
    if (key_user != NO_KEY){
      if(isDigit(key_user)){
        novo_usuario += key_user;
        lcd.print(key_user);
        gerenciador_codigo++;
        }
    }
    if(gerenciador_codigo == 11){
      byte ultimo_usuario = EEPROM.read(0); //Le o byte guardado na EEPROM
      ultimo_usuario = ultimo_usuario + 1;
      novo_usuario += ".";
      novo_usuario += ultimo_usuario;
      novo_usuario += ".1"; //Determina que eh para ser gravado como usuario
      while(getFingerprintEnroll(ultimo_usuario));
      EEPROM.write(0, ultimo_usuario); //Escreve qual foi o ultimo usuario a se cadastrar na EEPROM(E2PROM)
      client.print(novo_usuario); novo_usuario = ""; //Zerar apos o envio
      cadastro_usuario = false; //Finaliza o Cadastro
    }
  }
}


//Início funções criadas para manipular cores do led RGB

void corAzul(){
  digitalWrite(bluePin,HIGH);
  digitalWrite(greenPin,LOW);
  digitalWrite(redPin,LOW);
}

void corVermelha(){
  short int i=0;
  for (i=0;i<5;i++){
    digitalWrite(bluePin,LOW);
    digitalWrite(greenPin,LOW);
    digitalWrite(redPin,HIGH);
    delay(100);
    digitalWrite(redPin,LOW);
    delay(100);
  }
}

void corVerde(){
  digitalWrite(bluePin,LOW);
  digitalWrite(greenPin,HIGH);
  digitalWrite(redPin,LOW);
}

void corAmarela(){
  digitalWrite(bluePin,LOW);
  digitalWrite(greenPin,HIGH);
  digitalWrite(redPin,HIGH);
}

void incolor(){
  digitalWrite(bluePin,LOW);
  digitalWrite(greenPin,LOW);
  digitalWrite(redPin,LOW);
}

//Funçao criada para destravar o fecho

void abrir(){
  digitalWrite(travaPin,HIGH);
  delay(100);
  digitalWrite(travaPin,LOW);
}


inline int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -2;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -3;
  return finger.fingerID;
}

uint8_t getFingerprintEnroll(uint8_t id) {
  uint8_t p = -1;
  lcd.clear();
  lcd.print("Coloque o dedo");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Gravando...");
      delay(500);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("...");
      delay(250);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.print("Retire o dedo");
  delay(250);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  lcd.clear();
  lcd.print("Coloque o dedo");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Gravando...");
      delay(2000);
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("...");
      delay(250);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }


  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("...");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Usuario");
    lcd.setCursor(0, 1);
    lcd.print("Cadastrado!");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

//funçao deletar

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deletado!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicação");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Não foi possível excluir nesse local");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Erro na leitura da flash");
    return p;
  } else {
    Serial.print("Erro desconhecido: 0x"); Serial.println(p, HEX);
    return p;
  }
}

