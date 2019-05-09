#include "triangle.h"
#include <math.h>
Triangle::Triangle()
{

}
double Triangle::GetLength(double Length_A,double Length_B,double Angle)
{
    double radian = Angle*M_PI/180.0;
    double Length_2 = pow(Length_A,2)+pow(Length_B,2)-2*Length_A*Length_B*cos(radian);
    return pow(Length_2,0.5);
}
double Triangle::GetAngle(double Length_A,double Length_B,double Length_C)
{
    double temp = pow(Length_A,2)+pow(Length_B,2)-pow(Length_C,2);
    double temp2 =2*Length_A*Length_B;
    return 180/M_PI*(acos(temp/temp2));

}
