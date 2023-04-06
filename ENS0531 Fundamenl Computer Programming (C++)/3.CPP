/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：


定义一个复数类Complex，包含实部和虚部两个数据成员（double）。定义带有2个参数的
构造函数，编写成员函数show()显示复数值，其显示格式为 x+yi，
编写复数相减的成员函数 sub（Complex c1），并通过主函数进行测试。
期中 real 和 image为复数类Complex的数据成员。
输出结果见样张.JPG
-------------------------------------------------------*/
#include <iostream>
using namespace std;

/**********Program**********/
class Complex {
	  double real, image;
  public:
    Complex(double r, double i);
    Complex();
    void setComple(double r, double i);
    void sub(Complex x);
    void show();
};

Complex::Complex(double r, double i) {
  real = r;	
  image = i;
}
Complex::Complex() {
  real = 0;
  image = 0;
}
void Complex::setComple(double r, double i) {
  real = r;
  image = i;
}
void Complex::sub(Complex x) {	
	real -= x.real;
  image -= x.image;
}

/**********  End  **********/
void Complex::show()
 {
    if(image>0) {
      cout<<real<<"+"<<image<<"i"<<endl;
    }
    else if(image<0) {
      cout<<real<<"-"<<image<<"i"<<endl;
    }
    else {
      cout<<real<<endl;
    }
}

int main()
{
  Complex c1(4,5),c2;
  cout<<"C1"<<endl;
  c1.show();
  cout<<"C2"<<endl;
  c2.show();
  c2.setComple(6,7);
  cout<<"C2"<<endl;
  c2.show();
  c1.sub(c2);
  cout<<"C1"<<endl;
  c1.show();
  return 0;
}
