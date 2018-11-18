#include "Node.cpp"

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
	char choose;
	cout << "              ==================" << endl;
	cout << "             黑白棋游戏(黑方先手)" << endl;
	cout << "              ==================" << endl;
	cout << "             请选择是否先手(Y/N):";
	cin >> choose;
	cin.clear();
	cin.sync();
	if (choose != 'Y' && choose != 'y') player = 0; 

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
			cin.clear();
			cin.sync();
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