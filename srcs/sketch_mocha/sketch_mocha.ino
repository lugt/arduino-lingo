/**
  Bluetooth Module (Transfer Data)
  V 0.0.1
  */
//#include <SoftwareSerial.h>
int bluetooth_status = 0;

//Pre-States & StopReason_Args

//States

//Force_action

//Stopper

String sender;


//Sensors

//Pins
int pin_buzzer = 22;

int bluekey = 30;
int blue_connected = 0;

String clientkey = "2014,10,102506";

//BlueTooth Soft Serial
//SoftwareSerial mySerial(toTX,toRX); // RX, TX

//Structure
//int counter = 1; //Next:Bluetooth
int state = 99; // Initial
int prestate = 0;//NULL

// Bluetooth
String comdata;
String args;
String command;

String vdata;
String get_blue_String() {
//  Serial.println("Getting String From Blue");
  comdata = "";
  while (Serial3.available() > 0) {
     //Serial.println("Serial2 Is Reading");
     vdata = String(char(Serial3.read()));
    if (String("\r") == vdata || String("\n") == vdata || String("\r\n") == vdata) {
        // Nothing;
       // Serial.println("Ignored");
    } else {
      comdata += vdata;
      delay(2);
    }
  }
  delay(10);
  Serial.println(String("--")+comdata+String("--"));
  return comdata;
}





/**

      Led Modules

*/

void doled(int i) {
  digitalWrite(13, i);
}



/**
  
    Keypad Module;
    


*/


/**

    Bluetooth Module

*/


void blue_clear(){
  while (Serial3.available() > 0) {
      Serial3.read();
  }
}
void bluep(String s) {
  blue_clear();
  Serial3.print(s + String("\r\n"));
}


String get_pc_String() {
    comdata = "";
    while (Serial.available() > 0) {
      comdata += String(char(Serial.read()));
    }
    return comdata;
}


String temp;
void connect_blue() {
  // Connect Blue;
  // If OK
  bluep("AT"); // Search In Pair List
  temp = get_blue_String();
  Serial.println(String("Connecting Bluetooth / Check : ") + temp + String(";"));
  
  bluep("AT+STATE"); // Search In Pair List
  delay(200);
  temp = get_blue_String();
  temp.toLowerCase();
  if (temp.substring(0,16) != "+state:connected" || digitalRead(50) == HIGH) {
    blue_connected = 0;
    doled(0);
  }
  
  
  while (blue_connected == 0) {
    //    11a2 34 01b35f
    
    bluep("AT+STATE"); // Search In Pair List
    delay(200);
    temp = get_blue_String();
    temp.toLowerCase();
    if (temp.substring(0,16) == "+state:connected" || digitalRead(50) == HIGH) {
      blue_connected = 1;
      doled(0);
      return;
    }
    
    blue_clear();
    bluep(String("AT+FSAD=") + clientkey); // Search In Pair List
    delay(500);
    temp = get_blue_String();
    if (temp.substring(0,2) == "OK") {
      
      /*
      bluep(String("AT+RNAME?") + clientkey);
      delay(3000);
      temp = get_blue_String();
      Serial.println(String("RNAME Answer:") + temp);
      delay(300);
      temp.toLowerCase();
      if(temp == "+rname:latteok"){
        doled(0);
        blue_connected = 1;
        return;
      }
      */
      
      
      // IN the Pair List
      bluep(String("AT+LINK=") + clientkey);
      delay(5000);
      temp = get_blue_String();
      Serial.println(String("LINKING Answer:" ) + temp);
      
      if (temp.substring(0,2) == String("OK")) {
        // LINKED
        doled(0);
        blue_connected = 1;
        return;
      }
    } else {
      get_blue_String();
      bluep(String("AT+PAIR=") + clientkey + String(",5"));
      doled(0);
      delay(5*1100); // Wait For 5 Seconds
      doled(1);
      temp = get_blue_String();
      Serial.print("Pair Answer : ");
      Serial.println(temp);
      // OK
      // DOLED ?
    }
  }
}

// change this to the number of steps on your motor
int LeftI1 = 23;     //连接电机驱动板的I1接口  
int LeftI2 = 24;     //连接电机驱动板的I2接口  
int LeftEA = 27;      //连接电机驱动板的EA接口  
int RightI1 = 25;    //连接电机驱动板的I1接口  
int RightI2 = 26;    //连接电机驱动板的I2接口  
int RightEB = 28;     //连接电机驱动板的EB接口  
int StepCount = 0;  
int StepDelayTime=1500;  

