#include<SPI.h>


//configuracion 
    const int slavesQty = 1; //define el numero de esclavos a utilizar 
    
    int voltageMaxSet[slavesQty] = { //tope maximo de voltaje para el seteo de la resistencia
      19653
    };
    unsigned int voltageMinSet[slavesQty] = { //tope minimo para el seteo de la resistencia
      6551
    };
    unsigned int measureVoltage[slavesQty] = { //voltaje que se aplica para medir la resistencia 
      1310
    };
    unsigned int electricCurrents[slavesQty];
    
    unsigned int delta[slavesQty] = {
      1
    };
    int executeFlag[slavesQty] = { //una bandera por cada esclavo 
      0
    };
    
    
    
// variables del programa: 
    int stateMachine = 0; 
    
//Declaracion de PINES: 
    //Puerto del Arduino: 

    //Puertos SPI: 
    int _slaveSelect_[slavesQty] = {
      2
    };
    
    //PINES
    int _arduinoClk_ = 7; 
    
//METODOS : 
    //enviar por SPI
    void SPItransfer(int slaveNumber, unsigned int data, int state){
      //input: (esclavo que recibe la informacion, informacion)
      digitalWrite(slaveNumber,LOW); //activar esclavo
      
      delay(200);
      SPI.transfer((uint8_t)(state)); //enviar estatus de la maquina al esclavo
      
      delay(700);
      
      byte maquina = SPI.transfer((uint8_t)(data>>8));//transferencia de bits 0:7
      delay(700);
      
      byte ocho_bits = SPI.transfer((uint8_t)(data));//transferencia de bits 8:15
      delay(700);
      Serial.println(maquina, BIN);
      Serial.println(ocho_bits, BIN);
      digitalWrite(slaveNumber,HIGH); //desactivar esclavo
    
    }
    //Asignar variables del arreglo
    void dataChange (){
      for(int x; x<slavesQty; x++){
          SPItransfer(_slaveSelect_[x], delta[x], 1); 
          SPItransfer(_slaveSelect_[x], voltageMinSet[x], 2);
          SPItransfer(_slaveSelect_[x], voltageMaxSet[x], 3);
          SPItransfer(_slaveSelect_[x], measureVoltage[x], 4);
      }
    }
    
    void offsetValues(int offset) { // 32767
      for(int x; x<slavesQty; x++){
          voltageMinSet[x] += offset;
          voltageMaxSet[x] += offset;
          measureVoltage[x] += offset;
      }
    }

void setup(){
  //inicializar pines Arduino: 
  pinMode(_arduinoClk_,OUTPUT);
  
  //inicializar monitor serial: 
  Serial.begin(9600);
  
  //Inicializar protocolo SPI en cada esclavo:
  for (int x; x<slavesQty; x++) {
    pinMode(_slaveSelect_[x], OUTPUT);
    digitalWrite(_slaveSelect_[x], HIGH);
  } 

  //Inicializar la conexion serial en el arduino: 
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  delay(2000);
  
  offsetValues(32767);
  dataChange();
  
  Serial.println("ESTADOS ESCRITOS EN ATMEGA");
  
}



void loop() {
  //controlar los pasos de la maquina de estados:
  if(stateMachine < 5){ 
    stateMachine++;
  }else{stateMachine=1;}
  
  for (int x; x<slavesQty; x++){
    if (executeFlag[x] != 0){ //si detecta que se necesita cambiar los valores de un esclavo: 
      SPItransfer(x, delta[x]*(-1), 1); //cambiando el status de set a reset y de reset a set 
    }
  }
  
  if (stateMachine == 1){ 
    
  }
  
  
  
  digitalWrite(_arduinoClk_,HIGH);
  delayMicroseconds(30);
  digitalWrite(_arduinoClk_,LOW);
  delayMicroseconds(30);
  
}
