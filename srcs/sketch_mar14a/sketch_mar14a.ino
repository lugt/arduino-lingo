int LeftI1 = 23;     //连接电机驱动板的I1接口  
int LeftI2 = 24;     //连接电机驱动板的I2接口  
int LeftEA = 27;      //连接电机驱动板的EA接口  
int RightI1 = 25;    //连接电机驱动板的I1接口  
int RightI2 = 26;    //连接电机驱动板的I2接口  
int RightEB = 28;     //连接电机驱动板的EB接口  
int StepCount = 0;  
int StepDelayTime=1500;  
  
void setup()  
{  
  pinMode(LeftI1, OUTPUT);     //I1和I2都是数字信号  
  pinMode(LeftI2, OUTPUT);     //通过设置I1和I2来控制电机旋转方向  
  pinMode(LeftEA, OUTPUT);     //按占空比方式输出的模拟信号  
  pinMode(RightI1, OUTPUT);    //I1和I2都是数字信号  
  pinMode(RightI2, OUTPUT);    //通过设置I1和I2来控制电机旋转方向  
  pinMode(RightEB, OUTPUT);    //按占空比方式输出的模拟信号  
  Serial.begin(9600);          //设置波特率  
}  
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
