/*Este código executa a função deleteFinger(). Esta função é responsável por deletar digitais gravadas no sensor fingerprint.
Para executar esta função é necessário informar o ID a ser excluído*/


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


void setup()  
{
  Serial.begin(9600);
  
  // Define a taxa de dados para a porta serial do sensor
  finger.begin(57600);
}


void loop()                    
{  
  
deleteFinger(0);//INFORME O ID DESEJADO A SER EXCLUÍDO
  
}


//DELETA DIGITAIS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
