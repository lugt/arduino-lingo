/**

  THis is the Client Part Of The DBL.

*/


// Button
int ibtn = 3;


// Bluetooth
String re;
String se;

int iblue = 10;
int oblue = 11;
int bytese = 0;


// Keyboard Functions.
int key1,key2,key3,key4;
boolean with_key = false;
String key_inputed;
String kpr;



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
  pinMode(ibtn,INPUT);  
  
  // Check The Keyboard / LED 
  
  // Check For LCD;
  
  
  // 操作1 等待Serial正常
  // Open serial communications and wait for port to open:
  
  Serial.begin(57600); 
  
  blue_init();
  
  // my
  // set the data rate for the SoftwareSerial port
  
}

void blue_init(){
  blue.begin(4800);
  //blue.println("Hello, world?");
  // 操作2改变蓝牙信息（d）xxxxxxxxxxx
  blue.println("ATTEST");
  while(blue.available() <= 0){
    delay(10);
  }
  return;
}



int parse(){
  // Excute Data Recived
  if(se.substring(0,5) == "text"){
    return 0;
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
    if(re.substring(0,2) == "SV"){
      // From SERVER_SIDE
      se = re.substring(2);
      return true;
    }
  }
  return false;
}

boolean gather_btn(){
  if(digitalRead(ibtn) == HIGH){
    return true;
  }else{
    return false;
  }
}



long keystart = 0;
boolean gather_keys(){
  if(with_key == true){
   if(timer - keystart >= 30){
    // 3 Seconds
    // Sned It
      return true;
    }
    // anjian
  }else{
    // Get Keys; 
    // with_key = true;
    // keystart = timer;
    return false;
  }
}


void loop() // run over and over
{
  if (gather_blue()){
    // Gathered Data 
    parse();
  }
  
  if(gather_btn() == true){
    // Send Bluetooth
    blue.print("CLdoor");
  }  
  
  if(gather_keys() == true){
    // Send Bluetooth
    blue.print(String("CLtext")+(key_inputed));
  }  
  
  
  // Clock update
  delay(200);
  timer ++;
}

