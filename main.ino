//***常量标记位定义，标记左右和正反***
const int Left=0;
const int Right=1;
const int Zheng=0;
const int Fan=1;
//******************************
//************引脚定义**********
//车轮控制线
const int Left_Zheng=3;
const int Left_Fan=5;
const int Right_Zheng=10;
const int Right_Fan=11;

//循迹
 #define LED1  digitalRead(2)
 #define LED2  digitalRead(4)
 #define LED3  digitalRead(17)
 #define LED4  digitalRead(18)
 #define LED5  digitalRead(19)
//ESP8266
const int ESP_RX=0;
const int ESP_TX=1;
//超声波   
const int TrigPin = 15;  
const int EchoPin = 16; 
//LCD1602
int LCD1602_RS=14;   
    int LCD1602_RW=13;   
    int LCD1602_EN=12;   
    int DB[] = { 6, 7, 8, 9};
   
    int cm; 
//******************************
//*********LCD相关函数******************** 
void LCD_Command_Write(int command)
    {
    int i,temp;
    digitalWrite( LCD1602_RS,LOW);
    digitalWrite( LCD1602_RW,LOW);
    digitalWrite( LCD1602_EN,LOW);
     
    temp=command & 0xf0;
    for (i=DB[0]; i <= 9; i++)
    {
       digitalWrite(i,temp & 0x80);
       temp <<= 1;
    }
     
    digitalWrite( LCD1602_EN,HIGH);
    delayMicroseconds(1);
    digitalWrite( LCD1602_EN,LOW);
     
    temp=(command & 0x0f)<<4;
    for (i=DB[0]; i <= 9; i++)
    {
       digitalWrite(i,temp & 0x80);
       temp <<= 1;
    }
     
    digitalWrite( LCD1602_EN,HIGH);
    delayMicroseconds(1);
    digitalWrite( LCD1602_EN,LOW);
    }
     
    void LCD_Data_Write(int dat)
    {
    int i=0,temp;
    digitalWrite( LCD1602_RS,HIGH);
    digitalWrite( LCD1602_RW,LOW);
    digitalWrite( LCD1602_EN,LOW);
     
    temp=dat & 0xf0;
    for (i=DB[0]; i <= 9; i++)
    {
       digitalWrite(i,temp & 0x80);
       temp <<= 1;
    }
     
    digitalWrite( LCD1602_EN,HIGH);
    delayMicroseconds(1);
    digitalWrite( LCD1602_EN,LOW);
     
    temp=(dat & 0x0f)<<4;
    for (i=DB[0]; i <= 9; i++)
    {
       digitalWrite(i,temp & 0x80);
       temp <<= 1;
    }
     
    digitalWrite( LCD1602_EN,HIGH);
    delayMicroseconds(1);
    digitalWrite( LCD1602_EN,LOW);
    }
     
    void LCD_SET_XY( int x, int y )
    {
      int address;
      if (y ==0)    address = 0x80 + x;
      else          address = 0xC0 + x;
      LCD_Command_Write(address);
    }
     
    void LCD_Write_Char( int x,int y,int dat)
    {
      LCD_SET_XY( x, y );
      LCD_Data_Write(dat);
    }
       void LCD_Write_String(int X,int Y,char *s)
    {
        LCD_SET_XY( X, Y );    //设置地址
        while (*s)             //写字符串
        {
          LCD_Data_Write(*s);   
          s ++;
        }
    }
     
//*********控制函数******************** 
void Car_control(int LOR,int ZOF,int Speed)
{
  
  switch (LOR)
  {
    case Left: 
  {
    switch (ZOF)
     {
        case Zheng:    analogWrite(Left_Fan,0); analogWrite(Left_Zheng,Speed);break;
        case Fan:   analogWrite(Left_Zheng,0);analogWrite(Left_Fan,Speed);  break;
        default : break;
      }
   break;
  }
    case Right:
  {
    switch (ZOF)
      {
        case Zheng:    analogWrite(Right_Fan,0);  analogWrite(Right_Zheng,Speed);break;
        case Fan:    analogWrite(Right_Zheng,0);analogWrite(Right_Fan,Speed);  break;
        default : break;
      }
      break;
  }
    default: break;
   }

}
//*****************************





void setup ()
{
  Serial.begin(9600);           //串口开始，输出当前状态，便于调试
 int i = 0;
     pinMode(TrigPin, OUTPUT); 
      pinMode(EchoPin, INPUT); 
         for (i=6; i <= 14; i++)
       {
         pinMode(i,OUTPUT);
       }
       
   pinMode(2,INPUT);
   pinMode(4,INPUT);
   pinMode(17,INPUT);
   pinMode(18,INPUT);
   pinMode(19,INPUT);
  pinMode(Left_Zheng,OUTPUT);
  pinMode(Left_Fan,OUTPUT);
  pinMode(Right_Zheng,OUTPUT);
  pinMode(Right_Fan,OUTPUT);
   delay(100);
      LCD_Command_Write(0x28);//4线 2行 5x7
      delay(50);
      LCD_Command_Write(0x06);
      delay(50);
      LCD_Command_Write(0x0c);
      delay(50);
      LCD_Command_Write(0x80);
      delay(50);
      LCD_Command_Write(0x01);
      delay(50);

}
//********测距并输出函数******
void outputdistance()
{
   char ge,shi,bai;
     digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
delayMicroseconds(2); 
digitalWrite(TrigPin, HIGH); 
delayMicroseconds(10); 
digitalWrite(TrigPin, LOW); 

cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm 
//cm = (int(cm * 100.0)) / 100.0; //保留两位小数 

Serial.print(cm); 
Serial.print("cm"); 
Serial.println(); 
ge=cm%10;
shi=(cm%100)/10;
bai=cm/100;
LCD_Write_String(0,0,"dis:");
LCD_Data_Write('0'+bai);
LCD_Data_Write('0'+shi);
LCD_Data_Write('0'+ge);
LCD_Data_Write('C');
LCD_Data_Write('m');

delay(1000); 
delay(1000); 
   
}
void loop()
{
          
  outputdistance();
  if(LED1==0 && LED2 ==0)
  {
    Car_control(Left,Fan,100);
    Car_control(Right,Zheng,100);
    Serial.println("Turn left_big");
    
  }
  if(LED1==1 && LED2 ==0)
  {
    Car_control(Left,Zheng,50);
    Car_control(Right,Zheng,100);
        Serial.println("Turn left");

  }
    if(LED1==0 && LED2 ==1)
  {
    Car_control(Left,Zheng,100);
    Car_control(Right,Zheng,50);
    Serial.println("Turn right");
  }
  
  }


