/*
 ������Ĵ�����23����Ļ��һ����ʾ����Ҫ����������ɿ���һ�д���
 ͬʱֻ�����������Ĵ��볬����50�У���Լ67��
 �������������Ͷ���ֿ��������������й��ܵ�ע�ͣ����岿�ֺ�����ע��
 ������������������������
 2020.3.10
 */

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define BOARD_SIZE 8
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BLACKKING 3
#define WHITEKING 4

#define TRUE 1
#define FALSE 0

#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

#define MAX_STEP 15
#define MAX_ROUTINE_NUM 120

#define MAX_DEPTH 9

#define MAX_CHESS_NUM 12

#define FINITY 1e6
 /***************************************************************************************************************************************************/
struct Command {
	int x[MAX_STEP];
	int y[MAX_STEP];
	int numStep;
};
int board[BOARD_SIZE][BOARD_SIZE] = { 0 };

int me_flag;
int other_flag;
const int dir[4][2] = { {-1,-1},{-1,1},{1,-1},{1,1} };
/*****************************************************************************************************************************************************/
typedef struct {
	int x, y;
	int eat_num;//������=����
	int status;//������
}Chess;
int can_move_chess_num = 0;//ÿ������
Chess can_move_chess[MAX_DEPTH][MAX_CHESS_NUM];
//������������������ӣ������ͬ�������Ĳ����������Լ����,ȡ�������һ��
/*****************************************************************************************************************************************************/
typedef struct {
	int road[MAX_STEP][6];
	int length;
}Road;//�ҷ�x,y,status[0][1][2],�Է����Ե�����x��y��status[3][4][5]���ڻָ�����
int road_num = 0;//·������ÿ�ζ�Ҫ����
Road road_No[MAX_DEPTH][MAX_ROUTINE_NUM];//��
const int depth = MAX_DEPTH - 1;
int bestroad = 0;//���·�߱��
/*****************************************************************************************************************************************************/
typedef struct {//��¼�ҷ�λ�úͶԷ�λ���Լ�״̬�����㳷��
	int x1, y1, status1;
	int x2, y2, status2;
}Info;
typedef struct Rou {
	Info info;
	struct Rou* par;//ָ�򸸽ڵ�,�����¼,���ӵ����һ��ֻ������x1��y1������ͣ����λ��
}Routine;
Routine* head[MAX_ROUTINE_NUM];
int head_count = 0;//·����������ÿһ��������˵��Ҫ��ʼ��
/**********************************************************************************************************************************/
//����
int x1(int depth, int best, int i);
int y1(int depth, int best, int i);
int sta1(int depth, int best, int i);
int x2(int depth, int best, int i);
int y2(int depth, int best, int i);
int sta2(int depth, int best, int i);
int len(int depth, int best);
int in_board(int a, int b);
int is_sol(int a, int b, int sta);//��������
int is_king(int a, int b, int sta);
int is_emp(int a, int b);
//��ʼ��
void init_link_head(void);//����ͷָ�������ʼ��
Routine* apply(void);//����ָ��
void link_free(int headcount);
/*�����ͷ�,һ�����յ㣬�ܶ���·��ָ�������һ����
������һ�����ͷŵ�ʱ������ͷţ������������ͷ�*/
void init_road(void);//��ʼ��·������
//·�����������
int can_eat(int eatx, int eaty, int me, int jumpx, int jumpy);//���Ӽ��麯��
int eat_count(int x, int y, int me);//������ͳ�ƣ������ƶ�Ϊ0������Ϊ������
int find_max_eat_chess(int me, int depth);//�ҵ����������������һ������
void find_eat_routine(int x, int y, int steps, Routine* p, int me);//���������·����һ�����յ�
int find_no_eat_routine(int x, int y, int depth, int me, int status);//���������ӵ�·��
void no_eat_road_add(int x1, int y1, int status, int x2, int y2, int depth);
int trans_routine_to_road(int startpoint, int depth);//��·��ת��Ϊ����Road�����road�洢,����һ�����ӵ�·����,����������
int generate_road(int me, int depth);//����·�ߵĺ��������ص�ǰ��·����
void move(int best, int depth);//���Ӻ���
void mid_emp(int depth, int best, int i);
void unmove(int best, int depth);//�������Ӻ���
//���߲������·��
double eval(void);//��������
double get_eval(void);//��������ϸ��
int def(int a, int b, int sta);
double alphabeta(int side, int depth, double alpha, double beta);//alpha��beta��֦���õ����·�߱��
int ai_turn(void);//���������������������·�߱��
void turn(void);
//��������
void start(int flag);
void end(int x);
void loop(void);
void debug(const char* str);
void print_board(void);
void place(struct Command cmd, int cur_flag);
void init_ai(int me);
/*****************************************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	init_link_head();
	init_road();
	loop();
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************************************************************************/
int x1(int depth, int best, int i) {
	return road_No[depth][best].road[i][0];
}

