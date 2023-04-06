/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：定义一个描述学生基本情况的类Student，数据成员包括学号、

C++成绩、英语和数学成绩，成员函数包括获取学号、求出总成绩和平均成绩，格式：
                
long getID() 
int sum ()
double avg ()

输出结果见图：样张.JPG
-------------------------------------------------------*/
#include <iostream>
using namespace std;

/**********Program**********/
class Student
{
	double ID , AVG ;
	int CP , EN , MA , SUM ;
    public :
		Student ( double id , int cp , int en , int ma )
		{
			ID = id ; CP = cp ; EN = en ; MA = ma ;
		}
    Student() {}
		void Set ( double id , int cp , int en , int ma )
		{
			ID = id ; CP = cp ; EN = en ; MA = ma ;
		}
		double getID ()
		{
			return ID ;
		}
		int sum ()
		{
			SUM =  CP + EN + MA ;
			return SUM ;
		}
		double avg ()
		{
			AVG = SUM / 3 ;
			return AVG ;
		}
} ;

/**********  End  **********/
int main(void)
{
  Student s(201211145, 90, 80, 97),s1;
  cout<<"ID："<<s.getID()<<endl;
  cout<<"\tThe sum of source："<<s.sum()<<endl;  
  cout<<"\tThe avg of source："<<s.avg()<<endl;
  s1.Set(201212345,81,82,81); 
  cout<<"ID："<<s1.getID()<<endl;
  cout<<"\tThe sum of source："<<s1.sum()<<endl;  
  cout<<"\tThe avg of source："<<s1.avg()<<endl;
  return 0;
}



