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
  finger.begin(57600);// Define a taxa de dados para a porta serial do sensor
}


void loop()                     
{
  String message=test();
  
  Serial.println(message);
  delay(3000);
}


String test() {

  uint8_t digital; 
  digital= finger.getImage(); //esta função captura a imagem do dedo (digital)
  return "ok";
  }


