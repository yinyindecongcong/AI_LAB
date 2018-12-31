#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <string>
using namespace std;

#define N 8


int Start_Map[N][N] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,-1,1,0,0,0},
	{0,0,0,1,-1,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};

/*
int Start_Map[N][N] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0},
	{0,0,0,1,1,1,0,0},
	{0,-1,-1,-1,1,1,1,0},
	{0,0,0,-1,0,1,0,0},
	{0,0,1,-1,-1,-1,-1,-1},
	{0,0,0,0,0,0,0,0}
};
*/

int weight[N][N] = {
	{4,1,1,1,1,1,1,4},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{4,1,1,1,1,1,1,4}
};
const int STABLE_WEIGHT = 10;
string tochar(int a){
	if (a == 1) return "●";
	if (a == -1) return "○";
	if (a == 0) return "×";
	return "  ";
}

struct State
{
	int MAP[N][N];
	int turn; //turn为1表示轮到黑方
	vector<int> next_i;
	vector<int> next_j;

	State(int M[N][N]=Start_Map, int t=1,bool show_pos=true):turn(t){
		set_MAP(M);
		get_next_pos(show_pos);
	}

	State(State& s):next_i(s.next_i), next_j(s.next_j){
		set_MAP(s.MAP);
		turn = s.turn;

	}

	void set_MAP(int M[N][N]){
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				this->MAP[i][j] = M[i][j] == 2? 0: M[i][j];
	}

	void get_next_pos(bool show_pos=true){
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (this->MAP[i][j] == this->turn) get_next_pos_with_ij(i, j, show_pos);
	}

