#include "Node.h"

Node::Node(char M[N][N], int t, int a, int b, int pos_i, int pos_j):turn(t),alpha(a),beta(b){
	set_MAP(M, pos_i, pos_j);
	if (pos_i != -1) move(pos_i, pos_j);
	get_next_pos();
	child = vector<Node*>(next_i.size(), NULL);
}

void Node::set_MAP(char M[N][N], int pos_i, int pos_j){
	GameOver = 1;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++){
			this->MAP[i][j] = M[i][j] == '*'? ' ': M[i][j];
			if (this->MAP[i][j] == ' ' && (pos_i != i) && (pos_j != j)) GameOver = 0;
		}
}

void Node::get_next_pos(){
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (this->MAP[i][j] - '0' == this->turn) get_next_pos_with_ij(i, j);
}

int Node::calc_H(){
	int white = 0, black = 0, white_weight = 0, black_weight = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (MAP[i][j] == '0') {
				white++;
				white_weight += weight[i][j];
			}
			else if (MAP[i][j] == '1') {
				black++;
				black_weight += weight[i][j];
			}
	if (next_i.size() == 0 && !GameOver) 
		if (turn == 1 && white > black) return -1000;
		else if (turn == 0 && white < black) return 1000;
	Node * temp = new Node(this->MAP, 1 - this->turn);
	int black_move = (this->turn? next_i.size(): temp->next_i.size());
	int white_move = (this->turn == 0? next_i.size(): temp->next_i.size());
	delete temp;
	return (black_weight - white_weight) + (black_move - white_move);
}

void Node::get_next_pos_with_ij(int i, int j){
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

void Node::move(int i, int j){
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

void Node::show_map(){
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

bool Node::isGameOver(){
	return GameOver;
}

void Node::get_chess_num(int & black, int & white){
	black = white = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (MAP[i][j] == '1') black++;
			else if (MAP[i][j] == '0') white++;
}