int y1(int depth, int best, int i) {
	return road_No[depth][best].road[i][1];
}

int sta1(int depth, int best, int i) {
	return road_No[depth][best].road[i][2];
}

int x2(int depth, int best, int i) {
	return road_No[depth][best].road[i][3];
}

int y2(int depth, int best, int i) {
	return road_No[depth][best].road[i][4];
}

int sta2(int depth, int best, int i) {
	return road_No[depth][best].road[i][5];
}

int len(int depth, int best) {
	return road_No[depth][best].length;
}

int in_board(int a, int b) {
	if (a > -1 && a<8 && b>-1 && b < 8)
		return 1;
	return 0;
}

int is_sol(int a, int b, int sta) {
	if (board[a][b] == sta)
		return 1;
	return 0;
}

int is_king(int a, int b, int sta) {
	if (board[a][b] == sta)
		return 1;
	return 0;
}

int is_emp(int a, int b) {
	if (in_board(a, b) && board[a][b] == EMPTY)
		return 1;
	return 0;
}


void init_link_head() {
	for (int i = 0; i < MAX_ROUTINE_NUM; ++i)
		head[i] = NULL;
}

Routine* apply() {
	Routine* tempt = (Routine*)malloc(sizeof(Routine));
	if (tempt == NULL) {
		printf("error");
		exit(-1);
	}
	tempt->par = NULL;
	return tempt;
}

void link_free(int headcount) {
	while (1) {
		Routine* tempt[MAX_ROUTINE_NUM];
		int flag = 0;
		for (int i = 0; i < headcount; ++i) {
			if (head[i] == NULL)
				++flag;
		}
		if (flag != headcount) {
			for (int j = 0; j < headcount; ++j) {
				tempt[j] = head[j];
				head[j] = tempt[j]->par;
			}
			for (int i = 0; i < headcount; ++i) {
				for (int j = i + 1; j < headcount; ++j)
					if (tempt[i] == tempt[j])
						tempt[j] = NULL;
				free(tempt[i]);
				tempt[i] = NULL;
			}
		}
		if (flag == headcount)
			break;
	}
}

void init_road() {
	memset(road_No, 0, sizeof(road_No));
	memset(can_move_chess, 0, sizeof(can_move_chess));
}


int can_eat(int eatx, int eaty, int me, int jumpx, int jumpy) {
	int opponent = 3 - me;
	if (def(eatx, eaty, opponent) && is_emp(jumpx, jumpy))
		return 1;
	return 0;
}


int eat_count(int x, int y, int me) {
	int max = 0;
	int flag = 0;
	for (int i = 0; i < 4; ++i) {
		int tempt = 0;
		int tx = x + dir[i][0];
		int ty = y + dir[i][1];
		if (can_eat(tx, ty, me, tx + dir[i][0], ty + dir[i][1])) {
			flag = 1;
			int a = board[x][y];
			int b = board[tx][ty];
			board[x][y] = EMPTY;
			board[tx][ty] = EMPTY;
			board[tx + dir[i][0]][ty + dir[i][1]] = me;
			tempt = eat_count(tx + dir[i][0], ty + dir[i][1], me);
			board[tx + dir[i][0]][ty + dir[i][1]] = EMPTY;
			board[x][y] = a;
			board[tx][ty] = b;
		}
		max = (max > tempt) ? max : tempt;//ÿ����֦��ȡ���ֵ����ô�ܵķ�֦�������
	}
	if (flag == 0)
		return 0;
	return max + 1;
}

