#include<cstdio>
#include<Eigen/Dense>
using namespace Eigen;
using namespace std;
void diedai(double n)
{
	double dis=1e-3; 
    Vector2d t(3.0,3.0);
    Vector2d x(0.0,0.0);
    int time=0;
    while(true)
    {
        if((x-t).norm()<dis)break;
        Vector2d grad;
        grad(0)=x(0)-3.0;
        grad(1)=10*(x(1)-3.0);
        x=x-n*grad;
        time++;
    }
    printf("学习率:%.2lf   ",n); 
    printf("迭代次数:%d     %.6lf %.6lf\n",time,x(0),x(1));
}
int main()
{
	for(int i=1;i<=20;i++)
	{
		diedai(i*0.05);
	}
}
