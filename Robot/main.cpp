#include <wiringPi.h>				//关于树莓派
#include <wiringSerial.h>

#include "robot.h"					//关于机器人
#include "softPwm.h"				//PWM头文件
#include "leg.h"
#include "legstate.h"
#include "JointDrive.h"

#include "iostream"					//标准输入输出

#include "pthread.h"                //线程用得的头文件
#include <unistd.h>					//线程用的sleep函数

// LED Pin - wiringPi pin 0 是 BCM_GPIO 17。
//利用 wiringPiSetupSys 进行初始化时必须使用 BCM 编号
//选择其他 pin 编号时，请同时使用 BCM 编号
//更新 Property Pages - Build Events - Remote Post-Build Event 命令
//它使用 gpio 导出进行 wiringPiSetupSys 的设置


using namespace std;

#define	LED	1
#define	L1	18.45
#define	L2	19.4
#define	RF_K	6			//右前腿髋关节
#define	RF_X	10			//右前腿膝关节
#define	LF_K	9			//左前腿髋关节
#define	LF_X	4			//左前腿膝关节
#define	RB_K	12			//右后腿髋关节
#define	RB_X	1			//右后腿膝关节
#define	LB_K	7			//左后腿髋关节
#define	LB_X	16			//左后腿膝关节



char	RF_K_Angle	=  0	;		//右前腿髋关节
char	RF_X_Angle	=  0	;		//右前腿膝关节
char	RB_K_Angle	=  0	;		//右后腿髋关节
char	RB_X_Angle	=  0	;		//右后腿膝关节
char	LF_K_Angle	=  0	;		//左前腿髋关节
char	LF_X_Angle	=  0	;		//左前腿膝关节
char	LB_K_Angle	=  0	;		//左后腿髋关节
char	LB_X_Angle	=  0	;		//左后腿膝关节
						

int State = 0;				//运行状态
int USART = 0;				//串口文件描述符
JointDrive myJointDrive(L1, L2, 40, 0.8, 5, 15);/*L1_大腿长度 L2_小腿长度 v_行进速度 T_周期 h_抬腿高度 P_关节平衡角*/



/*
设置舵机角度

	id =  舵机id
	angle = 设置的角度
	speed = 速度
*/
void SetAngle(char id, char angle, char speed)
{
	char a[10] = { 0 };
	a[0] = 0xFA;
	a[1] = 0xAF;
	a[2] = id;
	a[3] = 0x01;
	a[4] = angle;
	a[5] = speed;
	a[6] = 0x00;
	a[7] = 0x00;
	int temp = a[2] + a[3] + a[4] + a[5] + a[6] + a[7];
	a[8] = 0xFF;
	a[8] = a[8] & temp;
	a[9] = 0xED;
	write(USART, a, 10);
}

void DjReset(char id)
{
	char a[10] = { 0 };
	a[0] = 0x48;
	a[1] = ((id << 4) & 0xff);		//ID
	a[2] = 0x55;
	a[3] = 0x55;
	a[4] = 0x55;
	a[5] = 0x55;
	a[6] = 0x55;
	a[7] = 0x55;
	a[8] = 0x55;
	a[9] = 0x55;
	write(USART, a, 10);
}
void Dj_POS_Mode(char id)
{
	char a[10] = { 0 };
	a[0] = 0x48;
	a[1] = 0x01;					//功能序号
	a[1]|= (id << 4);			//ID
	a[2] = 0x04;					//位置模式
	a[3] = 0x55;
	a[4] = 0x55;
	a[5] = 0x55;
	a[6] = 0x55;
	a[7] = 0x55;
	a[8] = 0x55;
	a[9] = 0x55;
	write(USART, a, 10);
}


