#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define N 6

char Start_Map[N][N] = {
	{' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' '},
	{' ',' ','0','1',' ',' '},
	{' ',' ','1','0',' ',' '},
	{' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' '}
};

int weight[N][N] = {
	{10,1,5,5,1,10},
	{1,1,1,1,1,1},
	{5,1,1,1,1,5},
	{5,1,1,1,1,5},
	{1,1,1,1,1,1},
	{10,1,5,5,1,10}
};


struct Node
{
	char MAP[N][N];
	int H; //估计函数
	int turn; //turn为1，下一步走黑棋
	int alpha, beta;
	vector<int> next_i;
	vector<int> next_j;
	vector<Node*> child;
	int GameOver;

	Node(char M[N][N]=Start_Map, int t=1, int a=INT_MIN, int b=INT_MAX, int pos_i=-1, int pos_j=-1);

	void set_MAP(char M[N][N], int pos_i, int pos_j);

	void get_next_pos();

	int calc_H();

	void get_next_pos_with_ij(int i, int j);

	void move(int i, int j);

	void show_map();

	bool isGameOver();

	void get_chess_num(int & black, int & white);

};

#endif