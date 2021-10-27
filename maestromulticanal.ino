#include<SPI.h>


//configuracion 
    int voltageMaxSet = 19653; //tope maximo de voltaje para el seteo de la resistencia
    unsigned int voltageMinSet = 6551; //tope minimo para el seteo de la resistencia
    unsigned int measureVoltage  = 1310; //voltaje que se aplica para medir la resistencia 

    unsigned int vMax = 32767+voltageMaxSet;
    unsigned int vMin = 32767+voltageMinSet;
    unsigned int vMed = 32767+measureVoltage; 
    
    unsigned int delta = 1;
    
    
    
// variables del programa: 
    
    
    
//Declaracion de PINES: 
    //Puerto del Arduino: 

    //Puerto SPI 1: 
    int _slave1SS_ = 2;
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

void setup(){
  //inicializar pines Arduino: 
  pinMode(_arduinoClk_,OUTPUT);
  
  //inicializar monitor serial: 
  Serial.begin(9600);
  
  //Inicializar protocolo SPI: 
  pinMode(_slave1SS_, OUTPUT);
  SPI.begin();
  digitalWrite(_slave1SS_, HIGH);           
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  delay(2000);
  SPItransfer(_slave1SS_ , delta, 1); 
  SPItransfer(_slave1SS_, vMin, 2);
  SPItransfer(_slave1SS_, vMax, 3);
  SPItransfer(_slave1SS_, vMed, 4);
  Serial.println("ESTADOS ESCRITOS EN ATMEGA");
  
}

void loop() {
  
  digitalWrite(_arduinoClk_,HIGH);
  delayMicroseconds(22);
  digitalWrite(_arduinoClk_,LOW);
  delayMicroseconds(22);
  
}
