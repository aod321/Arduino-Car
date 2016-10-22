//***************************************************************************************************************
//*****************************************全局变量和宏定义区*****************************************
//***常量标记位定义，标记左右和正反***
#include <Servo.h>
const int Left = 0;
const int Right = 1;
const int Zheng = 0;
const int Fan = 1;
//******************************
//************引脚定义**********
//车轮控制线
const int Left_Zheng = 3;
const int Left_Fan = 5;
const int Right_Zheng = 6;
const int Right_Fan = 11;

//五路循迹(实例用两路)
#define LED1  digitalRead(4)
#define LED2  digitalRead(2)
// #define LED3  digitalRead(17)
// #define LED4  digitalRead(18)
// #define LED5  digitalRead(19)

//ESP8266 wifi模块
//const int ESP_RX=0;
//const int ESP_TX=1;

//超声波
const int TrigPin = 15;
const int EchoPin = 16;

//LCD1602液晶显示
//int LCD1602_RS = 14;
//int LCD1602_RW = 13;
//int LCD1602_EN = 12;
//int DB[] = { 6, 7, 8, 9};


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

//    int cm;

//***************************************************************************************************************
//**************************************************函数声明区****************************************************

void LCD_Command_Write(int);
void LCD_Data_Write(int);
void LCD_SET_XY(int, int);
void LCD_Write_Char( int, int, int);
void LCD_Write_String(int, int, char *);
void car_Control(int, int, int);
int get_Distance();
void output_Distance();
void xunJi();
void headMoveControl(int , int );
void bizhang();
void Forward();
void Stop();
void Turn_Left();
void Turn_Right();

//***************************************************************************************************************
//***************************************************函数定义区***************************************************

//*****************************初始化程序**********************
void setup ()
{
  Serial.begin(9600);           //串口开始，输出当前状态，便于调试
  //********I/O口初始化********
  int i = 0;
  pinMode(TrigPin, OUTPUT);  //超声波输出
  pinMode(EchoPin, INPUT);  //回声输入
  for (i = 6; i <= 14; i++)
  {
    pinMode(i, OUTPUT);

  }

  pinMode(2, INPUT); //循迹红外输入
  pinMode(4, INPUT); //循迹红外输入
  //   pinMode(18,INPUT); //循迹红外输入
  //   pinMode(19,INPUT); //循迹红外输入
  pinMode(Left_Zheng, OUTPUT);
  pinMode(Left_Fan, OUTPUT);
  pinMode(Right_Zheng, OUTPUT);
  pinMode(Right_Fan, OUTPUT);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo objectv
  myservo.write(120);  //超声波朝前
  Forward();


}
char Mode = 'X';



//***************主循环***************
void loop()
{
  while (Serial.available() > 0)
  {
    Mode = Serial.read();
  }
  chose_Mode();
}

void chose_Mode()
{
  switch (Mode)
  {
     
    case 'X': xunJi(); break;
    case 'B':
      {
        int currDist=get_Distance();
        if (currDist > 25) {
          Forward();
        }
        else if (currDist < 15) {
          bizhang();
        }
        else if(currDist > 15){
          
          car_Control(Left,Zheng,65);                   //减速
          car_Control(Right,Fan,65);
        }


         break;
      }
    default: xunJi();  break;
  }
}
//***********循迹程序**********
void xunJi()
{
  if (LED1 == 0 && LED2 == 0)           //如果左右都未检测到
  {
    car_Control(Left, Zheng, 100);
    car_Control(Right, Zheng, 100);
    Serial.println("Go Straight");

  }
  if (LED1 == 1 && LED2 == 0)           //如果左边检测到线，右未检测到
  {
    car_Control(Left, Fan, 50);
    car_Control(Right, Zheng, 100);
    Serial.println("Turn left");

  }
  if (LED1 == 0 && LED2 == 1)         //如果右检测到，左未检测到
  {
    car_Control(Left, Zheng, 100);
    car_Control(Right, Fan, 50);
    Serial.println("Turn right");
  }
  if (LED1 == 1 && LED2 == 1)         //如果左右都检测到，直行
  {
    car_Control(Left, Zheng, 100);
    car_Control(Right, Zheng, 100);
    Serial.println("Go Straight");
  }

}


//********串口输出测距函数******
void output_Distance()
{
  //    char ge,shi,bai;
  int cm;
  cm = get_Distance();
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  //      ge=cm%10;
  //      shi=(cm%100)/10;
  //      bai=cm/100;
  //        LCD_Write_String(0,0,"dis:");
  //        LCD_Data_Write('0'+bai);
  //        LCD_Data_Write('0'+shi);
  //        LCD_Data_Write('0'+ge);
  //        LCD_Data_Write('C');
  //        LCD_Data_Write('m');
  //        delay(1000);
  //       delay(1000);


}

//*********测距函数**********
int get_Distance()
{
  int cm;
  digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm
  Serial.print(cm);
  Serial.println("cm");
  return cm;
  //cm = (int(cm * 100.0)) / 100.0; //保留两位小数

}

//*********车轮控制函数********************
//第一个参数决定左右车轮0左1右，第二个参数决定转的方向0正1反，第三个参数决定速度，pwm占空比0-255
void car_Control(int LOR, int ZOF, int Speed)
{
  switch (LOR)
  {
    case Left:
      {
        switch (ZOF)
        {
          case Zheng:    analogWrite(Left_Fan, 0); analogWrite(Left_Zheng, Speed); break;
          case Fan:   analogWrite(Left_Zheng, 0); analogWrite(Left_Fan, Speed);  break;
          default : break;
        }
        break;
      }
    case Right:
      {
        switch (ZOF)
        {
          case Zheng:    analogWrite(Right_Fan, 0);  analogWrite(Right_Zheng, Speed); break;
          case Fan:    analogWrite(Right_Zheng, 0); analogWrite(Right_Fan, Speed);  break;
          default : break;
        }
        break;
      }
    default: break;
  }

}



//
//***********超声波云台摆动控制********************
void headMoveControl(int pos, int delaytime = 15)
{

  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(delaytime);                       // waits 15ms for the servo to reach the position

}



//**************车辆运行****
void Forward()
{
  car_Control(Left, Zheng, 110);
  car_Control(Right, Zheng, 110);
  Serial.println("Forward");
}
void Stop()           //停车
{
  myservo.write(120);   //探头朝前
  car_Control(Left, Zheng, 0);
  car_Control(Right, Zheng, 0);
  Serial.println("Stop");
}
void Back()
{
  car_Control(Left, Fan, 100);
  car_Control(Right, Fan, 100);
  Serial.println("Back");
}
void Turn_Left()
{
  car_Control(Left, Fan, 70);
  car_Control(Right, Zheng, 70);
  Serial.println("Turn_Left");
}
void Turn_Right()
{
  car_Control(Left, Zheng, 70);
  car_Control(Right, Fan, 70);
  Serial.println("Turn_Right");

}


//************避障子程序***********************
void bizhang()
{
  Stop();
  headMoveControl(0, 500);
  int lDist = get_Distance();
  Stop();
  headMoveControl(180, 500);
  int rDist = get_Distance();
  Stop();
  if (lDist < rDist) {
    Turn_Left();
    delay(1000);
  }
  else {
    Turn_Right();
    delay(1000);
  }
  return;



}