	void get_next_pos_with_ij(int i, int j, bool show_pos){
		//水平方向
		if (j < N - 2 && MAP[i][j+1] == -turn){
			int k = j + 2;
			while (k < N && MAP[i][k] == -turn) k++;
			if (k < N && MAP[i][k] == 0){
				if (show_pos == true) MAP[i][k] = 2; 
				next_i.push_back(i); next_j.push_back(k);}
		}
		if (j > 1 && MAP[i][j-1] == -turn){
			int k = j - 2;
			while (k >= 0 && MAP[i][k] == -turn) k--;
			if (k >= 0 && MAP[i][k] == 0){
				if (show_pos == true) MAP[i][k] = 2;
				next_i.push_back(i); next_j.push_back(k);}
		}
		//竖直方向
		if (i < N - 2 && MAP[i+1][j] == -turn){
			int k = i + 2;
			while (k < N && MAP[k][j] == -turn) k++;
			if (k < N && MAP[k][j] == 0) {
				if (show_pos == true) MAP[k][j] = 2; 
				next_i.push_back(k); next_j.push_back(j);}
		}
		if (i > 1 && MAP[i-1][j] == -turn){
			int k = i - 2;
			while (k >= 0 && MAP[k][j] == -turn) k--;
			if (k >= 0 && MAP[k][j] == 0) {
				if (show_pos == true) MAP[k][j] = 2; 
				next_i.push_back(k); next_j.push_back(j);}
		}
		//右下对角线
		if (j < N - 2 && i < N - 2 && MAP[i+1][j+1] == -turn){
			int k = 2;
			while (j + k < N && i + k < N && MAP[i+k][j+k] == -turn) k++;
			if (j + k < N && i + k < N && MAP[i+k][j+k] == 0){
				if (show_pos == true) MAP[i+k][j+k] = 2; 
				next_i.push_back(i+k); next_j.push_back(j+k);}
		}
		if (j > 1 && i > 1 && MAP[i-1][j-1] == -turn){
			int k = 2;
			while (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] == -turn) k++;
			if (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] == 0){
				if (show_pos == true) MAP[i-k][j-k] = 2; 
				next_i.push_back(i-k); next_j.push_back(j-k);}
		}
		//左下对角线
		if (i < N - 2 && j > 1 && MAP[i+1][j-1] == -turn){
			int k = 2;
			while(i+k < N && j - k >= 0 && MAP[i+k][j-k] == -turn) k++;
			if (i+k < N && j - k >= 0 && MAP[i+k][j-k] == 0){
				if (show_pos == true) MAP[i+k][j-k] = 2; 
				next_i.push_back(i+k); next_j.push_back(j-k);}
		}
		if (j < N - 2 && i > 1 && MAP[i-1][j+1] == -turn){
			int k = 2;
			while(j+k < N && i - k >= 0 && MAP[i-k][j+k] == -turn) k++;
			if (j+k < N && i - k >= 0 && MAP[i-k][j+k] == 0){
				if (show_pos == true) MAP[i-k][j+k] = 2; 
				next_i.push_back(i-k); next_j.push_back(j+k);}
		}
	}

	void move(int i, int j){
		MAP[i][j] = turn;
		int k;
		//水平方向
		k = j + 1;
		while(k < N && MAP[i][k] == -turn) k++;
		if (k < N && MAP[i][k] == turn) 
			for (int w = j + 1; w < k; w++) MAP[i][w] = turn;
		k = j - 1;
		while(k >= 0 && MAP[i][k] == -turn) k--;
		if (k >= 0 && MAP[i][k] == turn) 
			for (int w = j - 1; w > k; w--) MAP[i][w] = turn;
		//竖直方向
		k = i + 1;
		while (k < N && MAP[k][j] == -turn) k++;
		if (k < N && MAP[k][j] == turn)
			for (int w = i + 1; w < k; w++) MAP[w][j] = turn;
		k = i - 1;
		while (k >= 0 && MAP[k][j] == -turn) k--;
		if (k >= 0 && MAP[k][j] == turn)
			for (int w = i - 1; w > k; w--) MAP[w][j] = turn;
		//右下对角线
		k = 1;
		while (j + k < N && i + k < N && MAP[i+k][j+k] == -turn) k++;
		if (j + k < N && i + k < N && MAP[i+k][j+k] == turn)
			for (int w = 1; w < k; w++) MAP[i+w][j+w] = turn;
		k = 1;
		while (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] == -turn) k++;
		if (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] == turn)
			for (int w = 1; w < k; w++) MAP[i-w][j-w] = turn;
		//左下对角线
		k = 1;
		while(i+k < N && j - k >= 0 && MAP[i+k][j-k] == -turn) k++;
		if (i+k < N && j - k >= 0 && MAP[i+k][j-k] == turn)
			for (int w = 1; w < k; w++) MAP[i+w][j-w] = turn;
		k = 1;
		while(j+k < N && i - k >= 0 && MAP[i-k][j+k] == -turn) k++;
		if (j+k < N && i - k >= 0 && MAP[i-k][j+k] == turn)
			for (int w = 1; w < k; w++) MAP[i-w][j+w] = turn;
	}

	void show_map(){
		
		cout << "            ";
		for (int i = 0; i < N; i++) cout << i << "  ";
		for (int i = 0; i < N; i++){
			cout << "\n          " << i;
			for (int j = 0; j < N; j++){
				cout << ' ' << tochar(MAP[i][j]);
			}
		}
		cout << endl;
	}

	void get_chess_num(int & black, int & white){
		black = white = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (MAP[i][j] == 1) black++;
				else if (MAP[i][j] == -1) white++;
	}

	bool isTerminal(){
		bool end = false;
		if (next_i.size() == 0){
			set_MAP(MAP);
			turn = -turn;
			get_next_pos(false);
			if (next_i.size() == 0) end = true;
		}
		return end;
	}
};

/*蒙特卡洛搜索的节点*/
struct Node{
	State state;
	double visit_times; //访问次数
	double quality_value; //评分
	Node * parent;
	vector<Node *> children;
	vector<int> not_used_pos_idx; //未探索的子节点下标列表
	int chess_num;

	Node(State s, Node * p=NULL):visit_times(0.0), quality_value(0.0){
		state = s;
		parent = p;
		chess_num = 0;
		for(unsigned int i = 0; i < state.next_i.size(); i++)
			not_used_pos_idx.push_back(i);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (state.MAP[i][j] == 1 || state.MAP[i][j] == -1) chess_num++;
		children = vector<Node*>(s.next_i.size(), NULL);
	}

	bool isFullyExpand(){
		return not_used_pos_idx.size() == 0;
	}
};


