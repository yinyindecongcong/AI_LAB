#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
using namespace std;

int nodes1 = 0;
int nodes2 = 0;

void print_N_queen(vector<int>& cols){
	for (unsigned int i = 0; i < cols.size(); i++){
		for (int j = 0; j < (int)cols.size(); j++){
			cout << (cols[i] == j) << ' ';
		}
		cout << endl;
	}
}

void print_N_queen2(vector<vector<int> >& V_cols){
	for (unsigned int i = 0; i < V_cols.size(); i++){
		for (unsigned int j = 0; j < V_cols.size(); j++){
			cout << (V_cols[i][j] == -1) << ' ';
		}
		cout << endl;
	}
}

bool satisfied(vector<int>& cols, int row, int col){
	for (int i = 0; i < row; i++)
		if (cols[i] == cols[row] || abs(cols[i] - cols[row]) == abs(i - row)) return 0;
	return 1;
}

int N_queen_BT(vector<int>& cols, int N, int row){
	nodes1++;
	static int sum = 0;
	if (row == N){if (sum++ == 0) print_N_queen(cols); return sum; }
	for (int i = 0; i < N; i++){
		cols[row] = i;
		if (satisfied(cols, row, i)) N_queen_BT(cols, N, row + 1);
	}
	return sum;
}

bool update(vector<vector<int> > & temp, int N, int row, int col){
	temp[row-1][col] = -1; //选中
	for (int i = row; i < N; i++){
		if (temp[i][col] == 0){
			temp[i][col] = row; 
			if (++temp[i][N] == N) return 0; 
		}
		int dis = abs(i - row + 1);
		if (col + dis < N && temp[i][col+dis] == 0){
			temp[i][col+dis] = row;
			if (++temp[i][N] == N) return 0;
		}
		if (col - dis >= 0 && temp[i][col-dis] == 0){
			temp[i][col-dis] = row;
			if (++temp[i][N] == N) return 0;
		}
	}
	return 1;
}

void roll_back(vector<vector<int> > & temp, int N, int row, int col){
	temp[row-1][col] = 0; //恢复
	for (int i = row; i < N; i++){
		if (temp[i][col] == row){
			temp[i][col] = 0; 
			--temp[i][N];
		}
		int dis = abs(i - row + 1);
		if (col + dis < N && temp[i][col+dis] == row){
			temp[i][col+dis] = 0;
			--temp[i][N];
		}
		if (col - dis >= 0 && temp[i][col-dis] == row){
			temp[i][col-dis] = 0;
			--temp[i][N];
		}
	}
}


int N_queen_FC(vector<vector<int> >& V_cols, int N, int row){
	nodes2++;
	static int sum2 = 0;
	if (row == N){if (sum2++ == 0) print_N_queen2(V_cols); return sum2; }
	for (int i = 0; i < N; i++){
		if (V_cols[row][i] == 0){
			if (update(V_cols, N, row+1, i) == 1)
				N_queen_FC(V_cols, N, row + 1);
			roll_back(V_cols, N, row+1, i);
		}
	}
	return sum2;
}

int main(){
	int N;
	cout << "input size of N-queen：";
	cin >> N;
	
	vector<int> cols(N, -1); //每一行走的列数
	clock_t start = clock();
	int ans = N_queen_BT(cols, N, 0);
	clock_t end = clock();
	cout << "there are " << ans << " solutions of " << N << "-queen" << endl;
	cout << "search nodes: " << nodes1 << endl;
	cout << "BT using time: " << (end - start) * 1000.0 / CLOCKS_PER_SEC << "ms"<< endl;
	
	vector<vector<int> >V_cols(N, vector<int>(N+1,0));//0表示可走，最后一列表示不可走的路
	clock_t start2 = clock();
	int ans2 = N_queen_FC(V_cols, N, 0);
	clock_t end2 = clock();
	cout << "there are " << ans2 << " solutions of " << N << "-queen" << endl;
	cout << "search nodes: " << nodes2 << endl;
	cout << "FC using time: " << (end2 - start2) * 1000.0 / CLOCKS_PER_SEC << "ms"<< endl;
}