int find_max_eat_chess(int me, int depth) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if (board[i][j] == me || board[i][j] == me + 2) {
				Chess tempt;
				int a = eat_count(i, j, me);
				tempt.x = i;
				tempt.y = j;
				tempt.status = board[i][j];
				tempt.eat_num = a;
				if (can_move_chess_num == 0) {//�Ƚϲ���ֻ��������������һ��
					can_move_chess[depth][can_move_chess_num] = tempt;
					++can_move_chess_num;
				}
				else {
					if (tempt.eat_num > can_move_chess[depth][0].eat_num) {
						can_move_chess[depth][0] = tempt;
						can_move_chess_num = 1;
					}
					else if (tempt.eat_num == can_move_chess[depth][0].eat_num) {
						can_move_chess[depth][can_move_chess_num] = tempt;
						++can_move_chess_num;
					}
				}
			}
		}
	}
	return can_move_chess_num;
}

void find_eat_routine(int x, int y, int steps, Routine* p, int me) {
	if (steps == 0) {
		Routine* pointer = apply();
		pointer->par = p;
		pointer->info.x1 = x;
		pointer->info.y1 = y;
		pointer->info.status1 = board[x][y];
		head[head_count] = pointer;
		++road_num;
		++head_count;
	}
	for (int i = 0; i < 4; ++i) {
		int tempt = 0;
		int tx = x + dir[i][0];
		int ty = y + dir[i][1];
		if (can_eat(tx, ty, me, tx + dir[i][0], ty + dir[i][1])) {
			int a = board[x][y];
			int b = board[tx][ty];
			board[x][y] = EMPTY;
			board[tx][ty] = EMPTY;
			board[tx + dir[i][0]][ty + dir[i][1]] = a;
			tempt = eat_count(tx + dir[i][0], ty + dir[i][1], me);
			if (tempt == steps - 1) {
				Routine* pointer = apply();//һ�����յ㣬������
				pointer->par = p;
				pointer->info.x1 = x;
				pointer->info.y1 = y;
				pointer->info.x2 = tx;
				pointer->info.y2 = ty;
				pointer->info.status1 = a;
				pointer->info.status2 = b;
				find_eat_routine
				(tx + dir[i][0], ty + dir[i][1], steps - 1, pointer, me);
			}
			board[tx + dir[i][0]][ty + dir[i][1]] = EMPTY;
			board[x][y] = a;
			board[tx][ty] = b;
		}
	}
}

int find_no_eat_routine(int x, int y, int depth, int me, int status) {
	if (me == BLACK) {
		if (status == me || status == me + 2) {
			if (is_emp(x - 1, y - 1)) //���Ϸ�,�������ӹ���
				no_eat_road_add(x, y, status, x - 1, y - 1, depth);
			if (is_emp(x - 1, y + 1)) //���Ϸ����������ӹ���
				no_eat_road_add(x, y, status, x - 1, y + 1, depth);
		}
		if (status == me + 2) {
			if (is_emp(x + 1, y - 1)) //���·�,����ר��
				no_eat_road_add(x, y, status, x + 1, y - 1, depth);
			if (is_emp(x + 1, y + 1)) //���·�,����ר��
				no_eat_road_add(x, y, status, x + 1, y + 1, depth);
		}
	}
	else if (me == WHITE) {
		if (status == me || status == me + 2) {
			if (is_emp(x + 1, y - 1)) //���·�,�������ӹ���
				no_eat_road_add(x, y, status, x + 1, y - 1, depth);
			if (is_emp(x + 1, y + 1)) //���·����������ӹ���
				no_eat_road_add(x, y, status, x + 1, y + 1, depth);
		}
		if (status == me + 2) {
			if (is_emp(x - 1, y - 1)) //���Ϸ�,����ר��
				no_eat_road_add(x, y, status, x - 1, y - 1, depth);
			if (is_emp(x - 1, y + 1)) //���Ϸ�,����ר��
				no_eat_road_add(x, y, status, x - 1, y + 1, depth);
		}
	}
	return road_num;
}

void no_eat_road_add(int x1, int y1, int status, int x2, int y2, int depth) {
	road_No[depth][road_num].road[0][0] = x1;
	road_No[depth][road_num].road[0][1] = y1;
	road_No[depth][road_num].road[0][2] = status;//��¼�����ڵ����
	road_No[depth][road_num].road[0][3] = x2;
	road_No[depth][road_num].road[0][4] = y2;
	road_No[depth][road_num].road[0][5] = EMPTY;//�ƶ�λ�õ�״̬
	road_No[depth][road_num].length = 1;
	++road_num;
}

