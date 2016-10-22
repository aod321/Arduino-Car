# Arduino-Car
## Logo

![Digilent创客空间](https://thumbnail0.baidupcs.com/thumbnail/47574a361a7d4d81084a402014f0abf9?fid=1812361213-250528-417519803024670&time=1477112400&rt=sh&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-NL%2FRtAt7v7p0h2oMoQVbkVnIYRw%3D&expires=8h&chkv=0&chkbd=0&chkpc=&dp-logid=6854579400436596414&dp-callid=0&size=c710_u400&quality=100)
## 代码说明
本代码为中国矿业大学Digilent创客部落培训用实例代码，用于演示Arduino制作循线和超声波避障小车，代码有中文注释，可自由下载交流。
## 材料清单
- Arduino主控板x1（推荐UNO，MEGA或Due）；- 小车底盘(含直流电机)x1；- L298N双H桥直流电机驱动板x1；- 红外传感器X2；- HC-SR04 超声波模块X1，超声波云台支架X1- 舵机X1- 18650电池组x1；- 面包板x1（可选）；- 杜邦线若干；
## 引脚说明

各引脚定义代码如下，按照代码接线即可：

    //车轮控制线
	const int Left_Zheng = 3;
	const int Left_Fan = 5;
	const int Right_Zheng = 6;
	const int Right_Fan = 11;
	//循线
	#define LED1  digitalRead(4)
	#define LED2  digitalRead(2)
	//超声波
	const int TrigPin = 15;
	const int EchoPin = 16;
	//舵机接9号口
	Servo myservo;  // create servo object to control 
	myservo.attach(9); 
 									

## 友情链接
[殷子的博客](https://aod321.github.io)
![免费学习机房](http://e.hiphotos.baidu.com/baike/c0%3Dbaike180%2C5%2C5%2C180%2C60/sign=8f3cabd7ad64034f1bc0ca54ceaa1254/9f2f070828381f302de3e8a3af014c086f06f0c6.jpg)

