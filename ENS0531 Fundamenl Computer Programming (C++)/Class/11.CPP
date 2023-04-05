/*-------------------------------------------------------
【程序设计】
---------------------------------------------------------

题目：定义盒子Box类，要求具有以下成员：长、宽、高分别为(int类型)x,y,z，

可计算盒子体积；可计算盒子的表面积;可求最大边长。

输出结果见图：样张.JPG
-------------------------------------------------------*/
#include <iostream>
using namespace std;

/**********Program**********/


/**********  End  **********/
 void main()
{
        Box mybox(1,2,3),mybox1;
        //输出盒子的体积
        cout << "This box's volume=" << mybox.box_volume() << endl;
        //输出盒子的表面积
        cout << "This box's area=" << mybox.box_area() << endl;
        cout<<"Max="<<mybox.Max ()<<endl;
            mybox1.SetBox(2,2,2);
       //输出盒子的体积
        cout << "This box's volume=" << mybox1.box_volume() << endl;
        //输出盒子的表面积
        cout << "This box's area=" << mybox1.box_area() << endl;
        cout<<"Max="<<mybox1.Max ()<<endl;               

}
