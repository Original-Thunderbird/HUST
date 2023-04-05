/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：

设计一个学校在册人员类（Person）。数据成员包括：身份证号（IdPerson），
姓名（Name），性别（Sex），生日（Birthday）和家庭住址（HomeAddress）。
成员函数包括人员信息的录入和显示。还包括构造函数与拷贝构造函数。
设计一个合适的初始值。
运行结果见样张。
-------------------------------------------------------*/

#include<iostream>
#include<cstring>
using namespace std;

class Person{
        char IdPerson[19];                        //身份证号,18位数字
        char Name[20];                                //姓名
        char Sex;                                //性别
        int Birthday;                                //生日,格式1986年8月18日写作19860818
        char HomeAddress[50];                //家庭地址
public:
        Person(char a[19],char b[20],char c,int d,char e[50]);
        Person(Person &);
        Person();
        ~Person();
       void PrintPersonInfo(); //输出人员信息
        void inputPerson();  //输入人员信息
       
};

/**********Program**********/


/**********  End  **********/
void Person::inputPerson(){
        
        cout<<"请输入身份证号,18位数字："<<endl;
        cin.getline(IdPerson,19);
        cout<<"请输入姓名："<<endl;
        cin.getline(Name,20);
        cout<<"请输入性别m或w："<<endl;
        cin>>Sex;
        cout<<"请输入生日,格式1986年8月18日写作19860818："<<endl;
        cin>>Birthday;
        cin.get();   //吸收回车符,否则地址输不进去
        cout<<"请输入地址："<<endl;
        cin.getline(HomeAddress,50);
}
void Person::PrintPersonInfo(){
        int i;
        cout<<"身份证号:"<<IdPerson<<'\n'<<"姓名:"<<Name<<'\n'<<"性别:";
        if(Sex=='m' ||Sex=='M')cout<<"男"<<'\n';
        else if(Sex=='w' ||Sex=='M')cout<<"女"<<'\n';
             else cout<<" "<<'\n';
        cout<<"出生年月日:";
        i=Birthday;
        cout<<i/10000<<"年";
        i=i%10000;
        cout<<i/100<<"月"<<i%100<<"日"<<'\n'<<"家庭住址:"<<HomeAddress<<'\n';
}

int main(){
Person  Ps1("320102811226161","朱海鹏",'m',19811226,"南京市黄浦路1号"),
Ps2(Ps1),Ps3;
        Ps1.PrintPersonInfo();
        Ps2.PrintPersonInfo();
        Ps3.inputPerson();
        Ps3.PrintPersonInfo();
        return 0;
}