int trans_routine_to_road(int startpoint, int depth) {//·����������
	int i;
	for (i = 0; i < head_count; ++i) {
		Routine* p = head[i];
		int tempt = i + startpoint;
		int* len = &road_No[depth][tempt].length;
		*len = 0;
		while (1) {
			road_No[depth][tempt].road[*len][0] = p->info.x1;
			road_No[depth][tempt].road[*len][1] = p->info.y1;
			road_No[depth][tempt].road[*len][2] = p->info.status1;
			if ((*len) > 0) {
				road_No[depth][tempt].road[*len][3] = p->info.x2;
				road_No[depth][tempt].road[*len][4] = p->info.y2;
				road_No[depth][tempt].road[*len][5] = p->info.status2;
			}
			++(*len);//ǡ���Ǿ�������������>=2
			if (p->par == NULL)
				break;
			p = p->par;
		}
	}
	return i;
}

int generate_road(int me, int depth) {
	int sum_road = 0;
	can_move_chess_num = 0;//���ƶ�������
	road_num = 0;//����
	find_max_eat_chess(me, depth);
	for (int i = 0; i < can_move_chess_num; ++i) {
		int x = can_move_chess[depth][i].x;
		int y = can_move_chess[depth][i].y;
		int eatnum = can_move_chess[depth][i].eat_num;
		int status = can_move_chess[depth][i].status;
		if (eatnum > 0) {
			head_count = 0;
			find_eat_routine(x, y, eatnum, NULL, me);
			int tempt = trans_routine_to_road(sum_road, depth);
			sum_road += tempt;
			link_free(head_count);
		}
		else {
			find_no_eat_routine(x, y, depth, me, status);
		}
	}
	return road_num;
}

void move(int best, int depth) {
	if (len(depth, best) == 1) {
		board[x1(depth, best, 0)][y1(depth, best, 0)] = EMPTY;
		board[x2(depth, best, 0)][y2(depth, best, 0)] = sta1(depth, best, 0);
		if (x2(depth, best, 0) == 0 && sta1(depth, best, 0) == BLACK)
			board[x2(depth, best, 0)][y2(depth, best, 0)] = BLACKKING;//����
		if (x2(depth, best, 0) == 7 && sta1(depth, best, 0) == WHITE)
			board[x2(depth, best, 0)][y2(depth, best, 0)] = WHITEKING;//����
	}
	else {
		int i = len(depth, best) - 1;
		for (; i > 0; --i) {
			board[x1(depth, best, i)][y1(depth, best, i)] = EMPTY;
			mid_emp(depth, best, i);
		}
		board[x1(depth, best, 0)][y1(depth, best, 0)] = sta1(depth, best, 0);
		if (x1(depth, best, 0) == 0 && sta1(depth, best, 0) == BLACK)//����
			board[x1(depth, best, 0)][y1(depth, best, 0)] = BLACKKING;
		if (x1(depth, best, 0) == 7 && sta1(depth, best, 0) == WHITE)//����
			board[x1(depth, best, 0)][y1(depth, best, 0)] = WHITEKING;
	}
}

void mid_emp(int depth, int best, int i) {
	board[(x1(depth, best, i) + x1(depth, best, i - 1)) / 2]
		[(y1(depth, best, i) + y1(depth, best, i - 1)) / 2] = EMPTY;
}

void unmove(int best, int depth) {
	if (len(depth, best) == 1) {
		board[x1(depth, best, 0)][y1(depth, best, 0)]
			= sta1(depth, best, 0);
		board[x2(depth, best, 0)][y2(depth, best, 0)]
			= sta2(depth, best, 0);
	}
	else {
		int flag = 1;
		int i;
		for (i = 0; i < len(depth, best) - 1; ++i) {
			board[x1(depth, best, i)][y1(depth, best, i)] = EMPTY;
			if (flag > 1)
				board[x2(depth, best, i)][y2(depth, best, i)]
				= sta2(depth, best, i);
			++flag;
		}
		board[x2(depth, best, i)][y2(depth, best, i)] = sta2(depth, best, i);
		board[x1(depth, best, i)][y1(depth, best, i)] = sta1(depth, best, i);
	}
}


