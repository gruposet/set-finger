/*Este código contém a função readFinger() responsável por executar a leitura de digitais no sensor fingerprint e identificar se uma digital lida está cadastrada. 
 * A execução desta função ativa a leitura de digital no sensor, permanecendo ativa até que seja detectada a digital do usuário identificação.
 * Para executar a função addFinger() não é necessário repassar nenhum entrada, no entanto, se ocorrer algum erro na identificação da digital, a função retorna -1*/

#include <LiquidCrystal.h> // declara a utilização da biblioteca LiquidCrystal
#define Luz_Fundo  7
//cria um objeto tipo LiquidCrystal que chamei de "lcd" nos pinos citados:
LiquidCrystal lcd(3, 4, 5, 6, 7, 8); //(RS, E, D4, D5, D6, D7)


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

 int in, id, retorno;
 
void setup()  
{
  Serial.begin(9600);

  lcd.begin(16, 2); // Iniciando o objeto "lcd" de 2 linhas e 16 colunas
  pinMode(Luz_Fundo,OUTPUT); //define o pino como saída
  digitalWrite(Luz_Fundo,HIGH); // Liga a luz do display.
  
  finger.begin(57600);// Define a taxa de dados para a porta serial do sensor

}



void loop()                    
{  

  Serial.println("####################MENU####################");
  Serial.println("DIGITE A OPÇÃO DESEJADA!");
  Serial.println("1 - LER E IDENTIFICAR DIGITAL");
  Serial.println("2 - ADICIONAR DIGITAL");
  Serial.println("3 - EXCLUIR DIGITAL");

  lcd.setCursor(0,0); // seta o cursor para: (coluna = 0, linha = 0)
  lcd.print("DIGITE: 1-Ler");
  lcd.setCursor(0,1); // seta o cursor para: (coluna = 0, linha = 0)
  lcd.print("2-Add 3-Del");

  
  while (! Serial.available());
  in=Serial.parseInt();
  Serial.println(in);  
  

      
  if(in==1){
    Serial.println("READ OK!");
    retorno=readFinger(); 
    Serial.print("Retorno: ");
    Serial.println(retorno);
  }
  
  if(in==2) 
  {
  Serial.println("INFORME O ID QUE DESEJA ADICIONAR...");
    while (! Serial.available());
    id=Serial.parseInt();
    retorno=addFinger(id);
    Serial.print("Retorno: ");
    Serial.println(retorno);
  }

  if(in==3) 
  {
  Serial.println("INFORME O ID QUE DESEJA EXCLUIR...");
    while (! Serial.available());
    id=Serial.parseInt();  
    retorno=deleteFinger(id);
    Serial.print("Retorno: ");
    Serial.println(retorno);
  }
  
}





//LÊ E IDENTIFICA A DIGITAL------------------------------------------------------------------------------------------------------------------------------------------------------
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
  return -1;
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
    Serial.println("Digital nao encontrada. TENTE NOVAMENTE!"); 
    return -1; //retorna -1 e sai da função addFinger() somente se a digital for reconhecida entre as digitais presentes no sensor
    }
  
  Serial.print("ID Encontrado: "); Serial.println(finger.fingerID); 
  return finger.fingerID; 
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



//DELETA DIGITAIS ---------------------------------------------------------------------------------------------------------------------------------------------------------------
int deleteFinger(uint8_t id) {
  
  uint8_t digital;
  
  digital = finger.deleteModel(id);

  if (digital == FINGERPRINT_OK) {
    Serial.print("Digital ID:");
    Serial.print(id);
    Serial.println(" Excluida com Sucesso!");
  } else {
    Serial.print("ERRO!NÃO FOI POSSIVEL EXCLUIR A DIGITAL ID: ");
    Serial.println(id);
    return -1;
  } 
}
