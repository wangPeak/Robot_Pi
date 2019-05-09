#include "JointDrive.h"

using namespace std;
JointDrive::JointDrive()
{
}


JointDrive::~JointDrive()
{
}
/*
L1	大腿长度
L2	小腿长度
v	速度
T	周期
h	抬腿高度
P	关节平衡角
*/
JointDrive::JointDrive(double L1, double L2, double v, double T, double h, double P)
{
	if (L1 > 0 || L2 > 0 || v > 0 || T > 0 || h > 0 || P > 0)
	{
		this->l1 = L1;
		this->l2 = L2;
		this->v = v;
		this->T = T;
		this->h = h;
		this->P = P;
		this->InitAhAkSign = true;
		this->_90_ADD_P = 90 + this->P;
		this->_90_SUB_P = 90 - this->P;
	}
	else
	{
		this->InitAhAkSign = false;
		cout << "Error_驱动函数参数 错误  !！！" << endl;
	}
	
}
bool JointDrive::Set_L1_L2(double L1, double L2)
{
	if (L1 > 100 || L2 > 100 || L1 < 1 || L2 < 1)
	{
		cout << "Error 关节驱动函数 关节长度设置错误" << endl;
		return false;
	}
	else
	{
		this->l1 = L1;
		this->l2 = L2;
		this->InitAhAkSign = false;
		return true;
	}
}
bool JointDrive::Set_Speed(double V)
{
	if (V > 100 || V < 1)
	{
		cout << "Error 关节驱动函数 速度设置错误" << endl;
		return false;
	}
	else
	{
		this->v = V;
		this->InitAhAkSign = false;
		return true;
	}
}
bool JointDrive::Set_T(double T)
{
	if (T > 100 || T < 0.2)
	{
		cout << "Error 关节驱动函数 步态周期设置错误" << endl;
		return false;
	}
	else
	{
		this->T = T;
		this->InitAhAkSign = false;
		return true;
	}
}
bool JointDrive::Set_h(double H)
{
	if (H > 100 || H < 0.5)
	{
		cout << "Error 关节驱动函数 抬腿高度设置错误" << endl;
		return false;
	}
	else
	{
		this->h = H;
		this->InitAhAkSign = false;
		return true;
	}
}
bool JointDrive::Set_P(double P)
{
	if (P > 180 || P < 1)
	{
		cout << "Error 关节驱动函数 关节平衡角设置错误" << endl;
		return false;
	}
	else
	{
		this->P = P;
		this->InitAhAkSign = false;
		return true;
	}
}
double JointDrive::get_P()
{
	return this->P;
}
bool JointDrive::DriveInit_Ah_Ak()
{
	this->L = (l1 + l2)*cos(M_PI/180.0*this->P);
	this->s = this->v * this->T / 2;
	this->Ah = asin(this->s / (2 * this->L));
	this->Ak = acos((this->l2 * cos(M_PI / 180.0*this->P) - this->h) / this->l2) - (M_PI/180*this->P);
	if (this->Ah > 180 || this->Ak > 180)
	{
		cout << "Error !! Ah || Ak 大于180 请检查参数 " << endl;
		return false;
	}
	this->_90_ADD_P = 90 + this->P;
	this->_90_SUB_P = 90 - this->P;
	this->InitAhAkSign = true;
	return true;
}
bool JointDrive::Drive(double time)
{	
	
	time = double(time * 10000.000f + 0.1) / 10000.000f;
	if (!this->InitAhAkSign)
	{	
		cout << "InitAhAkSign 为 false 无法计算驱动函数" << endl;
		return false;
	}
 
	this->RF_LB_K = 180 / M_PI * (this->Ah * sin((2 * M_PI * time / this->T) - M_PI_2));
	this->RF_LB_X = 180 / M_PI * (abs(this->Ak * sin((2 * M_PI * time / this->T + M_PI)) / 2) - (this->Ak * sin(2 * M_PI * time / this->T + M_PI)) / 2);
	
	this->LF_RB_K = 180 / M_PI * (this->Ah * sin((2 * M_PI * time / this->T) + M_PI_2));
	this->LF_RB_X = 180 / M_PI * (abs(this->Ak * sin((2 * M_PI * time / this->T)) / 2) - (this->Ak * sin(2 * M_PI * time / this->T)) / 2);
	
	
	if (abs(this->RF_LB_K) > 90 || abs(this->RF_LB_X) > 90 || abs(this->LF_RB_K) > 90 || abs(this->LF_RB_X) > 90)
	{
		cout << "Error 驱动函数计算错误，计算出有一关节输出大于90" << endl;
		return false;
	}
	else
	{
		return true;
	}
	//this->RF_LB_X = -this->Ak
}