/*
	1.�������ģ���������(����Ǵ���ʱ�����Ƹߵ�)
����2.����ƽ�ⷢչ(����Э������ֹ�Է���һ��ͻ��)
����3.����ע��֧�ŵ�(�����ر��Ķ�������)
����4.���Ӻ���໥��ϵ(��Ҫ�γɸ���Ŀյ�����ֹ�Է����ÿյ�����ս�����)
*/
double eval() {
	double
		b_def = 0.0, w_def = 0.0,//����
		b_num = 0.0, w_num = 0.0,//����
		b_free = 0.0, w_free = 0.0,//���ɶ�
		b_sid = 0.0, w_sid = 0.0,//����
		b_king = 0.0, w_king = 0.0,//��
		b_sum = 0.0, w_sum = 0.0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] == BLACK || board[i][j] == BLACKKING) {//1����
				if (def(i + 1, j - 1, BLACK))b_def++;
				if (def(i + 1, j + 1, BLACK))b_def++;
				if (def(i - 1, j - 1, BLACK))b_def++;
				if (def(i - 1, j + 1, BLACK))b_def++;
				if (i == 1 && j == 1)b_sid++;	//2����
				if (i == 3 && j == 1)b_sid++;
				if (i == 5 && j == 1)b_sid++;
				if (i == 7 && j == 1)b_sid++;
				if (i == 0 && j == 7)b_sid++;
				if (i == 2 && j == 7)b_sid++;
				if (i == 4 && j == 7)b_sid++;
				if (i == 6 && j == 7)b_sid++;
				if (board[i][j] == BLACKKING) {//3��
					b_king += 3;
					if (is_emp(i + 1, j - 1))b_free++;
					if (is_emp(i + 1, j + 1))b_free++;
				}
				else//4����
					b_num++;
				if (is_emp(i - 1, j - 1))b_free++;//5�ж�
				if (is_emp(i - 1, j + 1))b_free++;
			}
			if (board[i][j] == WHITE || board[i][j] == WHITEKING) {
				//1����
				if (def(i - 1, j - 1, WHITE))w_def++;
				if (def(i - 1, j + 1, WHITE))w_def++;
				if (def(i - 1, j - 1, WHITE))w_def++;
				if (def(i - 1, j + 1, WHITE))w_def++;
				//2����
				if (i == 1 && j == 1)w_sid++;
				if (i == 3 && j == 1)w_sid++;
				if (i == 5 && j == 1)w_sid++;
				if (i == 7 && j == 1)w_sid++;
				if (i == 0 && j == 7)w_sid++;
				if (i == 2 && j == 7)w_sid++;
				if (i == 4 && j == 7)w_sid++;
				if (i == 6 && j == 7)w_sid++;
				//3��
				if (board[i][j] == WHITEKING) {
					w_king += 3;
					if (is_emp(i - 1, j - 1))w_free++;
					if (is_emp(i - 1, j + 1))w_free++;
				}
				else//4����
					w_num++;
				if (is_emp(i + 1, j - 1))w_free++;//5�ж�
				if (is_emp(i + 1, j + 1))w_free++;
			}
		}
	}
	b_sum = 3.3 * b_def + 6 * b_num + 2.3 * b_free
		+ 2.8 * b_sid + 6 * b_king;
	w_sum = 3.3 * w_def + 6 * w_num + 2.3 * w_free
		+ 2.8 * w_sid + 6 * w_king;
	if (me_flag == BLACK)
		return b_sum - w_sum;
	return w_sum - b_sum;
}

double get_eval() {
	double score = eval();
	return score;
}

int def(int a, int b, int sta) {
	if (in_board(a, b) && (is_sol(a, b, sta) || is_king(a, b, sta + 2)))
		return 1;
	return 0;
}

double alphabeta(int side, int depth, double alpha, double beta) {
	if (depth == 0)
		return get_eval();
	int numroad = generate_road(side, depth);
	if (numroad == 0)
		return get_eval();//��·����Ҳ����
	double score = 0;
	if (side == me_flag) {
		for (int i = 0; i < numroad; ++i) {
			move(i, depth);
			score = alphabeta(3 - side, depth - 1, alpha, beta);
			unmove(i, depth);
			if (score > alpha) {
				alpha = score;
				if (depth == MAX_DEPTH - 1)
					bestroad = i;//���س�ʼ�㣬����
			}
			if (alpha >= beta)//alpha��֦
				return alpha;
		}
		return alpha;
	}
	else {
		for (int i = 0; i < numroad; ++i) {
			move(i, depth);
			score = alphabeta(3 - side, depth - 1, alpha, beta);
			unmove(i, depth);
			if (score < beta) {
				beta = score;
			}
			if (beta <= alpha) {
				return beta;//beta��֦
			}
		}
		return beta;
	}
}

