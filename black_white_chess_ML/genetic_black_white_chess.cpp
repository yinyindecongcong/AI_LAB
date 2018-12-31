#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
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

vector<int> G_weight(16,1);
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

string tochar(int a){
	if (a == 1) return "��";
	if (a == -1) return "��";
	if (a == 0) return "��";
	return "  ";
}

struct State
{
	int MAP[N][N];
	int turn; //turnΪ1��ʾ�ֵ��ڷ�
	vector<int> next_i;
	vector<int> next_j;
	/*ʵ��alpha-beta��������*/
	vector<State*> child;
	int alpha, beta;
	int H;

	State(int M[N][N]=Start_Map, int t=1,int a=INT_MIN, int b=INT_MAX, 
		int pos_i=-1, int pos_j=-1,bool show_pos=true):turn(t),alpha(a),beta(b){
		set_MAP(M);
		//��pos_j��Ϊ0����ʾ��һ��State������(pos_i, pos_j)λ��
		if (pos_j != -1) {turn = -t; move(pos_i, pos_j); turn = t;}
		get_next_pos(show_pos);
		child = vector<State*>(next_i.size(), NULL);
	}

	//���ո���Ȩ�ؾ���������
	int calc_H(vector<int>& weight){
		int white_score = 0, black_score = 0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++){
					int a = (i < N/2? i: N - 1 - i);
					int b = (j < N/2? j: N - 1 - j);
				if (MAP[i][j] == -1) 
					white_score += weight[a * N/2 + b];
				else if (MAP[i][j] == 1) 
					black_score += weight[a * N/2 + b];
			}
		return black_score - white_score;
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
		//ˮƽ����
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
		//��ֱ����
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
		//���¶Խ���
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
		//���¶Խ���
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
		//ˮƽ����
		k = j + 1;
		while(k < N && MAP[i][k] == -turn) k++;
		if (k < N && MAP[i][k] == turn) 
			for (int w = j + 1; w < k; w++) MAP[i][w] = turn;
		k = j - 1;
		while(k >= 0 && MAP[i][k] == -turn) k--;
		if (k >= 0 && MAP[i][k] == turn) 
			for (int w = j - 1; w > k; w--) MAP[i][w] = turn;
		//��ֱ����
		k = i + 1;
		while (k < N && MAP[k][j] == -turn) k++;
		if (k < N && MAP[k][j] == turn)
			for (int w = i + 1; w < k; w++) MAP[w][j] = turn;
		k = i - 1;
		while (k >= 0 && MAP[k][j] == -turn) k--;
		if (k >= 0 && MAP[k][j] == turn)
			for (int w = i - 1; w > k; w--) MAP[w][j] = turn;
		//���¶Խ���
		k = 1;
		while (j + k < N && i + k < N && MAP[i+k][j+k] == -turn) k++;
		if (j + k < N && i + k < N && MAP[i+k][j+k] == turn)
			for (int w = 1; w < k; w++) MAP[i+w][j+w] = turn;
		k = 1;
		while (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] == -turn) k++;
		if (j - k >= 0 && i - k >= 0 && MAP[i-k][j-k] == turn)
			for (int w = 1; w < k; w++) MAP[i-w][j-w] = turn;
		//���¶Խ���
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

/*alpha-beta��֦*/
void make_tree_with_depth(State* start, int& next_move, vector<int>&weight, int length=0, int depth=2){
	//turnΪ1��ʾ��һ���ߺ���
	if (length >= depth || start->next_i.size() == 0 || start->beta <= start->alpha){
		start->H = start->calc_H(weight);
		start->alpha = max(start->alpha, start->H);
		start->beta = min(start->beta, start->H);
		return;
	}
	int next_move_tmp = -3;
	for (unsigned int i = 0; i < start->next_i.size(); i++){
		int next_x = start->next_i[i];
		int next_y = start->next_j[i];
		State * next = new State(start->MAP, -start->turn, start->alpha, 
								 start->beta, next_x, next_y);

		start->child[i] = next;
		make_tree_with_depth(next, next_move_tmp, weight, length+1, depth);
		if (start->turn == 1 && next->beta > start->alpha)
			{start->alpha = next->beta; next_move = i;}
		else if (start->turn == -1 && next->alpha < start->beta)
			{start->beta = next->alpha; next_move = i;}
		delete next;
	}

}

