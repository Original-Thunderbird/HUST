#include < iostream >
using namespace std ;
int main ()
{
	int m= 0 , vg= 3 , vr= 9 , sg , sr , s , station = 0 , sleep = 0  , run = 0 ;
	cout << "请输入跑道长度" ;
	cin >> s ; 
	while ( sg < s && sr < s )
	{
		if ( station == 1 )
		{
			sleep ++ ;
			m ++ ;
			sg += vg ;
			if ( sleep == 30 )
			{
				station = 0 ;
				sleep = 0 ;
			}
		}
		else
		{
			m ++ ;
			run ++ ;
			sg += vg ;
			sr += vr ;
			if ( run == 10 )
			{
				run = 0 ;
				if (sr > sg )
					station = 1 ;
			}
		}
	}
	if ( sr < sg )
		cout << "兔子胜" << endl ;
    else if ( sr > sg )
		cout << "乌龟胜" << endl ;
	else 
		cout << "平局" << endl ;
	return 0 ;
}



