/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：

补充构造一个日期时间类（Timedate），数据成员包括年、月、日和时、分、秒，函数成员
包括设置日期时间和输出时间，其中年、月请用整型，并完成测试。
输出见样张。
注意：自己写的代码中不能有cout
-------------------------------------------------------*/
#include <iostream>
#include <iomanip>
using namespace std;


/**********Program**********/



/**********  End  **********/

void Timedate::list(){//成员函数,直接访问私有的数据成员
        cout<<"year/month/date :";
        switch(year){
        case 2000:cout<<"2000";break;
        case 2001:cout<<"2001";break;
        case 2002:cout<<"2002";break;
        case 2003:cout<<"2003";break;
        case 2004:cout<<"2004";break;
        case 2005:cout<<"2005";break;
        }
        switch(month){
        case 1:cout<<'/'<<"Jan";break;
        case 2:cout<<'/'<<"Feb";break;
        case 3:cout<<'/'<<"Mar";break;
        case 4:cout<<'/'<<"Apr";break;
        case 5:cout<<'/'<<"May";break;
        case 6:cout<<'/'<<"Jun";break;
        case 7:cout<<'/'<<"Jul";break;
        case 8:cout<<'/'<<"Aug";break;
        case 9:cout<<'/'<<"Sep";break;
        case 10:cout<<'/'<<"Oct";break;
        case 11:cout<<'/'<<"Nov";break;
        case 12:cout<<'/'<<"Dec";break;
        }
        cout<<'/'<<date<<endl;
        cout<<"hour:minite:second :";
        cout<<hh<<':'<<mm<<':'<<ss<<endl;
}


int main(){
        Timedate A(2004,3,3),B;
        A.list();  //输出A的日期和时间
        B.list();   //输出B的日期和时间
        B.putdate(2005,10,18);  //更新B的日期
        B.puttime(17,30,00);  //更新B的时间
        B.list();  //输出B的日期和时间
        return 0;
}

