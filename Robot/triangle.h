#ifndef TRIANGLE_H
#define TRIANGLE_H


class Triangle
{
public:
    Triangle();
    static double GetLength(double Length_A,double Length_B,double Angle);  //传入两边长A&B和一个角度AB求另一个边长
    static double GetAngle(double Length_A,double Length_B,double Length_C);  //传入三个边长求AB的夹角
};

#endif // TRIANGLE_H