void Dj_Set_POS(char id ,int Position)
{
	char a[10] = { 0 };
	a[0] = 0x48;
	a[1] = 0x05;					//功能序号
	a[1]|= (id << 4);		//ID
	a[2] = 0x13;					//PWM_H			
	a[3] = 0x88;					//PWM_L
	a[4] = 0x55;
	a[5] = 0x55;
	a[6] = ((Position >> 24) & 0xff);
	a[7] = ((Position >> 16) & 0xff);
	a[8] = ((Position >> 8) & 0xff);
	a[9] = (Position & 0xff);
	write(USART, a, 10);
}



/*平衡控制函数*/
void *balance(void *arg) {

	while (1)
	{
		//cout << "平衡线程运行中..." << endl;
		usleep(500000);
	}

}
/*驱动函数*/
void *Drive(void *arg) {

	myJointDrive.DriveInit_Ah_Ak();
	while (1)
	{

		


		if (State == 1)		//运行状态
		{
			for (double time = 0; time < 10000; time += 0.01)
			{
				if (myJointDrive.Drive(time))
				{
					char d = char(90 + myJointDrive.get_P() + myJointDrive.RF_LB_K);
					char x = char(180 - d + 90 - myJointDrive.get_P() - myJointDrive.RF_LB_X);
					char y = char(90 + myJointDrive.get_P() + myJointDrive.LF_RB_K);
					char z = char(180 - d + 90 - myJointDrive.get_P() - myJointDrive.LF_RB_X);

					//char d = char(90 + myJointDrive.get_P() + myJointDrive.RF_LB_K);
					//char x = char(180 - (90 + myJointDrive.get_P()) + 90 - myJointDrive.get_P() - myJointDrive.RF_LB_X);
					//char y = char(90 + myJointDrive.get_P() + myJointDrive.LF_RB_K);
					//char z = char(180 - (90 + myJointDrive.get_P()) + 90 - myJointDrive.get_P() - myJointDrive.LF_RB_X);

					RF_K_Angle = d;
					RF_X_Angle = x;
					LF_K_Angle = y;
					LF_X_Angle = z;

					SetAngle(RF_K, RF_K_Angle, 1);
					SetAngle(RF_X, RF_X_Angle, 1);
					SetAngle(LB_K, RF_K_Angle, 1);
					SetAngle(LB_X, RF_X_Angle, 1);

					SetAngle(LF_K, LF_K_Angle, 1);
					SetAngle(LF_X, LF_X_Angle, 1);
					SetAngle(RB_K, LF_K_Angle, 1);
					SetAngle(RB_X, LF_X_Angle, 1);
					cout << myJointDrive.RF_LB_K << endl;
					cout << myJointDrive.RF_LB_X << endl;

					if (myJointDrive.RF_LB_X < 0.01)
					{
						leg myleg(RF_K_Angle - 90, RF_X_Angle + 90, L1, L2);  //设定好零点角度！！！！！！！！！！！！
						

					}

					usleep(15000);
				}

			}
		}
		if (State == 2)		//运行状态
		{
			for (double time = 0; time < 10000; time += 0.01)
			{
				if (myJointDrive.Drive(time))
				{
					
					RF_K_Angle = LB_K_Angle = char(90 - myJointDrive.get_P() + myJointDrive.RF_LB_K);
					RF_X_Angle = LB_X_Angle = char(180 - (myJointDrive._90_SUB_P * 2 - myJointDrive.RF_LB_X));
					LF_K_Angle = RB_K_Angle = char(90 - myJointDrive.get_P() + myJointDrive.LF_RB_K);
					LF_X_Angle = RB_X_Angle = char(180 - (myJointDrive._90_SUB_P * 2 - myJointDrive.LF_RB_X));

					//char d1 = char(90 - myJointDrive.get_P() + myJointDrive.RF_LB_K);
					//char x1 = char(180 - (d1 + 90 - myJointDrive.get_P() - myJointDrive.RF_LB_X));//myJointDrive._90_SUB_P*2
					SetAngle(15, 90, 1);
					SetAngle(5, 90, 1);
					SetAngle(3, 90, 1);
					SetAngle(2, 90, 1);

					SetAngle(RF_K, RF_K_Angle, 1);
					SetAngle(RF_X, RF_X_Angle, 1);
					SetAngle(LB_K, LB_K_Angle, 1);
					SetAngle(LB_X, LB_X_Angle, 1);

					SetAngle(LF_K, LF_K_Angle, 1);
					SetAngle(LF_X, LF_X_Angle, 1);
					SetAngle(RB_K, RB_K_Angle, 1);
					SetAngle(RB_X, RB_X_Angle, 1);
					cout << myJointDrive.RF_LB_K << endl;
					cout << myJointDrive.RF_LB_X << endl;

					if (myJointDrive.RF_LB_X < 0.01)
					{
						leg myleg(RF_K_Angle - 90, RF_X_Angle + 90, L1, L2);  //设定好零点角度！！！！！！！！！！！！


					}

					usleep(30000);
				}

			}
		}
		
	}

}



