#ifndef LEG_H
#define LEG_H
#include "triangle.h"
#include "legstate.h"
#include <wiringSerial.h>
#include <wiringPi.h>
#include <math.h>
class leg
{
public:
        const double thigh_Length;           //大腿长度
        const double shank_Length;           //小腿长度
        double thigh_Angle;                  //大腿角度
        double shank_Angle;                  //小腿角度
        double Stride_High;                  //抬腿高度
        double Stride_Length;                //抬腿长度
        double leg_Distance;                 //小腿端点与大腿根部的长度
        double leg_Angle;                    //leg_Distance与大腿所形成的角度
		
		bool init = false;                   //初始化状态

        leg(double,double,double,double);

		//通过当前腿的角度得到当前抬腿高度和抬腿长度
        LegState PositiveSolution();         
		
		//通过指定抬腿高度与长度得到角度
        void InverseSolution(LegState* State);
		
		
};

#endif // LEG_H
