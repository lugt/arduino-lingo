/**

  THis is the Client Part Of The DBL.

*/


// Button
int ibtn = A3;


// Bluetooth
String re;
String se;

int iblue = 10;// toTX
int oblue = 11;// toRX
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
  Serial.begin(9600);
  blue_init();
  
  // my
  // set the data rate for the SoftwareSerial port
  
}

void blue_init(){
  blue.begin(9600);
  //blue.println("Hello, world?");
  // 操作2改变蓝牙信息（d）xxxxxxxxxxx
  blue.print("AT");
  delay(100);
  blue.print("AT+BAUD2");
  delay(1000);
  blue.print("AT+NAMELatte");
  delay(1000);
  blue.print("AT+PIN8274");
  while(blue.available() > 0){
    blue.read();
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






#include <Keypad.h>;

const byte ROWS = 4; //四行
const byte COLS = 4; //四列
//定义键盘上的按键标识
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9,8,7,6}; //连接到行扫描的输入输出端口
byte colPins[COLS] = {5,4,3,2}; //连接到列扫描的输入输出端口

//定义Keypad类的实例
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


boolean gather_btn(){
  if(digitalRead(ibtn) == HIGH){
    Serial.println(" IBTN is HIGH");
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
    delay(2000);
    while(gather_btn() == true){
      Serial.println("Btn is Still On");
       delay(200);
    }
  }  
  
  char customKey = customKeypad.getKey();

  if (customKey){
    if(String(customKey) == "#"){
      // Send Text
      Serial.print(String("CLtext") + key_inputed);
      blue.print(String("CLtext") + key_inputed);
      delay(1000);
      key_inputed = String("");
    }else{
      if(key_inputed.length < 100){
        key_inputed += String(customKey);
        Serial.println(customKey);
      }
    }
  }
  
  // Clock update
  delay(30);
  timer ++;
}

