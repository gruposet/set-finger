/*Este código contém a função addFinger(id), responsável pela adição de digitais no sensor fingerprint. 
 * A execução desta função ativa a leitura de digital no sensor, permanecendo ativa até que seja detectada a digital do usuário para cadastro. 
 * Este processo se repete duas vezes para que a digital seja lida e comparada. Portanto, é necessário que o usuário insira o mesmo dedo duas vezes.
 * Para executar esta função é necessário informar o ID a ser cadastrado no sensor. A função retorna -1 apenas se ocorrer erro em algumas das etapas de gravação da digital.
 */
 
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
  int retorno=addFinger(2);//INFORME O ID DESEJADO A SER CADASTRADO
  Serial.print("retorno: ");
  Serial.println(retorno);
  delay(3000);
}

//ADICIONAR DIGITAIS-------------------------------------------------------------------------------------------------------------------------------------------------------------
int addFinger(uint8_t id) {
  
  //VERIFICA SE HÁ UM SENSOR FINGERPRINT CONECTADO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Procurando sensor... ");
  if (finger.verifyPassword()) {
  Serial.println("Sensor FingerPrint Encontrado!");
  
  } 
  //se não houver nenhum sensor conectado imprime uma mensagem de erro e retorna a função setup
  else {
  Serial.println("Nenhum sensor foi encontrato. Conecte um sensor!");
  return -1;//return "NENHUM SENSOR FOI ENCONTRADO. CONECTE UM SENSOR E TENTE NOVAMENTE!";
  }
  
  
  //"digital" é a variável que recebe um retorno a ser comparado com a variável "FINGERPRINT_OK"
  //Exemplo: Na excução de "finger.getImage()", função que ativa o sensor para a leitura de uma digital, se não for detectado o dedo, ou seja,
  //se o sensor não conseguir obter a imagem da digital o valor retornado para a variável "digital" será diferente do valor de "FINGERPRINT_OK"
  uint8_t digital; 
  
  
  //LEITURA DA PRIMEIRA DIGITAL/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Aguardando uma digital valida. Coloque seu dedo...");
    
  
  //"finger.getImage()" faz a leitura do dedo (gera a imagem) e retorna um valor para a variável "digital"
  //obs. é necessário executar esta função antes do laço while para que seja atribuido o valor de resposta a variável "digital" para que o laço funcione corretamente
  //este laço tem como função executar a leitura da digital até que o usuário insira o dedo, ou seja, enquanto não for detectada uma digital no sensor, este permanecerá 
  //em modo de leitura aguardando-a
  
  digital= finger.getImage(); 
  while(digital != FINGERPRINT_OK)
  {
    digital= finger.getImage();
  }

  //converte a imagem da primeira digital lida em arquivo caracter
  digital = finger.image2Tz(1);
  if (digital != FINGERPRINT_OK)  return -1;//{return "ERRO! NÃO FOI POSSIVEL CONVERTER A IMAGEM";} //retorna -1 e sai da função addFinger() somente se a conversão da imagem da primeira digital não tiver ocorrido com sucesso

  Serial.println("Retire o dedo");
  delay(1000);


  //LEITURA DA SEGUNDA DIGITAL//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Aguardando uma digital valida. Coloque seu dedo novamente...");
  //delay(1000);
    
  digital= finger.getImage();
  while(digital != FINGERPRINT_OK)
  {
     digital= finger.getImage();
  }

  //converte a imagem da segunda digital lida em arquivo caracter
  digital = finger.image2Tz(2);
  if (digital != FINGERPRINT_OK)  return -1;//{return "ERRO! NÃO FOI POSSIVEL CONVERTER A IMAGEM";} //retorna -1 e sai da função addFinger() somente se a conversão da imagem da segunda digital não tiver ocorrido com sucesso
 
  //criação do modelo para a digital lida
  digital = finger.createModel();
  if (digital != FINGERPRINT_OK)  return -1;//{return "ERRO! NÃO FOI POSSIVEL CRIAR O MODELO. DIGITAIS IMCOMPATIVEIS";} //retorna -1 e sai da função addFinger() somente se as digitais não forem compativeis e o modelo não tenha sido criado com sucesso
   
  
  //Serial.println("Modelo criado com sucesso. Digitais compativeis");
  //delay(1000); 

  //gravação do modelo da digital
  digital = finger.storeModel(id);
  if (digital != FINGERPRINT_OK)  return -1;//{return "ERRO! NÃO FOI POSSIVEL GRAVAR O MODELO";} //retorna -1 e sai da função addFinger() somente se a gravação do modelo não tiver ocorrido com sucesso
  Serial.println("Digital adicionada com sucesso");
  Serial.print("ID: ");
  Serial.println(id);
  return id;
   
  
}