//选择新的Action
Node * Expand(Node * node){
	int choose_idx = rand() % node->not_used_pos_idx.size();
	int pos_i = node->state.next_i[node->not_used_pos_idx[choose_idx]];
	int pos_j = node->state.next_j[node->not_used_pos_idx[choose_idx]];
	State * temp_state = new State(node->state); 
	temp_state->move(pos_i, pos_j); //更新棋盘
	State * sub_state = new State(temp_state->MAP, -temp_state->turn); //创建棋盘
	Node * sub_node = new Node(*sub_state, node);//创建节点
	node->children[node->not_used_pos_idx[choose_idx]] = sub_node;
	node->not_used_pos_idx.erase(node->not_used_pos_idx.begin() + choose_idx);//更新
	return sub_node;
}

//选择UCB值最大的节点，若isSearching=0
Node * get_best_node(Node * node, int & idx, bool isSearching=1){
	Node * best_sub_node = NULL;
	double max_score = -10000000;
	double C = 1.414 * isSearching;
	for (unsigned int i = 0; i < node->children.size(); i++){
		double score = node->children[i]->quality_value / node->children[i]->visit_times;
		if (C != 0) score += C * sqrt(2 * log(node->visit_times) / node->children[i]->visit_times);
		if (score > max_score){
			max_score = score;
			best_sub_node = node->children[i];
			idx = i;
		}
	}
	return best_sub_node;
}

//选择最值得探索的节点
Node * Selection(Node * node){
	int tmp;
	while (node->not_used_pos_idx.size() || node->children.size()){
		if (node->isFullyExpand()) node = get_best_node(node, tmp);
		else return Expand(node);
	}
	return node;
}

int evaluate(State& s, int com_turn){
	int turn1 = 0, turn2 = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (s.MAP[i][j] == com_turn) turn1 += 1;
			else if (s.MAP[i][j] == -com_turn) turn2 += 1;
	return (turn1 > turn2? 20: -20);
}

bool stable(State * s, int i, int j){
	int h = 2, v = 2, d1 = 2, d2 = 2;
	for (int k = j - 1; k >= 0; k--) if (s->MAP[i][k] != s->turn) {h--; break;}
	for (int k = j + 1; k < N; k++) if (s->MAP[i][k] != s->turn) {h--; break;}
	for (int k = i - 1; k >= 0; k--) if (s->MAP[k][j] != s->turn) {v--; break;}
	for (int k = i + 1; k < N; k++) if (s->MAP[k][j] != s->turn) {v--; break;}
	for (int k = 1; i - k >= 0 && j - k >= 0; k++) 
		if (s->MAP[i-k][j-k] != s->turn) {d1--; break;}
	for (int k = 1; i + k < N && j + k < N; k++) 
		if (s->MAP[i+k][j+k] != s->turn) {d1--; break;}
	for (int k = 1; i - k >= 0 && j + k < N; k++) 
		if (s->MAP[i-k][j+k] != s->turn) {d2--; break;}
	for (int k = 1; i + k < N && j - k >= 0; k++) 
		if (s->MAP[i+k][j-k] != s->turn) {d2--; break;}
	return h && v && d1 && d2;
}
//模拟至游戏结束，得到当前节点的收益
int Simulation(Node * node, int com_turn){
	double p = 0;
	double gamma = 0.9;
	State * temp_state = new State(node->state);
	int count = 0;
	while (!(node->chess_num + count == 64 ||temp_state->isTerminal())) {
		int choose_idx = rand() % temp_state->next_i.size();
		int pos_i = temp_state->next_i[choose_idx];
		int pos_j = temp_state->next_j[choose_idx];
		temp_state->move(pos_i, pos_j);
		/*
		if (stable(temp_state, pos_i, pos_j)){ //是否是稳定子 
				p += (temp_state->turn == com_turn? 64-node->chess_num-count: node->chess_num + count - 64) * STABLE_WEIGHT * weight[pos_i][pos_j];
		}
		*/
		State * tmp = temp_state;
		temp_state = new State(temp_state->MAP, -temp_state->turn);
		/*
		if (node->chess_num + count < 35){
			int me = 0, op = 0;
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					if (temp_state->MAP[i][j] == com_turn) me++;
					else if (temp_state->MAP[i][j] == -com_turn) op++;
			p += gamma * (op - me);
		}
		*/
		delete tmp;
		gamma *= gamma;
		count ++;
	}
	int black, white;
	temp_state->get_chess_num(black, white);
	//int score = evaluate(*temp_state, com_turn) + p;
	delete temp_state;
	if (com_turn == 1)return (black > white? 1: -1);
	if (com_turn == -1)return (black > white? -1: 1);
	//return score;
}

