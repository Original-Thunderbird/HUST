/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：

设计并测试一个矩形类（Rectangle），属性为矩形的左上与右下角的坐标，
矩形水平放置。操作为计算矩形周长与面积。

输出见样张


-------------------------------------------------------*/
#include <iostream>
#include <cmath>
using namespace std;


/**********Program**********/




/**********  End  **********/
int main(){
        Rectangle rect; 
        rect.Show();
        rect.Assign(100,200,300,400);
        rect.Show();
        Rectangle rect1(0,0,200,200);
        rect1.Show();
        Rectangle rect2(rect1);
        rect2.Show();
        cout<<"left-top point is ("<<rect.getLeft()<<","<<rect.getTop()<<")"<<'\n';
        cout<<"right-bottom point is ("<<rect.getRight()<<","<<rect.getBottom()<<")"<<'\n';
        cout<<"面积"<<rect.Area()<<'\t'<<"周长"<<rect.Perimeter()<<endl;
        return 0;
}

