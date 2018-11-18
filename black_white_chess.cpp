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
	int H; //估计函数
	int turn; //turn为1，下一步走黑棋
	int alpha, beta;
	vector<int> next_i;
	vector<int> next_j;
	vector<Node*> child;

	Node(char M[N][N]=Start_Map, int t=1, int a=INT_MIN, int b=INT_MAX, int pos_i=-1, int pos_j=-1)
	 :turn(t),alpha(a),beta(b){
		set_MAP(M);
		if (pos_i != -1) move(pos_i, pos_j);
		get_next_pos();
		H = calc_H();
		child = vector<Node*>(next_i.size(), NULL);
	}

	void set_MAP(char M[N][N]){
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				this->MAP[i][j] = M[i][j] == '*'? ' ': M[i][j];
	}

	void get_next_pos(){
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (this->MAP[i][j] - '0' == this->turn) get_next_pos_with_ij(i, j);
	}

	int calc_H(){
		int white = 0, black = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (MAP[i][j] == '0') white++;
				else if (MAP[i][j] == '1') black++;
		if (next_i.size() == 0) 
			return (white > black? INT_MIN: INT_MAX);
		return black - white;
	}

	void get_next_pos_with_ij(int i, int j){
		//水平方向
		if (j < N - 1 && MAP[i][j+1] - '0' == 1 - turn){
			int k = j + 2;
			while (k < N && MAP[i][k] - '0' == 1 - turn) k++;
			if (k < N && MAP[i][k] == ' ')
				{MAP[i][k] = '*'; next_i.push_back(i); next_j.push_back(k);}
		}
		if (j > 1 && MAP[i][j-1] - '0' == 1 - turn){
			int k = j - 2;
			while (k >= 0 && MAP[i][k] - '0' == 1 - turn) k--;
			if (k >= 0 && MAP[i][k] == ' ')
				{MAP[i][k] = '*'; next_i.push_back(i); next_j.push_back(k);}
		}
		//竖直方向
		if (i < N - 1 && MAP[i+1][j] - '0' == 1 - turn){
			int k = i + 2;
			while (k < N && MAP[k][j] - '0' == 1 - turn) k++;
			if (k < N && MAP[k][j] == ' ') 
				{MAP[k][j] = '*'; next_i.push_back(k); next_j.push_back(j);}
		}
		if (i > 1 && MAP[i-1][j] - '0' == 1 - turn){
			int k = i - 2;
			while (k >= 0 && MAP[k][j] - '0' == 1 - turn) k--;
			if (k >= 0 && MAP[k][j] == ' ') 
				{MAP[k][j] = '*'; next_i.push_back(k); next_j.push_back(j);}
		}
		//右下对角线方向
		if (j < N - 1 && i < N - 1 && MAP[i+1][j+1] - '0' == 1 - turn){
			int k = 2;
			while (j + k < N && i + k < N && MAP[i+k][j+k] - '0' == 1 - turn) k++;
			if (j + k < N && i + k < N && MAP[i+k][j+k] == ' ')
				{MAP[i+k][j+k] = '*'; next_i.push_back(i+k); next_j.push_back(j+k);}
		}
		if (j > 1 && i > 1 && MAP[i-1][j-1] - '0' == 1 - turn){
			int k = 2;
			while (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] - '0' == 1 - turn) k++;
			if (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] == ' ')
				{MAP[i-k][j-k] = '*'; next_i.push_back(i-k); next_j.push_back(j-k);}
		}
		//左下对角线方向
		if (i < N - 1 && j > 1 && MAP[i+1][j-1] - '0' == 1 - turn){
			int k = 2;
			while(i+k < N && j - k >= 0 && MAP[i+k][j-k] - '0' == 1 - turn) k++;
			if (i+k < N && j - k >= 0 && MAP[i+k][j-k] == ' ')
				{MAP[i+k][j-k] = '*'; next_i.push_back(i+k); next_j.push_back(j-k);}
		}
		if (j < N - 1 && i > 1 && MAP[i-1][j+1] - '0' == 1 - turn){
			int k = 2;
			while(j+k < N && i - k >= 0 && MAP[i-k][j+k] - '0' == 1 - turn) k++;
			if (j+k < N && i - k >= 0 && MAP[i-k][j+k] == ' ')
				{MAP[i-k][j+k] = '*'; next_i.push_back(i-k); next_j.push_back(j+k);}
		}
	}

	void move(int i, int j){
		MAP[i][j] = '0' + 1 - turn;
		int k;
		//水平方向
		k = j + 1;
		while(k < N && MAP[i][k] - '0' == turn) k++;
		if (k < N && MAP[i][k] - '0' == 1 - turn) 
			for (int w = j + 1; w < k; w++) MAP[i][w] = '0' + 1 - turn;
		k = j - 1;
		while(k >= 0 && MAP[i][k] - '0' == turn) k--;
		if (k >= 0 && MAP[i][k] - '0' == 1 - turn) 
			for (int w = j - 1; w > k; w--) MAP[i][w] = '0' + 1 - turn;
		//竖直方向
		k = i + 1;
		while (k < N && MAP[k][j] - '0' == turn) k++;
		if (k < N && MAP[k][j] - '0' == 1 - turn)
			for (int w = i + 1; w < k; w++) MAP[w][j] = '0' + 1 - turn;
		k = i - 1;
		while (k >= 0 && MAP[k][j] - '0' == turn) k--;
		if (k >= 0 && MAP[k][j] - '0' == 1 - turn)
			for (int w = i - 1; w > k; w--) MAP[w][j] = '0' + 1 - turn;
		//右下对角线方向
		k = 1;
		while (j + k < N && i + k < N && MAP[i+k][j+k] - '0' == turn) k++;
		if (j + k < N && i + k < N && MAP[i+k][j+k] - '0' == 1 - turn)
			for (int w = 1; w < k; w++) MAP[i+w][j+w] = '0' + 1 - turn;
		k = 1;
		while (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] - '0' == turn) k++;
		if (j - k < N && i - k < N && MAP[i-k][j-k] - '0' == 1 - turn)
			for (int w = 1; w < k; w++) MAP[i-w][j-w] = '0' + 1 - turn;
		//左下对角线方向
		k = 1;
		while(i+k < N && j - k >= 0 && MAP[i+k][j-k] - '0' == turn) k++;
		if (i+k < N && j - k >= 0 && MAP[i+k][j-k] - '0' == 1 - turn)
			for (int w = 1; w < k; w++) MAP[i+w][j-w] = '0' + 1 - turn;
		k = 1;
		while(j+k < N && i - k >= 0 && MAP[i-k][j+k] - '0' == turn) k++;
		if (i-k < N && j + k >= 0 && MAP[i-k][j+k] - '0' == 1 - turn)
			for (int w = 1; w < k; w++) MAP[i-w][j+w] = '0' + 1 - turn;
	}

	void show_map(){
		for (int i = 0; i < N; i++){
			cout << "           ";
			for (int k = 0; k < 4 * N; k++) cout << '-';
			cout << "\n           |";
			for (int j = 0; j < N; j++){
				cout << ' ' << this->MAP[i][j] << " |";
			}
			cout << endl;
		}
	}

	bool isGameOver(){
		return next_i.empty();
	}

	void get_chess_num(int & black, int & white){
		black = white = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (MAP[i][j] == '1') black++;
				else if (MAP[i][j] == '0') white++;
	}

};

