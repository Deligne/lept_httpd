#include <stdio.h>

int N = 8;
int count = 0;
char board[10][10];

typedef struct Pos
{
	int i;
	int j;
}Pos;

Pos pos[3] = {{-1,-1},{-1,1},{-1,0}};
void init() {
	for (int i = 0; i < N+2; ++i)
	{
		board[0][i] = '#';
		board[N+1][i] = '#';
		board[i][0] = '#';
		board[i][N+1] = '#';
	}
	for (int i = 1; i <=N; ++i)
	{
		for (int j = 1; j < N+1; ++j)
		{
			board[i][j] = ' ';
		}
	}
}

void display() {
	for (int i = 0; i < N+2; ++i)
	{
		for (int j = 0; j < N+2; ++j)
		{
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

int check(int x, int y) {
	int ret = 1;
	for(int p = 0; p < 3; p++) {
		int ni = x;
		int nj = y;
		while((board[ni][nj] != '#') && ret) {
			ni += pos[p].i;
			nj += pos[p].j;
			ret = ret && (board[ni][nj] != '*');
		}
	}
	return ret;
}

void find(int i) {
	int j=0;
	if(i > N) {
		count++;
		display();
	} else {
			for(j = 1; j<= N;j++) {
				if(check(i, j))
				{
					board[i][j] = '*';
					find(i+1);
					board[i][j] = ' ';
				}
		}
	}
}



int main(int argc, char const *argv[])
{
	init();
	display();
	find(1);
	printf("%d\n", count);
	return 0;
}