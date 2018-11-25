#include "Node.cpp"

void make_tree_with_depth(Node* start, int& next_move, int turn=1, int length=0, int depth=2){
	//turnΪ1��ʾ��һ���ߺ���
	if (length >= depth || start->next_i.size() == 0 || start->beta <= start->alpha){
		start->H = start->calc_H();
		start->alpha = max(start->alpha, start->H);
		start->beta = min(start->beta, start->H);
		return;
	}
	int next_move_tmp = -3;
	for (unsigned int i = 0; i < start->next_i.size(); i++){
		int next_x = start->next_i[i];
		int next_y = start->next_j[i];
		Node * next = new Node(start->MAP, 1-turn, start->alpha, start->beta, next_x, next_y);
		start->child[i] = next;
		make_tree_with_depth(next, next_move_tmp, 1-turn, length+1, depth);
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
	cout << "           ******��ǰ��ȣ�" << depth << "********" << endl;
	while(!Q.empty()){
		Node * front = Q.front();
		Q.pop();
		sum++;
		//front->show_map();
		//cout << "           H,alpha,beta=" << front->H << ',' << front->alpha << ',' << front->beta << endl;
		for (unsigned int i = 0; i < front->child.size(); i++)
			if (front->child[i] == NULL) break;
			else Q.push(front->child[i]); 
		if (--count == 0){
			//cout << "           ******��ǰ��ȣ�" << ++depth << "********" << endl;
			//count = Q.size();
		}
	}
	cout << "           ******�ڵ�����" << sum << "********" << endl;
	cout << "           H,alpha,beta=" << start->H << ',' << start->alpha << ',' << start->beta << endl;
}

void Game_start(){
	Node *A = new Node();
	Node *temp = NULL;
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

	while(!A->isGameOver()){
		A->get_chess_num(black, white);
		cout << "              ==================" << endl;
		cout << "              ��ǰ״̬"<< "(�ֵ�" << (A->turn? "��":"��") << "��)" << endl;
		cout << "               ���ӣ�" << black << " ���ӣ�" << white << endl;
		cout << "              ==================" << endl;
		A->show_map();
		if (A->next_i.size() == 0){
			cout << " ��ǰ��û�п��ߵ��壬������ǰ��" << endl;
			pos_i = pos_j = -1;
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
				while(pos_i >= N || pos_i < 0 || pos_j >= N || pos_j < 0 || A->MAP[pos_i][pos_j] != '*'){
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
				if (A->next_i.size() <= 3) make_tree_with_depth(A, next_move, A->turn, 0, 9);
				else make_tree_with_depth(A, next_move, A->turn, 0, 8);
				pos_i = A->next_i[next_move];
				pos_j = A->next_j[next_move];
				cout << " ���Լ�������ѡ����(" << pos_i << ',' << pos_j << ")\n";
				cout << "           H,alpha,beta=" << A->H << ',' << A->alpha << ',' << A->beta << endl;
			}
		}
		temp = A;
		A = new Node(A->MAP, 1 - A->turn, INT_MIN, INT_MAX, pos_i, pos_j);
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

void Game_start2(){
	Node *A = new Node();
	Node *temp = NULL;
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

	while(!A->isGameOver()){
		A->get_chess_num(black, white);
		cout << "              ==================" << endl;
		cout << "              ��ǰ״̬"<< "(�ֵ�" << (A->turn? "��":"��") << "��)" << endl;
		cout << "               ���ӣ�" << black << " ���ӣ�" << white << endl;
		cout << "              ==================" << endl;
		A->show_map();
		if (A->next_i.size() == 0){
			cout << " ��ǰ��û�п��ߵ��壬������ǰ��" << endl;
			pos_i = pos_j = -1;
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
			{
				int next_move = -1;
				cout << "\n ���Լ�����..." << endl;
				if (A->next_i.size() <= 3) make_tree_with_depth(A, next_move, A->turn, 0, 9);
				else make_tree_with_depth(A, next_move, A->turn, 0, 8);
				pos_i = A->next_i[next_move];
				pos_j = A->next_j[next_move];
				cout << " ���Լ�������ѡ����(" << pos_i << ',' << pos_j << ")\n";
				cout << "           H,alpha,beta=" << A->H << ',' << A->alpha << ',' << A->beta << endl;
			}
		}
		temp = A;
		A = new Node(A->MAP, 1 - A->turn, INT_MIN, INT_MAX, pos_i, pos_j);
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

int main(){
	Game_start();
}