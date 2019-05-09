#include "leg.h"

leg::leg(double thigh_angle,double shank_angle,double thigh,double shank):thigh_Length(thigh),shank_Length(shank)
{
    if (shank <= 0 || thigh <= 0) {
        this->init = false;
    }
    if(thigh_angle<180.0 && shank_angle <= 180.0) {
        this->thigh_Angle = thigh_angle;
        this->shank_Angle = shank_angle;
        this->init = true;
    }
    else {
        this->init = false;
    }
    if (init) {

       this->leg_Distance = Triangle::GetLength(thigh_Length,shank_Length,shank_Angle);
       this->leg_Angle = Triangle::GetAngle(this->thigh_Length,this->leg_Distance,this->shank_Length);
    }
}
LegState leg::PositiveSolution()
{
    this->leg_Distance = Triangle::GetLength(this->thigh_Length,this->shank_Length,this->shank_Angle);
    this->leg_Angle = Triangle::GetAngle(this->thigh_Length,this->leg_Distance,this->shank_Length);  //
   double Angle = this->thigh_Angle+this->leg_Angle;                       //     <--         角度互补  ——————————————  两条平行线中间角度互补
   double radian = Angle*3.14159/180.0;                                    //                                  /
                                                                           //                                 /
    LegState temp;                                                         //                                /
    temp.Stride_High = this->leg_Distance*sin(radian);                     //                          ——————————————
    temp.Stride_Length = this->leg_Distance*cos(radian);
    return temp;

}
/*
通过指定抬腿高度与长度得到角度
*/
void leg::InverseSolution(LegState* State )
{
    this->leg_Distance = Triangle::GetLength(State->Stride_High,State->Stride_Length,90);
    double HighAngle =  Triangle::GetAngle(State->Stride_High,this->leg_Distance,State->Stride_Length);       //控制直角三角形上面的角度
    this->leg_Angle = Triangle::GetAngle(this->leg_Distance,this->thigh_Length,this->shank_Length);         //leg_Distance与大腿所形成的角度
    State->thigh_Angle = 90.0 - (this->leg_Angle + HighAngle);
    //this->thigh_Angle = State.thigh_Angle;
    State->shank_Angle = Triangle::GetAngle(this->thigh_Length,this->shank_Length,this->leg_Distance);
}