int main(void)
{
	State = 0; //==1运行
	wiringPiSetupSys();
	//wiringPiSetupGpio();
pinMode(LED, OUTPUT);
	
	/*启动初始化串口*/
	{

		if ((USART = serialOpen("/dev/ttyAMA0",115200)) == -1)
		{
			std::cout << "串口打开失败" << endl;
		}
		else
		{
			std::cout << "串口打开" << endl;
		}
	}

	/*启动线程*/
	{
		pthread_t T1;                               //线程1   用于控制机器人平衡
		if ((pthread_create(&T1, NULL, balance, NULL)) == 0)					//启动平衡 线程  
		{
			std::cout << "T1_平衡线程启动" << endl;

		}
		else
		{
			std::cout << "T1_平衡线程 Start Error !!!" << endl;
		}

		pthread_t T2;                               //线程2   用于输出控制函数
		if ((pthread_create(&T2, NULL, Drive, NULL)) == 0)					//启动平衡 线程  
		{
			std::cout << "T2_驱动函数线程启动" << endl;

		}
		else
		{
			std::cout << "T2_驱动函数线程 Start Error !!!" << endl;
		}
	}
	 
	//pinMode(1,PWM_OUTPUT);
	//pwmSetMode(PWM_MODE_MS);
	//pwmSetClock(192);
	//pwmToneWrite(1,50);
	//pwmSetRange(2000);

	while (true)
	{

		int a = sizeof(short);
		DjReset(0);
		//pwmWrite(1,50);
		//delay(1000);
		//pwmWrite(1,75);
		//delay(1000);




		//char d1 = char(90 - myJointDrive.get_P() + myJointDrive.RF_LB_K);
		//char x1 = char(180 - (d1 + 90 - myJointDrive.get_P() - myJointDrive.RF_LB_X));//myJointDrive._90_SUB_P*2

		
		//SetAngle(RF_K, 90, 1);
		//SetAngle(RF_X, 0, 1);
		//SetAngle(LB_K, 90, 1);
		//SetAngle(LB_X, 0, 1);

		//SetAngle(LF_K, 90, 1);
		//SetAngle(LF_X, 0, 1);
		//SetAngle(RB_K, 90, 1);
		//SetAngle(RB_X, 0, 1);
		//digitalWrite(LED, HIGH);  //开
		//delay(1500); // 毫秒
		//digitalWrite(LED, LOW);	  //关
		//delay(1500);
		//LegState myLegState;
		//myLegState.Stride_High = 50.0;
		//myLegState.Stride_Length = 20.0;
		//leg myleg(50, 40, 50, 40);
		//myleg.InverseSolution(&myLegState);
		//double ass = myLegState.shank_Angle;
		//double bss = myLegState.thigh_Angle;
		//std::cout << ass << endl;
		//std::cout << bss << endl;
		/*SetAngle(10, 90, 80);
		SetAngle(6, 135, 80);
		SetAngle(16, 180, 80);
		SetAngle(0, 0, 80);
		SetAngle(0, 180, 80);*/
		
	
		


	}
	return 0;
}