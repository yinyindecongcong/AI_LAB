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
struct Node
{
	char MAP[N][N];
	int H; //���ƺ���
	int turn; //turnΪ1����һ���ߺ���
	int alpha, beta;
	vector<int> next_i;
	vector<int> next_j;
	vector<Node*> child;

	Node(char M[N][N]=Start_Map, int t=1, int a=INT_MIN, int b=INT_MAX, int pos_i=-1, int pos_j=-1);

	void set_MAP(char M[N][N]);

	void get_next_pos();

	int calc_H();

	void get_next_pos_with_ij(int i, int j);

	void move(int i, int j);

	void show_map();

	bool isGameOver();

	void get_chess_num(int & black, int & white);
};

#endif