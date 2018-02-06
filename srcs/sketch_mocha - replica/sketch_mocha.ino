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


int motor_pinA = 23;
int motor_pinB = 24;
int motor_pinE = 8;

// Motor Related
const int motor_running = 30;
const int motor_stopped = 40;
const int motor_freestyle = 43;

int motor_clockwise = 0;
int motor_speed = 100;
int motor_pos = motor_freestyle;


//Bluetooth Pins;
int toTX = 10; // 17
int toRX = 11; // 16


String clientkey = "2014,10,102506";

//BlueTooth Soft Serial
SoftwareSerial Serial3(toTX,toRX); // RX, TX

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
      comdata += String(char(Serial3.read()));
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

void bluep(String s) {
  Serial3.print(s + String("\r\n"));
}

String temp;
void connect_blue() {
  // Connect Blue;
  // If OK
  Serial3.print("AT\r\n"); // Search In Pair List
  temp = get_blue_String();
  Serial.println(String("Connecting Bluetooth : ") + temp + String(";"));
  
  Serial3.print("AT+STATE\r\n"); // Search In Pair List
  temp = get_blue_String();
  temp.toLowerCase();
  if (temp != "+state:connected") {
    blue_connected = 0;
    doled(0);
  }
  delay(300);
  while (blue_connected == 0) {
    //    11a2 34 01b35f
    Serial3.print(String("AT+FSAD=") + clientkey); // Search In Pair List
    temp = get_blue_String();
    if (temp == "OK") {
      // IN the Pair List
      Serial3.print(String("AT+LINK=") + clientkey );
      if (temp == "OK") {
        // LINKED
        doled(1);
        blue_connected = 1;
        return;
      }
    } else {
      Serial3.print(String("AT+PAIR=") + clientkey + String(",5"));
      delay(5 * 1100); // Wait For 5 Seconds
      // OK
      // DOLED ?
    }
  }
}




int check_bluetooth() {
  digitalWrite(bluekey, HIGH);
  Serial3.begin(38400);
  // Off Light  
  doled(1);
  int o = 0;
  String temp;

  // Connected Bluetooth;

  while (o == 0) {
    Serial.write("Detect First AT");
    Serial3.write("AT\r\n");
    temp = get_blue_String(); // Ignored
    if (temp == "OK") {
      Serial.write("OK Detected");
      o = 1;
    }
    delay(1000);
  }
  
  
  // Blink
  
  doled(1);
  delay(3000);
  doled(0);

  // Initialize
  Serial.print("ATINIT");
  Serial3.print("AT+INIT\r\n");
  temp = get_blue_String();
  // AT+BAUD4
  Serial3.print("AT+UART=9600,0,0\r\n");
  temp = get_blue_String();
  delay(300);
  Serial3.print("AT+ROLE=1\r\n");
  temp = get_blue_String();
  delay(300);
  Serial3.print("AT+NAME=MOCHA\r\n");
  temp = get_blue_String();
  delay(300);
  Serial3.print("AT+BIND=11a2,34,05a5ce\r\n");
  temp = get_blue_String();
  // CLIENT 11a2 34 01b35f
  delay(1000);
  // "AT+PIN8274" = > "AT+PSWD=8274"
  Serial3.print("AT+PSWD=8274\r\n");
  temp = get_blue_String();
  Serial.print("All Settings Gone");

  // LED Blink

  doled(1);
  delay(3000);

  connect_blue();
}
int get_bluetooth_message() {
  comdata = "";
  comdata = get_blue_String();
  analyze(comdata);
}

int get_bluetooth() {
  if (bluetooth_status == 0) {
    check_bluetooth();
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
    return 0;
  }
  //if (Serial.available()){
  //       get_bluetooth_anti_message();
  //        return 1;
  //}
  return -1;
}


void door_responder() {
  // Respond
  Serial3.println("SVdoor"); // A
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
    door_responder();
    door_buzzer();

  } else if (String("door") == command && String("PH") == sender) {
    er_a("Report Sonar Set.");
    if (args == " open") {
    } else if (args = " close") {
    } else {
      er_a("Not on/off in reson");
    }
  } else if (String("speed") == command && String("PH") == sender) {
    if (args.length() <= 4) {
      motor_speed = args.toInt();
      er_a("Speed Set.");
      motor_set();
    } else {
      er_a("Speed Irrational");
      er_a(args);
    }
  } else {
    er_a("MESSEGE Not Acceptable");
  }
}

int er_a(String st) {
  bluep(st);
  Serial.println(st);
}
/**
  Setup
  */
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  //Serial.println("Hello Monitor !");
  /*     pinModes     */
  //\---Sonar
  //\---Infare
  
  pinMode(bluekey,OUTPUT);
  pinMode(pin_buzzer, OUTPUT);
  pinMode(13, OUTPUT);
  //  pinMode(infarepin2,INPUT);
  //\---Led
  //pinMode(ledpin,OUTPUT);
  //\---Motor
  pinMode(motor_pinA, OUTPUT);
  pinMode(motor_pinB, OUTPUT);
  pinMode(motor_pinE, OUTPUT);
  //while (!Serial) {
  //; // wait for serial port to connect. Needed for Leonardo only
  //}
  // set the data rate for the SoftwareSerial port

  //Serial3.println("Hello, world?");
  //Test Wheather Bluetooth is connected.
  //Default Settings
  /*
  motor_speed = 255;
  motor_clockwise = 0;
  motor_pos = motor_freestyle;
  motor_set();
  */
}
/*
int motor_update() {
  analogWrite(motor_pinE, motor_speed);
  return 0;
}
*/


int get_wifi() {
  // Get WIFI
  return 0;
}


void loop() {
  //motor_update
  if (1 == get_bluetooth()) {
    return ;
  }
  //Wifi Update
  if (1 == get_wifi()) {
    return ;
  }
  /*
  switch(kstate){
     case 2: // opened
        //motor_pos = motor_freestyle;
        //motor_set();
      break;
     case 3: // need opening
       // open
       //state = opened;
       //motor_pos = motor_running;
       //motor_set();
       break;
   }
  }
  */
}

// motor_set
/*
void motor_set() {
  int mspeed = motor_speed;
  int mclock = motor_clockwise;
  int mpos = motor_running;
  //mpos : Stop, Runing, Freestyle
  if (mpos == motor_running) {
    analogWrite(motor_pinE, mspeed);
    if (mclock == 0) {
      digitalWrite(motor_pinA, HIGH);
      digitalWrite(motor_pinB, LOW);
    } else {
      digitalWrite(motor_pinA, LOW);
      digitalWrite(motor_pinB, HIGH);
    }
  } else if (mpos == motor_stopped) {
    analogWrite(motor_pinE, mspeed);
    digitalWrite(motor_pinA, HIGH);
    digitalWrite(motor_pinB, HIGH);
  } else if (mpos == motor_freestyle) {
    analogWrite(motor_pinE, mspeed);
    digitalWrite(motor_pinA, LOW);
    digitalWrite(motor_pinB, LOW);
  }
}
//Software Serial Module Initialized
/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
  This example code is in the public domain.
  */