//��ʾDNA
void show_DNAs(vector<vector<int> >& DNAs){
	for (int i = 0; i < (int)DNAs.size(); i++){
		cout << "DNA " << i << " >>>" << endl;
		for (int j = 0; j < (N * N / 4); j++){
			cout << DNAs[i][j] << ' ';
			if (j % 4 == 3) cout << endl;
		}
		cout << endl;
	}
}

//�����ʼ����Ⱥ
void initialize(vector<vector<int> >& DNAs){
	for (int i = 0; i < (int)DNAs.size(); i++)
		for (int j = 0; j < (N * N / 4); j++)
			DNAs[i][j] = 1 + rand() % 50;
}

//�ھ���ʱ˫������DNAѡ���ƶ�λ�ã������Զ�
void get_move(State * A, vector<int>& DNA, int & pos_i, int & pos_j){
	/*ֱ��ѡ������
	int max_w = 0, max_i = 0;
	for (unsigned int i = 0; i < A->next_i.size(); i++){
		int a = (A->next_i[i] < N / 2? A->next_i[i]: N - 1 - A->next_i[i]);
		int b = (A->next_j[i] < N / 2? A->next_j[i]: N - 1 - A->next_j[i]);
		if (DNA[a * N / 2 + b] > max_w){
			max_w = DNA[a * N / 2 + b];
			max_i = i;
		}
	pos_i = A->next_i[max_i];
	pos_j = A->next_j[max_i];
	}
	*/

	/*����Ȩ�����ѡ��Ȩ�ش��ѡ�п�����Խ��
	int temp[A->next_i.size() + 1] = {0};
	for (unsigned int i = 0; i < A->next_i.size(); i++){
		int a = (A->next_i[i] < N / 2? A->next_i[i]: N - 1 - A->next_i[i]);
		int b = (A->next_j[i] < N / 2? A->next_j[i]: N - 1 - A->next_j[i]);
		temp[i + 1] = temp[i] + DNA[a * N / 2 + b];
	}
	int choice = rand() % temp[A->next_i.size()];
	unsigned int i = 0;
	for (; i < A->next_i.size(); i++)
		if (choice <= temp[i + 1]) break;
	pos_i = A->next_i[i];
	pos_j = A->next_j[i];
	*/

	//alpha-beta��֦����Ȩ�ؽ���ѡ��
	int next_move = -1;
	make_tree_with_depth(A, next_move, DNA, 0, 4);
	pos_i = A->next_i[next_move];
	pos_j = A->next_j[next_move];
}

//�������վ����ۺ����Զ�
int fight(vector<int>& player1, vector<int>& player2){
	State *A = new State(), *temp = NULL;
	int black, white;
	int player = 1; //�Ƿ�Ϊ�ڷ�
	int No_way = 0;
	int pos_i = -1, pos_j = -1;
	while(1){
		A->get_chess_num(black, white);
		if (black + white == 64) break;
		if (A->next_i.size() == 0){
			//��ǰ��û�п��ߵ��壬������ǰ��
			if (++No_way == 2)//˫����û�п��ߵ��壬����
				break;
		}
		else{
			No_way = 0;
			//player1˳��
			if (A->turn == player) get_move(A, player1, pos_i, pos_j);
			//player2˳��
			else get_move(A, player2, pos_i, pos_j);
			A->move(pos_i, pos_j);
		}
		temp = A;
		A = new State(A->MAP, -A->turn);
		delete temp;
		temp = NULL;
	}
	A->get_chess_num(black, white);
	delete A;
	return (black >= white? 1: -1); //����player1�Ƿ�ʤ��
}

//��[start, end)���ѡ�������±���о���
void random_choose_DNA(int& index1, int& index2, int start, int end){
	index1 = rand() % (end - start) + start;
	do {
		index2 = rand() % (end - start) + start;
	}while(index1 == index2);
}

