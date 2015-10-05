/*>>>>>>>>>>>>>>>>>>>>>>>Bibliotecas e pinos<<<<<<<<<<<<<<<<<<<*/
/*-----------------------Definindo ethernet--------------------*/
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,14,22); // IP Adress to our Server
const int serverPort=7000;
// Inicializa biblioteca ethernet cliente
EthernetClient client;
/*------------------Definindo Arduino Json e String-----------*/
#include <String.h>
#include <ArduinoJson.h>
String SETFINGER_HWID = "GT-SET";
String welcomeName;
bool AuthOk = false;
String lastName;
int lastID;
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>SETUP<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void setup(){
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

  readTCPStream();
  
  if (!client.connected()) {
      Serial.println("Disconnected!");
      serverConnect();
  }
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>funções<<<<<<<<<<<<<<<<<<<<<<<<<*/

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
        }
        else if(strcmp(auth, "fail") == 0){
          Serial.println("AUTH FAIL");
          printMessage("USUARIO NAO", "AUTORIZADO");
          redColorAlert();
        }
      }
      if(strcmp(type, "register") == 0){
        int id = root["id"];
        registerFinger(id);
      }
      flag = !flag;
    }
}