void Backpropagation(Node * node, int reward, int com_turn){
	while (node != NULL){
		node->visit_times ++;
		node->quality_value += reward;
		// if (node->state.turn == com_turn) node->quality_value += reward;
		// else node->quality_value -= reward;
		node = node->parent;
	}
}

void MCTS(State& s, int& pos_i, int &pos_j, int com_turn){
	clock_t start = clock();
	Node * cur_node = new Node(s);
	//int budget = 10;
	while ((clock() - start) / CLOCKS_PER_SEC < 0.1){
	//while (budget--){
		Node * sub_node = Selection(cur_node);
		int reward = Simulation(sub_node, com_turn);
		Backpropagation(sub_node, reward, com_turn);
	}
	int idx;
	clock_t end = clock();
	for (unsigned int i = 0; i < cur_node->children.size(); i++){
    	cout << cur_node->children[i]->visit_times << ' ' << cur_node->children[i]->quality_value << " >>>(" << cur_node->state.next_i[i] << "," << cur_node->state.next_j[i] << ")\n";
    }
    cout << "total cost: " << (end - start) * 1.0 / CLOCKS_PER_SEC << endl;
    cout << "epoch: " << cur_node->visit_times << endl;
	get_best_node(cur_node, idx, 0);
	pos_i = cur_node->state.next_i[idx];
	pos_j = cur_node->state.next_j[idx];
}

void Game_start(){
	State *A = new State();
	State *temp = NULL;
	int pos_i = -1, pos_j = -1;
	int black, white;
	int player = 1; //是否为黑方
	int No_way = 0;
	char choose;
	cout << "              ==================" << endl;
	cout << "             黑白棋游戏(黑方先手)" << endl;
	cout << "              ==================" << endl;
	cout << "             请选择是否先手(Y/N):";
	cin >> choose;
	cin.clear();
	cin.sync();
	if (choose != 'Y' && choose != 'y') player = -1; 
	while(1){
		A->get_chess_num(black, white);
		cout << "              ==================" << endl;
		cout << "              当前状态"<< "(轮到" << (A->turn==1? "黑":"白") << "方)" << endl;
		cout << "               黑子：" << black << " 白子：" << white << endl;
		cout << "              ==================" << endl;
		A->show_map();
		if (black + white == 64) break;
		if (A->next_i.size() == 0){
			cout << " 当前方没有可走的棋，跳过当前轮" << endl;
			if (++No_way == 2){
				cout << "              ==================" << endl;
				cout << "              双方均没有可走的棋，结束" << endl;
				cout << "              ==================" << endl;
				break;
			}
		}
		else{
			No_way = 0;
			cout << " 可走的棋为：";
			for (unsigned int i = 0; i < A->next_i.size(); i++)
				cout << '(' << A->next_i[i] << ',' << A->next_j[i] << ") ";
			//玩家顺序
			if (A->turn == player){
				cout << " 请输入落子的坐标(例: "<< A->next_i[0] << ' ' << A->next_j[0] << ")：";
				cin >> pos_i >> pos_j;
				while(pos_i >= N || pos_i < 0 || pos_j >= N || pos_j < 0 || A->MAP[pos_i][pos_j] != 2){
					cin.clear();
					cin.sync();
					cout << "\n 不是合法的坐标，请重新输入(例: "<< A->next_i[0] << ' ' << A->next_j[0] << ")：";
					cin >> pos_i >> pos_j;
				}
				cin.clear();
				cin.sync();
			}
			//电脑顺序
			else{
				cout << "\n 电脑计算中..." << endl;
				/*
				int next_move = -1;
				next_move = 0;
				pos_i = A->next_i[next_move];
				pos_j = A->next_j[next_move];
				*/
				MCTS(*A, pos_i, pos_j, -player);
				cout << " 电脑计算结果：选择走(" << pos_i << ',' << pos_j << ")\n";
			}
			A->move(pos_i, pos_j);
		}
		temp = A;
		A = new State(A->MAP, -A->turn);
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
	State * A = new State();
	A->move(7,5);
	cout << stable(A, 7, 5) << endl;
	srand(time(0));
	Game_start();
}


