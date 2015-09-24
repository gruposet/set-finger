/*Este código contém a função readFinger() responsável por executar a leitura de digitais no sensor fingerprint e identificar se uma digital lida está cadastrada. 
 * A execução desta função ativa a leitura de digital no sensor, permanecendo ativa até que seja detectada a digital do usuário.
 * Para executar a função addFinger() não é necessário repassar nenhum entrada, no entanto, se ocorrer algum erro na identificação da digital, a função retorna -1*/

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
  
  //Esta função realiza a leitura da digital, retorna -1 se a digital não for reconhecida ou retorna o ID desta digital caso seja reconhecida.
  //Obs. a leitura da digital é executada muito rapidamente. Isso é possivel ser observado na oscilação do sinal indicador luminoso vermelho no sensor, portanto, 
  //levar em consideração o tempo de leitura de acordo com a aolicação.
  readFinger(); 
  
}


//LÊ E IDENTIFICA A DIGITAL//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// retorna -1 se a digital não for reconhecida ou retorna o ID da digital se esta for reconhecida, ou seja, se esta estiver cadastrada no sensor
int readFinger() {

  //VERIFICA SE HÁ UM SENSOR FINGERPRINT CONECTADO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Procurando sensor...");
  if (finger.verifyPassword()) {
  Serial.println("Sensor FingerPrint Encontrado!");
  } 
  //se não houver nenhum sensor conectado imprime uma mensagem de erro e retorna a função setup
  else {
  Serial.println("Nenhum sensor foi encontrato. Conecte um sensor!");
  readFinger();
  }


  //"digital" é a variável que recebe um retorno a ser comparado com a variável "FINGERPRINT_OK"
  //Exemplo: Na excução de "finger.getImage()", função que ativa o sensor para a leitura de uma digital, se não for detectado o dedo, ou seja,
  //se o sensor não conseguir obter a imagem da digital o valor retornado para a variável "digital" será diferente do valor de "FINGERPRINT_OK"
  uint8_t digital; 


  //LEITURA DA DIGITAL///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Aguardando uma digital valida. Coloque seu dedo...");
   
   
  //"finger.getImage()" faz a leitura da digital(gera a imagem) e retorna um valor para a variável "digital"
  //obs. é necessário executar esta função antes do laço while para que seja atribuido o valor de resposta a variável "digital" para que o laço funcione corretamente
  //este laço tem como função executar a leitura da digital até que o usuário insira o dedo, ou seja, enquanto não for detectada uma digital no sensor, este permanecerá 
  //em modo de leitura aguardando-a
  digital= finger.getImage();
  while(digital != FINGERPRINT_OK)
  {
    digital= finger.getImage();
  }
  
  //converte a imagem lida em arquivo caracter
  digital = finger.image2Tz();
  if (digital != FINGERPRINT_OK)  return -1; //retorna -1 e sai da função addFinger() somente se a conversão da imagem da digital não tiver ocorrido com sucesso

  //verifica se a digital lida está entre as digitais cadastradas no sensor
  digital = finger.fingerFastSearch();
  if (digital != FINGERPRINT_OK){  
    Serial.println("DIGITAL NAO ENCONTRADA. TENTE NOVAMENTE!"); 
    return -1; //retorna -1 e sai da função addFinger() somente se a digital for reconhecida entre as digitais presentes no sensor
    }
  
  Serial.print("ID Encontrado: "); Serial.println(finger.fingerID); 
  return finger.fingerID; 
}
