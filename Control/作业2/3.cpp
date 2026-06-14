#include<cstdio>
#include<cstdlib>
#include"osqp.h"
using namespace std;
int main()
{
	int n,m;
	n=2;//变量 
	m=1;//约束条件
	OSQPInt P=2;//矩阵中非零个数 
	OSQPInt Pi[]={0,1};//每一列非零元素出现在的行数 
	OSQPInt Pj[]={0,1,2};
	//Pj[0]=0
	//Pj[i]=Pj[i-1]+第i-1列的非零元素个数 
	OSQPFloat Px[]={1.0,10.0};
	
	OSQPFloat q[]={-3.0,-30.0};
	
	OSQPInt A=2;
	OSQPInt Ai[]={0,0};
	OSQPInt Aj[]={0,1,2};
	OSQPFloat Ax[]={1.0,1.0};
	
	OSQPFloat l[]={-OSQP_INFTY};
	OSQPFloat u[]={4.0};
	//创建矩阵，前两个参数分别是：行数 列数  
	OSQPMatrix* P = osqp_sparse_matrix(n, n, P, Pi, Pj, Px);
	OSQPMatrix* A = osqp_sparse_matrix(m, n, A, Ai, Aj, Ax);
	//配置求解器的结构体 
	OSQPSettings settings;
	osqp_set_default_settings(&settings);
	settings.verbose=0;
	//初始化 
	OSQPData data;
	data.n=n;
	data.m=m;
	data.P=P;
	data.A=A;
	data.l=l;
	data.u=u;
	OSQPSolver* solver=osqp_setup(&data,&settings);
	osqp_solve(solver);
	printf("x=%.6lf y=%.6lf",solver->solution->x[0],solver->solution->x[1]);
	
	osqp_cleanup(solver);
	csc_free(P); csc_free(A);
	system("pause");
	return 0;
}
//#include <cstdio>
//#include <cstdlib>
//#include "osqp.h"
//
//int main()
//{
//    int n = 2;    // 优化变量数
//    int m = 1;    // 约束条数
//
//    // 二次项矩阵 P (2×2 对称矩阵)
//    OSQPInt P_nnz = 2;
//    OSQPInt Pi[] = {0, 1};
//    OSQPInt Pj[] = {0, 1, 2};
//    OSQPFloat Px[] = {1.0, 10.0};
//    csc* P = csc_matrix(n, n, P_nnz, Pi, Pj, Px);
//
//    // 一次项向量 q
//    OSQPFloat q[] = {-3.0, -30.0};
//
//    // 约束矩阵 A (1×2 行向量)
//    OSQPInt A_nnz = 2;
//    OSQPInt Ai[] = {0, 0};
//    OSQPInt Aj[] = {0, 1, 2};
//    OSQPFloat Ax[] = {1.0, 1.0};
//    csc* A = csc_matrix(m, n, A_nnz, Ai, Aj, Ax);
//
//    // 约束上下界
//    OSQPFloat l[] = {-OSQP_INFTY};
//    OSQPFloat u[] = {4.0};
//
//    // 求解器配置
//    OSQPSettings settings;
//    osqp_set_default_settings(&settings);
//    settings.verbose = 0;
//
//    // 问题数据结构体
//    OSQPData data;
//    data.n = n;
//    data.m = m;
//    data.P = P;
//    data.q = q;
//    data.A = A;
//    data.l = l;
//    data.u = u;
//
//    // 初始化并求解
//    OSQPSolver* solver = osqp_setup(&data, &settings);
//    osqp_solve(solver);
//
//    printf("x = %.6lf, y = %.6lf\n", 
//           solver->solution->x[0], 
//           solver->solution->x[1]);
//
//    // 释放内存
//    osqp_cleanup(solver);
//    csc_free(P);
//    csc_free(A);
//
//    system("pause");
//    return 0;
//}