int ai_turn() {
	int me = me_flag;//Ҫô��1��Ҫô��2
	alphabeta(me, depth, -FINITY, FINITY);
	int best = bestroad;
	return best;
}

void turn() {
	int best = ai_turn();
	int i = len(depth, best) - 1;
	if (len(depth, best) == 1)
		printf("2 %d,%d %d,%d",
			x1(depth, best, 0), y1(depth, best, 0),
			x2(depth, best, 0), y2(depth, best, 0));
	else {
		printf("%d", len(depth, best));
		for (; i > -1; --i)
			printf(" %d,%d", x1(depth, best, i), y1(depth, best, i));
	}
	printf("\n");
	fflush(stdout);
	move(best, depth);//����,������������ӣ����öԷ�ʱ������
}


void start(int flag) {
	memset(board, 0, sizeof(board));
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 8; j += 2)
			board[i][j + (i + 1) % 2] = WHITE;
	for (int i = 5; i < 8; i++)
		for (int j = 0; j < 8; j += 2)
			board[i][j + (i + 1) % 2] = BLACK;
	init_ai(flag);
}

void end(int x) {
}

void loop() {
	char tag[10] = { 0 };
	struct Command command = {
		{0},
		{0},
		0 };
	int status;
	while (TRUE)
	{
		memset(tag, 0, sizeof(tag));
		scanf("%s", tag);
		double a = clock();
		if (strcmp(tag, START) == 0)
		{
			scanf("%d", &me_flag);
			other_flag = 3 - me_flag;
			start(me_flag);
			printf("OK\n");
			fflush(stdout);
		}
		else if (strcmp(tag, PLACE) == 0)
		{
			scanf("%d", &command.numStep);
			for (int i = 0; i < command.numStep; ++i)
			{
				scanf("%d,%d", &command.x[i], &command.y[i]);
			}
			place(command, other_flag);
		}
		else if (strcmp(tag, TURN) == 0)
		{
			turn();
		}
		else if (strcmp(tag, END) == 0)
		{
			scanf("%d", &status);
			end(status);
		}
		double b = clock();
		//print_board();//�ύ֮ǰ�ص�
		//printf("%f����\n", 1000 * (b - a) / CLOCKS_PER_SEC);//�ύ֮ǰ�ص�
	}
}

void debug(const char* str) {
	printf("DEBUG %s\n", str);
	fflush(stdout);
}

void print_board() {
	char visual_board[BOARD_SIZE][BOARD_SIZE + 1] = { 0 };
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (board[i][j] == EMPTY)
				visual_board[i][j] = '.';//����
			else if (board[i][j] == BLACK)
				visual_board[i][j] = 'b';
			else if (board[i][j] == WHITE)
				visual_board[i][j] = 'w';
			else if (board[i][j] == WHITEKING)
				visual_board[i][j] = 'W';
			else if (board[i][j] == BLACKKING)
				visual_board[i][j] = 'B';
		}
		printf("%s\n", visual_board[i]);
		fflush(stdout);
	}
}

void place(struct Command cmd, int cur_flag) {
	int x_mid, y_mid;
	int i;
	int a = board[cmd.x[0]][cmd.y[0]];
	for (i = 0; i < cmd.numStep - 1; ++i) {
		board[cmd.x[i]][cmd.y[i]] = EMPTY;
		board[cmd.x[i + 1]][cmd.y[i + 1]] = cur_flag;
		if (abs(cmd.x[i] - cmd.x[i + 1]) == 2)//���Ӳ���
		{
			x_mid = (cmd.x[i] + cmd.x[i + 1]) / 2;
			y_mid = (cmd.y[i] + cmd.y[i + 1]) / 2;
			board[x_mid][y_mid] = EMPTY;
		}
	}
	board[cmd.x[i]][cmd.y[i]] = a;
	if (cmd.x[i] == 0 && a == BLACK)//��������
		board[cmd.x[i]][cmd.y[i]] = BLACKKING;
	if (cmd.x[i] == 7 && a == WHITE)
		board[cmd.x[i]][cmd.y[i]] = WHITEKING;
}

void init_ai(int me) {
}