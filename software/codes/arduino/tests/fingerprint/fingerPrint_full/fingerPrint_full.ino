/*Este código contém a função readFinger() responsável por executar a leitura de digitais no sensor fingerprint e identificar se uma digital lida está cadastrada. 
 * A execução desta função ativa a leitura de digital no sensor, permanecendo ativa até que seja detectada a digital do usuário identificação.
 * Para executar a função addFinger() não é necessário repassar nenhum entrada, no entanto, se ocorrer algum erro na identificação da digital, a função retorna -1*/

#include <Adafruit_Fingerprint.h>

//Decisão de biblioteca a ser utilizada de acordo com a versão da IDE
#if ARDUINO >= 100
 #include <SoftwareSerial.h>
 SoftwareSerial mySerial(11, 12);
#else
 #include <NewSoftSerial.h>
 NewSoftSerial mySerial(11, 12);
#endif


//Declaração do objeto para comunicação.
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

 uint8_t in;
 uint8_t id;
void setup()  
{
  Serial.begin(9600);
  finger.begin(57600);// Define a taxa de dados para a porta serial do sensor
}



void loop()                    
{  

  Serial.println("####################MENU####################");
  Serial.println("DIGITE A OPÇÃO DESEJADA!");
  Serial.println("1 - LER E IDENTIFICAR DIGITAL");
  Serial.println("2 - ADICIONAR DIGITAL");
  Serial.println("3 - EXCLUIR DIGITAL");
  
  while (! Serial.available());
  in=Serial.parseInt();
  Serial.println(in);  
  

      
  if(in==1){
    Serial.println("READ OK!");
    readFinger(); 
  }
  
  if(in==2) 
  {
  Serial.println("INFORME O ID QUE DESEJA ADICIONAR...");
    while (! Serial.available());
    id=Serial.parseInt();
    addFinger(id);
  }

  if(in==3) 
  {
  Serial.println("INFORME O ID QUE DESEJA EXCLUIR...");
    while (! Serial.available());
    id=Serial.parseInt();  
    deleteFinger(id);
  }
  
}


//LÊ E IDENTIFICA A DIGITAL -----------------------------------------------------------------------------------------------------------------------------------------------------
// retorna -1 se a digital não for reconhecida ou retorna o ID da digital se esta for reconhecida, ou seja, se esta estiver cadastrada no sensor
  //Esta função realiza a leitura da digital, retorna -1 se a digital não for reconhecida ou retorna o ID desta digital caso seja reconhecida.
  //Obs. a leitura da digital é executada muito rapidamente. Isso é possivel ser observado na oscilação do sinal indicador luminoso vermelho no sensor, portanto, 
  //levar em consideração o tempo de leitura de acordo com a aolicação.
  
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





//ADICIONAR DIGITAIS-------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t addFinger(uint8_t id) {
  
  //VERIFICA SE HÁ UM SENSOR FINGERPRINT CONECTADO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Procurando sensor...");
  if (finger.verifyPassword()) {
  Serial.println("Sensor FingerPrint Encontrado!");
  delay(1000);
  } 
  //se não houver nenhum sensor conectado imprime uma mensagem de erro e retorna a função setup
  else {
  Serial.println("Nenhum sensor foi encontrato. Conecte um sensor!");
  delay(1000);
  addFinger(0);
  }
  
  
  //"digital" é a variável que recebe um retorno a ser comparado com a variável "FINGERPRINT_OK"
  //Exemplo: Na excução de "finger.getImage()", função que ativa o sensor para a leitura de uma digital, se não for detectado o dedo, ou seja,
  //se o sensor não conseguir obter a imagem da digital o valor retornado para a variável "digital" será diferente do valor de "FINGERPRINT_OK"
  uint8_t digital; 
  
  
  //LEITURA DA PRIMEIRA DIGITAL/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Aguardando uma digital valida. Coloque seu dedo...");
  delay(1000);  
  
  
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
  if (digital != FINGERPRINT_OK)  return -1; //retorna -1 e sai da função addFinger() somente se a conversão da imagem da primeira digital não tiver ocorrido com sucesso

  
  Serial.println("Retire o dedo");
  delay(3000);


  //LEITURA DA SEGUNDA DIGITAL//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Aguardando uma digital valida. Coloque seu dedo novamente...");
  delay(1000);
    
  digital= finger.getImage();
  while(digital != FINGERPRINT_OK)
  {
     digital= finger.getImage();
  }

  //converte a imagem da segunda digital lida em arquivo caracter
  digital = finger.image2Tz(2);
  if (digital != FINGERPRINT_OK)  return -1; //retorna -1 e sai da função addFinger() somente se a conversão da imagem da segunda digital não tiver ocorrido com sucesso
 
  //criação do modelo para a digital lida
  digital = finger.createModel();
  if (digital != FINGERPRINT_OK){
    Serial.println("ERRO! DIGITAIS INCOMPATIVEIS");
    return -1; //retorna -1 e sai da função addFinger() somente se as digitais não forem compativeis e o modelo não tenha sido criado com sucesso
  }
  
  
  Serial.println("MODELO CRIADO COM SUCESSO. DIGITAIS COMPATIVEIS");
  delay(1000); 

  //gravação do modelo da digital
  digital = finger.storeModel(id);
  if (digital != FINGERPRINT_OK)  return -1; //retorna -1 e sai da função addFinger() somente se a gravação do modelo não tiver ocorrido com sucesso
  Serial.println("DIGITAL ADICIONADA COM SUCESSO");
  Serial.print("ID:");
  Serial.println(id);
  delay(1000);
 
  
}




//DELETA DIGITAIS ---------------------------------------------------------------------------------------------------------------------------------------------------------------
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
