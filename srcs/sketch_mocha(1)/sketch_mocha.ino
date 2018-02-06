/**

  THis is the Server Part Of The DBL.

*/


// Button
int buzzer = 3;


// Bluetooth
String re;
String se;

int iblue = 10;
int oblue = 11;
int bytese = 0;


// Activity
int todoList[10];
int valueList[10];
int todotop = 0;
int ia , ipa;

// Keyboard Functions.
String keys;



// Timer Functional
long timer = 0;


/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>

SoftwareSerial blue(iblue, oblue); // RX, TX

void setup()
{
  //Setup Button Click
  pinMode(buzzer,OUTPUT);  
  
  // Check The Keyboard / LED 
  
  // Check For LCD;
  
  
  // 操作1 等待Serial正常
  // Open serial communications and wait for port to open:
  
  Serial.begin(57600); 
  
  blue_init();
  
  // my
  // set the data rate for the SoftwareSerial port
  
}

boolean addActivity(int t){
  if(todotop >= 9){return false;}
  switch(t){
    case 1:break;
    case 2:break;
    case 3:// Ringing
    //    digitalWrite(oring,HIGH);
    todoList[todotop] = 3;
    valueList[todotop] = 1;
    todotop ++;
    return true;
    break;
  }
}

void afterActivity(int len){
  for(ia=0;ia<10;ia++){
    if(ia + 1 == len){      
      todoList[ia] = todoList[ia+2];
      valueList[ia] = valueList[ia+2];
      continue;
    }
    if(ia == len){
      continue;
    }
    todoList[ia] = todoList[ia+1];
    valueList[ia] = valueList[ia+1];
  }
  todotop--;
  delay(100);
}

int ipr;
void parseActivity(){
  if(todotop > 0){
      for(ipa = 0; ipa < todotop ; ipa ++ ){
      switch(todoList[ipa]){
        case 3:
          for(ipr=0;ipr<80;ipr++){        
             digitalWrite(buzzer,HIGH);//发声音       
             delay(1);//延时1ms        
             digitalWrite(buzzer,LOW);//不发声音       
             delay(1);//延时ms     
          }    
          delay(300);  
          for(ipr=0;ipr<100;ipr++)//输出另一个频率的声音     
          {        
            digitalWrite(buzzer,HIGH);//发声音       
            delay(2);//延时2ms        
            digitalWrite(buzzer,LOW);//不发声音       
            delay(2);//延时2ms     
          }   
          delay(300);
          for(ipr=0;ipr<100;ipr++)//输出另一个频率的声音     
          {        
            digitalWrite(buzzer,HIGH);//发声音       
            delay(2);//延时2ms        
            digitalWrite(buzzer,LOW);//不发声音       
            delay(1);//延时2ms     
          }   
          afterActivity(ipa);
          break;
        default:
          blue.print("Unknown Activity");
      }
    }
  }
}





void blue_init(){
  blue.begin(4800);
  //blue.println("Hello, world?");
  // 操作2改变蓝牙信息（d）xxxxxxxxxxx
  blue.println("ATTEST");
  while(blue.available() <= 0){
    delay(10);
  }
  while(blue.available() > 0){
    blue.read();
  }
  return;
}



int parse(){
  // Excute Data Recived
  if(se.substring(0,5) == "door"){
    // door
    while(addActivity(3) != true){
      delay(1);
      parseActivity();
    }
  }
  return -1;
}


boolean gather_blue(){
  if( blue.available() > 0){
    //Bluetooth Has Send An Information
    bytese = 0;
    while(blue.available() > 0){
      re = re + String(blue.read());
      bytese = bytese + 1;
      if(bytese >= 100){
        //Cotinuiusly Readed 100 Bytes, Force Stop.
        re = "";
        return false;
      }
    }
    if(re.substring(0,2) == "CL"){
      // From SERVER_SIDE
      se = re.substring(2);
      return true;
    }
  }
  return false;
}



void loop() // run over and over
{
  if (gather_blue()){
    // Gathered Data 
    parse();
  }
  
  parseActivity();
  // Clock update
  delay(10);
  timer ++;
}

