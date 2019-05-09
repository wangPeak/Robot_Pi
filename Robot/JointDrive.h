#pragma once

#include "iostream"					//标准输入输出
#include "math.h"

class JointDrive
{
public:
	JointDrive();
	~JointDrive();
	/*L1_大腿长度 L2_小腿长度 v_行进速度 T_周期 h_抬腿高度 P_关节平衡角*/
	JointDrive(double L1, double L2, double v, double T, double h,double P);
	
	
	double L = 0;			
	double s = 0;			//运动步长
	double Ah = 0;			//髋关节摆幅
	double Ak = 0;			//膝关节摆幅
	double RF_LB_K = 0;		//右 前腿 左 后腿 髋 关节驱动角
	double RF_LB_X = 0;		//右 前腿 左 后腿 膝 关节驱动角
	double LF_RB_K = 0;		//左 前腿 右 后腿 髋 关节驱动角
	double LF_RB_X = 0;		//左 前腿 右 后腿 膝 关节驱动角
	double _90_ADD_P = 0;
	double _90_SUB_P = 0;
	
	bool InitAhAkSign = false;
	// t = 10 * 2 * Math.PI / 360 * Math.Sin(2 * Math.PI / 0.8 * 0.0001) / 2 - Math.Abs(10 * 2 * Math.PI * Math.Sin(2 * Math.PI / 0.8 * 0.001)) / 2;
	bool Set_L1_L2(double L1, double L2);
	bool Set_Speed(double V);
	bool Set_T(double T);
	bool Set_h(double H);
	bool Set_P(double P);
	double get_P();
	bool DriveInit_Ah_Ak();
	bool Drive(double time);

protected:
	double l1 = 0;		//大腿长度
	double l2 = 0;		//小腿长度
	double v = 0;		//行进速度
	double T = 0;		//周期
	double h = 0;		//抬腿高度
	double P = 0;		//关节平衡角
};

