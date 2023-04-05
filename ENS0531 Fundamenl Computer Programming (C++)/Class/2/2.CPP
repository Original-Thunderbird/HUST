/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：

定义一个日期类Date，包含年、月、日三个数据成员（int），定义带有3个参数的构造函数，
以及一个求日期是当年的第几天的成员函数和输出日期的成员函数，
日期的显示格式为年/月/日。编写主函数进行测试。
（每年各月天数分别为31,28,31,30,31,30,31,31,30,31,30,31，闰年2月为29天，
闰年的条件year%4==0&&year%100!=0)||year%400==0)）
输出结果见样张.JPG
-------------------------------------------------------*/
#include <iostream>
using namespace std;

/**********Program**********/
class Date 
{
	int year , month , day ;
    public :
		Date ( int y , int m , int d ) ;
		Date ( ) ;
		void SetD ( int y , int m , int d ) ;
		void show () ;
		int getDay () ;

} ;
Date :: Date ( int y , int m , int d )     {	year = y ;	month = m ; 	day = d ;  }
Date :: Date ( )   { }
void Date :: SetD ( int y , int m , int d )    {	year = y ;	month = m ; 	day = d ;  }
int Date :: getDay () 
{
	int date = 0 ;
	switch ( month-1 )
	{
    	case 11 :		date += 30 ;
	    case 10 :		date += 31 ;
    	case 9 :		date += 30 ;
    	case 8 :		date += 31 ;
    	case 7 :		date += 31 ;
    	case 6 :		date += 30 ;
    	case 5 :		date += 31 ;
    	case 4 :		date += 30 ;
    	case 3 :		date += 31 ;
    	case 2 :		date += 28 ;
    	case 1 :		date += 31 ;
	}
	date += day ;
	if ( year%4==0&&year%100!=0||year%400==0 )		date += 1 ;
	return date ;
}

/**********  End  **********/
void Date::show()
{
        cout<<year<<"/"<<month<<"/"<<day<<endl;

}
int main()
{
        Date d1(2009,2,1),d2;   //d1为2009年2月1日
        d1.show();
        cout<<d1.getDay()<<endl;
        d2.SetD(2012,3,1);    //d2为2012年3月1日
        d2.show();
        cout<<d2.getDay()<<endl;
        return 0;
}

