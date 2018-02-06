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

void doled(int i) {
  digitalWrite(13, i);
}

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
  while( digitalRead(50) == LOW) {
    delay(200);
    blue_connected = 0;
    doled(0);
  }
  doled(1);
  blue_connected = 1;
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
  connect_blue();
}

int get_bluetooth_message() {
  comdata = "";
  comdata = get_blue_String();
  analyze(comdata);
}


long timer;
long last;

int get_bluetooth() {
  if (blue_connected == 0) {
    init_bluetooth();
    last = timer;
  }else{
    if(timer - last >= 20 * 60){
      connect_blue();  
      last = timer;
    }
  }
  
  /**
  1.Get Connection Status
  2.Check Password.(Login) Not necessary
  3.Send Keep-Alive-Pack
  4.Get Response;
  5.Analyze Resonpse;
  */
  if (Serial3.available()) {
    get_bluetooth_message();
    return 1;
  }
  
  if (Serial.available()){
      comdata = get_pc_String();
      Serial3.print(comdata);
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
  args = token.substring(4);
  if (String("CL") == sender && String("door") == command) {
    // CL - > DOOR    DOOR_BELL _ CLICKED
    //door_responder();
    door_buzzer();
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
//  pinMode(13, TPUT);
  pinMode(50,INPUT);
  
  init_bluetooth();
  
}


void loop() {
  //motor_update
  if (1 == get_bluetooth()) {
    return ;
  }
  timer ++;
  delay(50);
}