void ForwardInit()  
{  
  digitalWrite(LeftEA, HIGH);  
  digitalWrite(RightEB,HIGH );  
  digitalWrite(LeftI1, LOW);  
  digitalWrite(LeftI2,HIGH );  
  digitalWrite(RightI1,HIGH);  
  digitalWrite(RightI2, HIGH);  
  StepCount=0;  
}  
void BackwardInit()  
{  
  digitalWrite(LeftEA, HIGH);  
  digitalWrite(RightEB,HIGH );  
  digitalWrite(LeftI1, LOW);  
  digitalWrite(LeftI2,LOW );  
  digitalWrite(RightI1,LOW);  
  digitalWrite(RightI2, HIGH);  
  StepCount=0;  
}  
void ForwardOneStep()  
{  
  delayMicroseconds(StepDelayTime);  
  switch(StepCount)  
  {  
    case 0:  
      digitalWrite(RightI2,LOW);  
      digitalWrite(LeftI1,HIGH);  
      break;  
    case 1:  
      digitalWrite(RightI1,LOW);  
      digitalWrite(RightI2,HIGH);  
      break;  
    case 2:  
      digitalWrite(LeftI2,LOW);  
      digitalWrite(RightI1,HIGH);  
      break;  
    case 3:  
      digitalWrite(LeftI1,LOW);  
      digitalWrite(LeftI2,HIGH);  
      break;  
  }  
  StepCount=(StepCount + 1) % 4;  
}  
  
void BackwardOneStep()  
{  
  delayMicroseconds(StepDelayTime);  
  switch(StepCount)  
  {  
    case 0:  
      digitalWrite(RightI2,LOW);  
      digitalWrite(LeftI1,HIGH);  
      break;  
    case 1:  
      digitalWrite(LeftI1,LOW);  
      digitalWrite(LeftI2,HIGH);  
      break;  
    case 2:  
      digitalWrite(LeftI2,LOW);  
      digitalWrite(RightI1,HIGH);  
      break;  
    case 3:  
      digitalWrite(RightI1,LOW);  
      digitalWrite(RightI2,HIGH);  
      break;  
  }  
  StepCount=(StepCount + 1) % 4;  
}  
  
void looprun()  
{  
    ForwardInit();  
    for(int i=0;i<300;i++)  
    {  
      ForwardOneStep();  
    }  
    delay(7000);  
    BackwardInit();  
    for(int i=0;i<300;i++)  
    {  
      BackwardOneStep();  
    }  
    delay(300);
    digitalWrite(LeftEA, LOW);  
    digitalWrite(RightEB,LOW); 
}  


int init_bluetooth() {
  
  // Off Light  
  doled(1);
  int o = 0;
  String temp;

  // Connected Bluetooth;
  while (o == 0) {
    Serial3.print("AT\r\n");
    temp = get_blue_String(); // Ignored
    if (temp == "OK") {
      o = 1;
    }
    delay(1000);
  }  
  
  // Blink
  
  doled(0);
  delay(3000);
  doled(1);

  // Initialize
  blue_clear(); //CLear All Functions
  Serial.println("[OK] AT Is INITing");
  
  
  bluep("AT+INIT");
  temp = get_blue_String();
  Serial.print("[OK] AT Is INITED");
  Serial.println(temp);
  
  
  // AT+BAUD4
  bluep("AT+UART=9600,0,0");
  delay(300);
  temp = get_blue_String();  
  Serial.print("[OK] AT Is UART 9600ed");
  Serial.println(temp);


  bluep("AT+ROLE=1");
  delay(300);
  temp = get_blue_String();
  Serial.print("[OK] AT Is UART Roled");
  Serial.println(temp);
  
  
  bluep("AT+NAME=MOCHA");
  delay(300);
  temp = get_blue_String();
  Serial.print("[OK] AT Is NAMED");
  Serial.println(temp);
  
  
  bluep("AT+BIND=11a2,34,05a5ce");
  delay(300);
  temp = get_blue_String();
  Serial.print("[OK] AT Is BIND");
  Serial.println(temp);
  // CLIENT 11a2 34 01b35f
  
  // "AT+PIN8274" = > "AT+PSWD=8274"
  bluep("AT+PSWD=8274");
  delay(300);  
  temp = get_blue_String();
  Serial.print("[OK] AT Is PSWDed");
  Serial.println(temp);
  
  
  Serial.println("All Settings Gone");

  // LED Blink
  int i;
  for(i=0;i<7;i++){
  doled(1);
  delay(300);
  doled(0);
  delay(100);
  }
  connect_blue();
}