//ѡ�����Ӧ�ȸߵĸ���
int selection(vector<vector<int> >& DNAs){
	//Ҫ�ĳ������Ծֻ��֣����Ծ���̫�࣬��Ϊÿ�������ѡ��3���˶Ծ�
	unsigned int k = 100, score = 0;
	vector<int> fitness(DNAs.size(),0);
	for (unsigned int i = 0; i < DNAs.size(); i++){
		for(unsigned int j = 0; j < 3; j++)
			do {k = rand() % DNAs.size();} while(k == i);
			if (i != k) fitness[i] += fight(DNAs[i], DNAs[k]);
	}
	//������Ӧ�ȸ����������
	for (unsigned int i = 0; i < DNAs.size() - 1; i++){
		int max_idx = 0, max_score = -10;
		for (unsigned int j = i; j < DNAs.size(); j++)
			if (fitness[j] > max_score){ max_idx = j; max_score = fitness[j]; }
		swap(fitness[i], fitness[max_idx]);
		swap(DNAs[i], DNAs[max_idx]);
		if (i < 5) score += fitness[i];
	}
	/*ÿ��ѡ��2�����壬���Ⱥ��ֶԾ����ɴ�
	int index1, index2;
	for (int i = 0; i < (int)DNAs.size() / 2; i++){
		random_choose_DNA(index1, index2, i, DNAs.size());
		//��ѡ����ԣ����ѡ�������о�����Ӯ������
		int player1_win = 0;
		//ʹ��alpha-betaʱֻ��Ҫ��һ�֣�����ǹ̶���
		player1_win += fight(DNAs[index1], DNAs[index2]);
		player1_win -= fight(DNAs[index2], DNAs[index1]);
		// for (int r = 0; r < 5; r++) player1_win += fight(DNAs[index1], DNAs[index2]);
		// for (int r = 0; r < 5; r++) player1_win -= fight(DNAs[index2], DNAs[index1]);
		// cout << "Competition between DNA " << index1 << " and " << index2 << ">>>";
		// cout << "DNA " << index1 << " gets score: " << player1_win << endl;
		score += abs(player1_win);
		if (player1_win)swap(DNAs[i], DNAs[index1]);
		else swap(DNAs[i], DNAs[index2]);
	}
	*/
	return score;
}

//���棬�Զ�
void crossover(vector<vector<int> >& DNAs){
	int index1, index2;
	for (unsigned int i = DNAs.size() / 2; i < DNAs.size() - 5; i++){
		/*���ѡ������selection��ѡ���DNA�����Ƚ���*/
		random_choose_DNA(index1, index2, 0, (int)DNAs.size() / 2);
		for (int j = 0; j < N * N / 4; j++){
			int tmp = rand() % 2;
			DNAs[i][j] = (tmp? DNAs[index1][j]: DNAs[index2][j]);
		}
	}
}

//���죬�Զ�
void mutation(vector<vector<int> >& DNAs){
	for (int i = 0; i < 5; i++){
		/*���ѡ��ǰ��selection��crossover��DNA�����ȱ���*/
		int index = rand() % (DNAs.size() - 5);
		for (int j = 0; j < N * N / 4; j++){
			int r = rand() % 10;
			//�滻�ĸ���Ϊ6/10
			DNAs[DNAs.size() - i - 1][j] = DNAs[index][j];
			if (r <= 6) DNAs[DNAs.size() - i - 1][j] += (rand() % 20 - 10); 
			if (DNAs[DNAs.size() - i - 1][j] <= 0)DNAs[DNAs.size() - i - 1][j] = 1; 
			if (DNAs[DNAs.size() - i - 1][j] > 49)DNAs[DNAs.size() - i - 1][j] = 49; 
		}
	}
}

//�Ŵ��㷨
void genetic_algorithm(vector<vector<int> > &DNAs, int epoch = 100){
	initialize(DNAs);
	int i = 0;
	while (i < epoch){
		int score_sum = 0;
		score_sum += selection(DNAs);
		crossover(DNAs);
		mutation(DNAs);
		
		if (i % 100 == 0){
			//������û��������
			printf("Epoch %d/%d >>> score_sum = %d/20\n", i, epoch, score_sum);
			show_DNAs(DNAs);
		}
		i++;
	}
}

