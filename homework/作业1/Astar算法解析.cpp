#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<cmath>
#include<queue>
#define N 6 // 地图规模
using namespace std;

class Node
{
	public:
		int x, y; // 	当前节点的xy坐标
		int F, G, H; // F为预计走到终点总代价 F=G+H
					 // G为从起点到xy坐标的已用代价
					 // H为从xy到终点的预估代价
		Node(int a, int b):x(a), y(b){}
		
		// 重载操作符，使优先队列以F值大小为标准维持堆，实现堆顶最小，c语言里默认为从大到小 
		bool operator < (const Node &a) const
		{
			return F > a.F;
		} 
}; 

// 八个方向，上下左右以及其斜角
int dir[8][2] = {{-1,-1}, {-1, 0}, {-1, 1}, {0, -1}, 
		 {0, 1},  {1, -1}, {1, 0},  {1, 1}};
// 优先队列实现从小到大排序
priority_queue<Node>que;
// 地图
int qp[N][N] = { {0,0,0,0,0,0},
		         {0,1,1,0,1,1},
		         {0,0,1,0,0,0},
	             {0,0,1,1,1,0},
		         {0,1,1,0,0,0},
		         {1,1,0,0,0,0} };
bool visit[N][N]; // 记录这个节点是否达到最小代价 
int valF[N][N];   // 当前节点的最小代价
int path[N][N][2]; // 记录该节点的父亲节点

int Manhuattan(int x, int y, int x1, int y1); // 算曼哈顿距离 用于H启发函数
bool NodeIsLegal(int x, int y, int xx, int yy); // 判断这个起点终点是否合法
void A_start(int x0, int y0, int x1, int y1); // 主体算法
void PrintPath(int x1, int y1); // 打印路径 

//运作流程：从当前最小代价的点出发，判断八个方向下的最小代价有无改变同时记录更新。
//重要变量：node当前节点信息,visit[][]节点是否找到最小代价,valF[][]节点最小代价
//与其他函数的联系：Manhuattan计算距离，NodeIsLegal判断合法性。
void A_start(int x0, int y0, int x1, int y1)
{
	//  初始化起点
	Node node(x0, y0);
	node.G = 0; 
	node.H = Manhuattan(x0, y0, x1, y1); 
	node.F = node.G + node.H;
	valF[x0][y0] = node.F; 
	// 存入队列
	que.push(node); 
	
	while(!que.empty())
	{
		Node node_top = que.top(); que.pop(); // 取出堆顶，此时取出来的一定是上次循环发展出来的最小代价节点，所以直接判断此时的代价F就是这个点的最小代价 
		visit[node_top.x][node_top.y] = true; // 该点已经找到最小代价 
		if(node_top.x == x1 && node_top.y == y1) // 判断是否到达终点
			break;
		
		for(int i=0; i<8; i++)//从八个方向依次出发
		{
			Node node_next(node_top.x + dir[i][0], node_top.y + dir[i][1]); // 用新的结构体来存储每个方向的新的坐标
			// 第一个条件判断起点终点的合法性，第二个条件判断这个点是否已经找到最小代价，找到了就不用找了
			if(NodeIsLegal(node_next.x, node_next.y, node_top.x, node_top.y) && !visit[node_next.x][node_next.y]) 
			{	
				// 实际代价为上一个点的代价+上个点走到这个点的直线距离 d=勾股定理
				node_next.G = node_top.G + int(sqrt(pow(dir[i][0],2)+pow(dir[i][1],2))*10); 
				// 这个点的预计距离为该点到终点的曼哈顿距离
				node_next.H = Manhuattan(node_next.x, node_next.y, x1, y1);  
				// F=G+H
				node_next.F = node_next.G + node_next.H; 
				
				// 第一个条件判断此时找到的路径是否由于之前找到的路径
				// 第二个条件判断如果这个点都没走过，那当前代价就是这个点目前的最小代价，因此记录。
				// 二者只要满足一个条件就要记录更新
				if(node_next.F < valF[node_next.x][node_next.y] || valF[node_next.x][node_next.y] == 0)
				{
					// 保存该节点的父节点 
					path[node_next.x][node_next.y][0] = node_top.x;
					path[node_next.x][node_next.y][1] = node_top.y;
					valF[node_next.x][node_next.y] = node_next.F; // 记录这个点的最小代价
					que.push(node_next); // 把这个节点放入队列
				}
			}
		}
	}
}
//打印路径
void PrintPath(int x1, int y1)
{
	if(path[x1][y1][0] == -1 || path[x1][y1][1] == -1)
	{
		cout<<"没有可行路径！"<<endl;
		return;
	}
	int x = x1, y = y1;
	int a, b; 
	while(x != -1 || y != -1)//通过记录的父亲节点一步一步回溯找到路径
	{
		qp[x][y] = 2; // 
		a = path[x][y][0];
		b = path[x][y][1];
		x = a;
		y = b;
	}
	// □表示未经过的节点， █表示障碍物， ☆表示可行节点 
	string s[3] = {"□", "█", "☆"};
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
			cout<<s[qp[i][j]];
		cout<<endl;
	}
}

int Manhuattan(int x, int y, int x1, int y1)
{
	return (abs(x - x1) + abs(y - y1)) * 10;
}

bool NodeIsLegal(int x, int y, int xx, int yy)
{
	if(x < 0 || x >= N || y < 0 || y >= N) return false; // 判断边界 
	if(qp[x][y] == 1) return false; // 判断障碍物 
	// 两节点成对角型且它们的公共相邻节点存在障碍物 
	if(x != xx && y != yy && (qp[x][yy] == 1 || qp[xx][y] == 1)) return false;
	return true;
}
int main()
{
	fill(visit[0], visit[0]+N*N, false); //    初始化 所有节点都没有找到最小路径
	fill(valF[0], valF[0]+N*N, 0); //          初始化 所有节点的代价都是0
	fill(path[0][0], path[0][0]+N*N*2, -1); // 初始化 所有节点的父亲节点都是-1 即没有人从这里出发
	
	//  // 起点 // 终点
	int x0, y0, x1, y1; 
	cout<<"输入起点：";
	cin>>x0>>y0;
	cout<<"输入终点：";
	cin>>x1>>y1;
	x0--; y0--; x1--; y1--;
	
	if(!NodeIsLegal(x0, y0, x0, y0))
	{
		cout<<"非法起点！"<<endl;
		return 0;	
	}
	
	A_start(x0, y0, x1, y1);  // A*算法 
	PrintPath(x1, y1);        // 打印路径 
}