int get_bluetooth_message() {
  comdata = "";
  comdata = get_blue_String();
  Serial.print("Messager :");
  Serial.println(comdata);
  analyze(comdata);
}


long timer;
long last;

int get_bluetooth() {
  Serial.println("BLuetooth Fetch");
  if (blue_connected == 0 || digitalRead(50) == LOW) {
    Serial.println("BL Not Connected");
    init_bluetooth();
    last = timer;
  }else{
    if(timer - last >= 20 * 60){
      connect_blue();  
      last = timer;
    }
  }
  Serial.println("Check Over");
  
  /**
  1.Get Connection Status
  2.Check Password.(Login) Not necessary
  3.Send Keep-Alive-Pack
  4.Get Response;
  5.Analyze Resonpse;
  */
  if (Serial3.available() > 0) {
    Serial.println("Blue Available");
    get_bluetooth_message();
    //return 1;
  }
  
  if (Serial.available() > 0){
      comdata = get_pc_String();
      Serial3.print(comdata);
      analyze(comdata);
      return 1;
  }
  return -1;
}

void door_buzzer() {
  int i = 0;
  for (i = 0; i < 80; i++) {
    digitalWrite(pin_buzzer, HIGH); //发声音
    delay(1);//延时1ms
    digitalWrite(pin_buzzer, LOW); //不发声音
    delay(1);//延时ms
  }
  delay(300);
  for (i = 0; i < 100; i++) //输出另一个频率的声音
  {
    digitalWrite(pin_buzzer, HIGH); //发声音
    delay(2);//延时2ms
    digitalWrite(pin_buzzer, LOW); //不发声音
    delay(2);//延时2ms
  }
  delay(300);
  for (i = 0; i < 100; i++) //输出另一个频率的声音
  {
    digitalWrite(pin_buzzer, HIGH); //发声音
    delay(2);//延时2ms
    digitalWrite(pin_buzzer, LOW); //不发声音
    delay(1);//延时2ms
  }
}

void analyze(String token) {
  Serial.print("");
  Serial.println("");
  if (token.length() <= 0) {
    return ;
  }
  //Detect Commander
  token.toLowerCase();
  sender = token.substring(0, 2);
  command = token.substring(2, 6);
  
  
  Serial.print("Token :");
  Serial.println(token);
  Serial.print("Sender :");
  Serial.println(sender);
  Serial.print("Command :");
  Serial.println(command);
  args = token.substring(4);
  if (String("cl") == sender && String("door") == command) {
    // CL - > DOOR    DOOR_BELL _ CLICKED
    //door_responder();
    Serial.println("Door Bell's Ringing");
    door_buzzer();
    looprun();
  }
}

/**
  Setup
  */
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(57600);
  Serial3.begin(38400);
  
  pinMode(pin_buzzer, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
  
  
  pinMode(LeftI1, OUTPUT);     //I1和I2都是数字信号  
  pinMode(LeftI2, OUTPUT);     //通过设置I1和I2来控制电机旋转方向  
  pinMode(LeftEA, OUTPUT);     //按占空比方式输出的模拟信号  
  pinMode(RightI1, OUTPUT);    //I1和I2都是数字信号  
  pinMode(RightI2, OUTPUT);    //通过设置I1和I2来控制电机旋转方向  
  pinMode(RightEB, OUTPUT);    //按占空比方式输出的模拟信号  
  
  pinMode(46,OUTPUT);
  
  digitalWrite(43,HIGH);
  delay(500);
  digitalWrite(42,HIGH);
  pinMode(50,INPUT);
  
  init_bluetooth();
  
}


void loop() {
  //motor_update
  Serial.println("Looping");
  if (1 == get_bluetooth()) {
    return ;
  }
  timer ++;
  delay(1000);
}