void Game_start(){
	State *A = new State();
	State *temp = NULL;
	int pos_i = -1, pos_j = -1;
	int black, white;
	int player = 1; //��ʾ���Ϊ�ڷ�
	int No_way = 0;
	char choose;
	cout << "              ==================" << endl;
	cout << "             �ڰ�����Ϸ(�ڷ�����)" << endl;
	cout << "              ==================" << endl;
	cout << "             ��ѡ���Ƿ�����(Y/N):";
	cin >> choose;
	cin.clear();
	cin.sync();
	if (choose != 'Y' && choose != 'y') player = 0; 

	while(1){
		A->get_chess_num(black, white);
		cout << "              ==================" << endl;
		cout << "              ��ǰ״̬"<< "(�ֵ�" << (A->turn? "��":"��") << "��)" << endl;
		cout << "               ���ӣ�" << black << " ���ӣ�" << white << endl;
		cout << "              ==================" << endl;
		A->show_map();
		if (black + white == 64) break;
		if (A->next_i.size() == 0){
			cout << " ��ǰ��û�п��ߵ��壬������ǰ��" << endl;
			if (++No_way == 2){
				cout << "              ==================" << endl;
				cout << "              ˫����û�п��ߵ��壬����" << endl;
				cout << "              ==================" << endl;
				break;
			}
		}
		else{
			No_way = 0;
			cout << " ���ߵ���Ϊ��";
			for (unsigned int i = 0; i < A->next_i.size(); i++)
				cout << '(' << A->next_i[i] << ',' << A->next_j[i] << ") ";
			//���˳��
			if (A->turn == player){
				cout << " ���������ӵ�����(��: "<< A->next_i[0] << ' ' << A->next_j[0] << ")��";
				cin >> pos_i >> pos_j;
				while(pos_i >= N || pos_i < 0 || pos_j >= N || pos_j < 0 || A->MAP[pos_i][pos_j] != 2){
					cin.clear();
					cin.sync();
					cout << "\n ���ǺϷ������꣬����������(��: "<< A->next_i[0] << ' ' << A->next_j[0] << ")��";
					cin >> pos_i >> pos_j;
				}
				cin.clear();
				cin.sync();
			}
			//����˳��
			else{
				int next_move = -1;
				cout << "\n ���Լ�����..." << endl;
				make_tree_with_depth(A, next_move, G_weight, 0, 5);
				pos_i = A->next_i[next_move];
				pos_j = A->next_j[next_move];
				cout << " ���Լ�������ѡ����(" << pos_i << ',' << pos_j << ")\n";
				cout << "           H,alpha,beta=" << A->H << ',' << A->alpha << ',' << A->beta << endl;
			}
			A->move(pos_i, pos_j);
		}
		temp = A;
		A = new State(A->MAP, -A->turn, INT_MIN, INT_MAX);
		delete temp;
		temp = NULL;
	}
	A->get_chess_num(black, white);
	cout << "              ==================" << endl;
	cout << "                   ����״̬" << endl;
	cout << "               ���ӣ�" << black << " ���ӣ�" << white << endl;
	cout << "              ==================" << endl;
	A->show_map();
}


int main(int argc, char const *argv[]){
	//Game_start();
	srand(time(0));
	vector<vector<int> > DNAs(20, vector<int>(16,0));
	clock_t start = clock();
	genetic_algorithm(DNAs, 20000);
	clock_t end = clock();
	printf("The 20000 epoches of genetic algorithm using totally: %f ms\n", 
			(end - start) * 1000.0 / CLOCKS_PER_SEC);
	// srand(time(0));
	// vector<vector<int> > DNAs(10, vector<int>(16,0));
	// initialize(DNAs);
	// show_DNAs(DNAs);
	// selection(DNAs);
	// show_DNAs(DNAs);
	// corssover(DNAs);
	// show_DNAs(DNAs);
	// mutation(DNAs);
	// show_DNAs(DNAs);
}