void make_tree_with_depth(Node* start, int& next_move, int turn=1, int length=0, int depth=2){
	//turn为1表示下一步走黑棋
	if (length >= depth || start->isGameOver() || start->beta <= start->alpha){
		start->alpha = max(start->alpha, start->H);
		start->beta = min(start->beta, start->H);
		return;
	}
	for (unsigned int i = 0; i < start->next_i.size(); i++){
		int next_x = start->next_i[i];
		int next_y = start->next_j[i];
		Node * next = new Node(start->MAP, 1-turn, start->alpha, start->beta, next_x, next_y);
		start->child[i] = next;
		make_tree_with_depth(next, next_move, 1-turn, length+1, depth);
		if (turn && next->beta > start->alpha) 
			{start->alpha = next->beta; next_move = i;}
		else if (!turn && next->alpha < start->beta)
			{start->beta = next->alpha; next_move = i;}
	}
}

void show_tree(Node * start){
	int count = 1, depth = 0, sum = 0;
	queue<Node*> Q;
	Q.push(start);
	cout << "           ******当前深度：" << depth << "********" << endl;
	while(!Q.empty()){
		Node * front = Q.front();
		Q.pop();
		sum++;
		front->show_map();
		cout << "           H,alpha,beta=" << front->H << ',' << front->alpha << ',' << front->beta << endl;
		for (unsigned int i = 0; i < front->child.size(); i++)
			if (front->child[i] == NULL) break;
			else Q.push(front->child[i]); 
		if (--count == 0){
			cout << "           ******当前深度：" << ++depth << "********" << endl;
			count = Q.size();
		}
	}
	cout << "           ******节点数：" << sum << "********" << endl;
	cout << "           H,alpha,beta=" << start->H << ',' << start->alpha << ',' << start->beta << endl;
}

void Game_start(){
	Node *A = new Node();
	Node *temp = NULL;
	int pos_i = -1, pos_j = -1;
	int black, white;
	int player = 1; //表示玩家为黑方
	
	while(!A->isGameOver()){
		A->get_chess_num(black, white);
		cout << "              ==================" << endl;
		cout << "              当前状态"<< "(轮到" << (A->turn? "黑":"白") << "方)" << endl;
		cout << "               黑子：" << black << " 白子：" << white << endl;
		cout << "              ==================" << endl;
		A->show_map();
		cout << " 可走的棋为：";
		for (unsigned int i = 0; i < A->next_i.size(); i++)
			cout << '(' << A->next_i[i] << ',' << A->next_j[i] << ") ";
		//玩家顺序
		if (A->turn == player){
			cout << " 请输入落子的坐标(例: "<< A->next_i[0] << ' ' << A->next_j[0] << ")：";
			cin >> pos_i >> pos_j;
			while(pos_i >= N || pos_i < 0 || pos_j >= N || pos_j < 0 || A->MAP[pos_i][pos_j] != '*'){
				cin.clear();
				cin.sync();
				cout << "\n 不是合法的坐标，请重新输入(例: "<< A->next_i[0] << ' ' << A->next_j[0] << ")：";
				cin >> pos_i >> pos_j;
			}
		}
		//电脑顺序
		else{
			int next_move = -1;
			cout << "\n 电脑计算中..." << endl;
			make_tree_with_depth(A, next_move, A->turn, 0, 7);
			pos_i = A->next_i[next_move];
			pos_j = A->next_j[next_move];
			cout << " 电脑计算结果：选择走(" << pos_i << ',' << pos_j << ")\n";
			cout << "           H,alpha,beta=" << A->H << ',' << A->alpha << ',' << A->beta << endl;
		}
		temp = A;
		A = new Node(A->MAP, 1 - A->turn, INT_MIN, INT_MAX, pos_i, pos_j);
		delete temp;
		temp = NULL;
	}
	A->get_chess_num(black, white);
	cout << "              ==================" << endl;
	cout << "                   结束状态" << endl;
	cout << "               黑子：" << black << " 白子：" << white << endl;
	cout << "              ==================" << endl;
	A->show_map();
}

int main(){
	Game_start();
}