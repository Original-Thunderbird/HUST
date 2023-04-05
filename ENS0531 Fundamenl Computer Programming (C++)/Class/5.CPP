/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：编写程序完成一个三角形类Triangle，三角形类包含有a、b、c
三条边等基础属性（都为double类型）,该类对象拥有功能如下：
  （1）设置三条边的值；
  （2）判断三条边能否构成三角形；
  （3）求三角形的周长；
  （4）求三角形的面积;
  （5）求三角形三边边长的成员函数；
根据main函数，完成该程序。

输出结果见图：样张.JPG

要求与输出样张一样才能得到满分。
-------------------------------------------------------*/

#include <iostream>
#include <cmath>
using namespace std;

/**********Program**********/
class Triangle
{
	Triangle ( double a , double b , double c )
	{






/**********  End  **********/

int main( )
{
        Triangle  T1(3,4,5), T2;
  
        if(  T1.IsTriangle())
        { 
                cout<<"T1 Area is: "<<T1.GetArea()<<endl;
                cout<<"T1 Perimeter is: "<<T1.GetPerimeter()<<endl;

        }
        else{ 
                cout<<" no Triangle!"<<endl;
                cout<<T1.Geta()<<'\t'<<T1.Getb()<<'\t'<<T1.Getc()<<endl;
        }
        T2.setTriangle(17,8,9);
        if(  T2.IsTriangle())
        {
                cout<<"T2 Area is: "<<T2.GetArea()<<endl;
                cout<<"T2 Perimeter is: "<<T2.GetPerimeter()<<endl;
        }
        else
        { 
                cout<<" no Triangle!"<<endl;
                cout<<T2.Geta()<<'\t'<<T2.Getb()<<'\t'<<T2.Getc()<<endl;
        }

        return 0;
}
