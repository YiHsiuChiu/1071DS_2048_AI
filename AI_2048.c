#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>		
#include<stdbool.h>

int realmap[4][4];
int smap[4][4];
int tmap[4][4];
int Bmap[4][4];
int step[9999];
int times=0;

void StartGame(int map[][4]);
int GetNum(void);
void AddNum(int map[][4]);
void PrintMap(int map[][4]);
bool MovingUp(int nowmap[][4], int premap[][4]);
bool MovingDown(int nowmap[][4], int premap[][4]);
bool MovingRight(int nowmap[][4], int premap[][4]);
bool MovingLeft(int nowmap[][4], int premap[][4]);
bool Lose(int map[][4]);
int logB(int num);
void setBmap(int Bmap[][4], int map[][4]);
void next(int realmap[][4], int map[][4]);
double getscore(int map[][4], int Bmap[][4]);
int empty(int map[][4]);

int smoothness(int Bmap[][4]);
int monotonicity2(int Bmap[][4]);
double emptycells(int map[][4]);
int maxValue(int Bmap[][4]);

double tryUp(int premap[][4], int nowmap[][4], double point,int layer);
double tryDown(int premap[][4], int nowmap[][4], double point, int layer);
double tryRight(int premap[][4], int nowmap[][4], double point, int layer);
double tryLeft(int premap[][4], int nowmap[][4], double point, int layer);

void StartGame(int map[][4]) {
	for (int i = 0; i < 4; i++) 
		for (int j = 0; j < 4; j++) 
			map[i][j] = 0;
	AddNum(map); AddNum(map);
}	

int GetNum(void) {
	srand(time(NULL));
	int randnum = rand() % 10;
	if (randnum == 9)
		return 4;
	else
		return 2;
}	

void AddNum(int map[][4]) {
	srand(time(NULL));
	while (1) {
		int randadd = rand() % 16;
		if (map[randadd / 4][randadd % 4] != 0)
			continue;
		else {
			int num = GetNum();
			map[randadd / 4][randadd % 4] = num;
			break;
		}
	}
}	

void PrintMap(int map[][4]) {
	printf("\n*************************\n");
	for (int i = 0; i < 4; i++) {
		printf("*     *     *     *     *\n");
		if(map[i][0]!=0)
			printf("*  %d  ", map[i][0]); 
		else
			printf("*     ");

		if (map[i][1]!=0)
			printf("*  %d  ", map[i][1]);
		else
			printf("*     ");

		if (map[i][2]!=0)
			printf("*  %d  ", map[i][2]);
		else
			printf("*     ");

		if (map[i][3]!=0)
			printf("*  %d  *\n", map[i][3]);
		else
			printf("*     *\n");
		printf("*     *     *     *     *\n");
		printf("*************************\n");
	}
}	

bool MovingUp(int nowmap[][4], int premap[][4]) {
	for (int i = 0; i < 4; i++) 
		for (int j = 0; j < 4; j++) 
			nowmap[i][j] = premap[i][j];
	bool can_move = false;
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			if (nowmap[j][i] != 0 && nowmap[j - 1][i] == 0) {
				can_move = true;
				int k = j;
				while (k > 0 && nowmap[k - 1][i] == 0) {
					nowmap[k - 1][i] = nowmap[k][i];
					nowmap[k][i] = 0;
					k--;
				}
			}
		}	
		for (int j = 1; j < 4; j++) {
			if (nowmap[j][i] != 0 && nowmap[j][i] == nowmap[j - 1][i]) {
				can_move = true;
				nowmap[j - 1][i] *= 2;
				nowmap[j][i] = 0;
				int k = j;
				while (k < 3 && nowmap[k + 1][i] != 0) {
					nowmap[k][i] = nowmap[k + 1][i];
					nowmap[k + 1][i] = 0;
					k++;
				}
			}
		}	
	}
	if (can_move)
		return true;
	else
		return false;
}
bool check(void){
	int a=0;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(realmap[i][j]>a)
				a=realmap[i][j];
		}
	}
	if(a<1024){
		StartGame(realmap);times=0;
		return false;
	}
	else if(a==1024&&GetNum()==2){
		StartGame(realmap);times=0;
		return false;
	}	
	return true;	
}
bool MovingDown(int nowmap[][4], int premap[][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			nowmap[i][j] = premap[i][j];
	bool can_move = false;
	for (int i = 0; i < 4; i++) {
		for (int j = 3; j >= 1; j--) {
			if (nowmap[j][i] == 0 && nowmap[j - 1][i] != 0) {
				can_move = true;
				nowmap[j][i] = nowmap[j - 1][i];
				nowmap[j - 1][i] = 0;
				int k = j;
				while (k < 3 && nowmap[k + 1][i] == 0) {
					nowmap[k + 1][i] = nowmap[k][i];
					nowmap[k][i] = 0;
					k++;
				}
			}
		}	
		for (int j = 3; j >= 1; j--) {
			if (nowmap[j - 1][i] != 0 && nowmap[j][i] == nowmap[j - 1][i]) {
				can_move = true;
				nowmap[j][i] *= 2;
				nowmap[j - 1][i] = 0;
				int k = j - 1;
				while (k > 0 && nowmap[k - 1][i] != 0) {
					nowmap[k][i] = nowmap[k - 1][i];
					nowmap[k - 1][i] = 0;
					k--;
				}
			}
		}	
	}
	if (can_move)
		return true;
	else
		return false;
}

bool MovingLeft(int nowmap[][4], int premap[][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			nowmap[i][j] = premap[i][j];
	bool can_move = false;
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			if (nowmap[i][j] != 0 && nowmap[i][j - 1] == 0) {
				can_move = true;
				int k = j;
				while (k > 0 && nowmap[i][k - 1] == 0) {
					nowmap[i][k - 1] = nowmap[i][k];
					nowmap[i][k] = 0;
					k--;
				}
			}
		}
		for (int j = 1; j < 4; j++) {
			if (nowmap[i][j] != 0 && nowmap[i][j] == nowmap[i][j - 1]) {
				can_move = true;
				nowmap[i][j - 1] *= 2;
				nowmap[i][j] = 0;
				int k = j;
				while (k < 3 && nowmap[i][k + 1] != 0) {
					nowmap[i][k] = nowmap[i][k + 1];
					nowmap[i][k + 1] = 0;
					k++;
				}
			}
		}	
	}
	if (can_move)
		return true;
	else
		return false;
}

bool MovingRight(int nowmap[][4], int premap[][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			nowmap[i][j] = premap[i][j];
	bool can_move = false;
	for (int i = 0; i < 4; i++) {
		for (int j = 3; j >= 1; j--) {
			if (nowmap[i][j] == 0 && nowmap[i][j - 1] != 0) {
				can_move = true;
				nowmap[i][j] = nowmap[i][j - 1];
				nowmap[i][j - 1] = 0;
				int k = j;
				while (k < 3 && nowmap[i][k + 1] == 0) {
					nowmap[i][k + 1] = nowmap[i][k];
					nowmap[i][k] = 0;
					k++;
				}
			}
		}	
		for (int j = 3; j >= 1; j--) {
			if (nowmap[i][j - 1] != 0 && nowmap[i][j] == nowmap[i][j - 1]) {
				can_move = true;
				nowmap[i][j] *= 2;
				nowmap[i][j - 1] = 0;
				int k = j - 1;
				while (k > 0 && nowmap[i][k - 1] != 0) {
					nowmap[i][k] = nowmap[i][k - 1];
					nowmap[i][k - 1] = 0;
					k--;
				}
			}
		}	
	}
	if (can_move)
		return true;
	else
		return false;
}

bool Lose(int map[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (map[i][j] == 0)
				return false;
		}
	}	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i - 1 >= 0)
				if (map[i][j] == map[i - 1][j])
					return false;
			if (j - 1 >= 0)
				if (map[i][j] == map[i][j - 1])
					return false;
			if (i + 1 <= 3)
				if (map[i][j] == map[i + 1][j])
					return false;
			if (j + 1 <= 3)
				if (map[i][j] == map[i][j + 1])
					return false;
		}
	}	
	if(check())		
		return true;
	else return false;
}

int logB(int num) {
	if (num == 0)
		return 0;
	int result = 1;
	while (num / 2 != 1) {
		result++;
		num = num / 2;
	}
	return result;
}

void setBmap(int Bmap[][4], int map[][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			Bmap[i][j] = logB(map[i][j]);
}

void next(int realmap[][4], int map[][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			realmap[i][j] = map[i][j];
}

double getscore(int map[][4], int Bmap[][4]) {
	return smoothness(Bmap) + 0.15*monotonicity2(Bmap) + emptycells(map) + maxValue(Bmap);
}

int empty(int map[][4]) {
	int num = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (map[i][j] == 0)
				num++;
		}
	}
	return num;
}

int smoothness(int Bmap[][4]) {
	int result=0;
	for (int i = 0; i < 3; i++) {
		for(int j=0;j<4;j++){
			if (Bmap[i][j] == 0)
				continue;
			if (j != 3&&Bmap[i][j + 1]!=0)
				result += abs(Bmap[i][j] - Bmap[i][j + 1]);
			if(Bmap[i + 1][j]!=0)
				result += abs(Bmap[i][j]-Bmap[i + 1][j]);
		}
	}
	return -result;
}

int monotonicity2(int Bmap[][4]) {
	int num,right = 0, left = 0, up = 0, down = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (Bmap[i][j] == 0)
				continue;
			if (j != 3 && Bmap[i][j + 1] != 0) {
				num = Bmap[i][j] - Bmap[i][j + 1];
				if (num > 0 && right < num)
					right = num;
				else if (num < 0 && left < -num)
					left = -num;
			}
			if (Bmap[i + 1][j] != 0) {
				num = abs(Bmap[i][j] - Bmap[i + 1][j]);
				if (num > 0 && up < num)
					up = num;
				else if (num < 0 && down < -num)
					down = -num;
			}
		}
	}
	right = -right, left = -left, up = -up, down = -down;
	if (right < left)
		right = left;
	if (up < down)
		up = down;
	return right + up;
}

double emptycells(int map[][4]) {
	int n=empty(map);
	if (n==0)
		return 0;
	else if(n==1)
		return 0;
	else if(n==2)
		return 0.693;
	else if(n==3)
		return 1.098;
	else if(n==4)
		return 1.386;
	else if(n==5)
		return 1.609;
	else if(n==6)
		return 1.791;
	else if(n==7)
		return 1.945;
	else if(n==8)
		return 2.079;
	else if(n==9)
		return 2.197;
	else if(n==10)
		return 2.302;
	else if(n==11)
		return 2.397;
	else if(n==12)
		return 2.484;
	else if(n==13)
		return 2.56;
	else
		return 2.63;
}

int maxValue(int Bmap[][4]) {
	int max = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (max < Bmap[i][j])
				max = Bmap[i][j];
		}
	}
	return max;
}

double tryUp(int premap[][4], int nowmap[][4],double point, int layer){
	layer++;
	double temp;
	double point2;
	if (MovingUp(nowmap, premap)) {
		int n = empty(nowmap);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (nowmap[i][j] == 0) {
					nowmap[i][j] = GetNum();			
					point2 = 0;
					temp = MovingUp(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingDown(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingLeft(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingRight(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);
					point += point2;	
					smap[i][j] = 0;
				}
			}
		}
		return point/n;
	}
	else
		return -9999;

}

double tryDown(int premap[][4], int nowmap[][4], double point, int layer) {
	layer++;
	double temp;
	double point2 = 0;
	if (MovingDown(nowmap, premap)) {
		int n = empty(nowmap);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (nowmap[i][j] == 0) {
					nowmap[i][j] = GetNum();
					point2 = 0;
					temp = MovingUp(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingDown(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingLeft(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingRight(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);
					point += point2;
					smap[i][j] = 0;
				}
			}
		}
		return point/n;
	}
	else
		return -9999;


}

double tryLeft(int premap[][4], int nowmap[][4], double point, int layer) {
	layer++;
	double temp;
	double point2 = 0;
	if (MovingLeft(nowmap, premap)) {
		int n = empty(nowmap);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (nowmap[i][j] == 0) {
					nowmap[i][j] = GetNum();
					point2 = 0;
					temp = MovingUp(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingDown(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingLeft(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingRight(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);
					point += point2;
					smap[i][j] = 0;
				}
			}
		}
		return point/n;
	}
	else
		return -9999;


}

double tryRight(int premap[][4], int nowmap[][4], double point, int layer) {
	layer++;
	double temp;
	double point2 = 0;
	if (MovingRight(nowmap, premap)) {
		int n = empty(nowmap);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (nowmap[i][j] == 0) {
					nowmap[i][j] = GetNum();
					point2 = 0;
					temp = MovingUp(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingDown(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingLeft(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);

					temp = MovingRight(tmap, smap);
					setBmap(Bmap, tmap);
					point2 += getscore(tmap, Bmap);
					point += point2;
					smap[i][j] = 0;
				}
			}
		}
		return point/n;
	}
	else
		return -9999;

}

int main() {
	StartGame(realmap);
	bool temp;
	double score[4];
	int result[7];
	for (int i = 0; i < 7; i++)
		result[i] = 0;
	while (1) {
		score[0] = tryUp(realmap, smap,0,0);
		score[1] = tryDown(realmap, smap, 0, 0);
		score[2] = tryLeft(realmap, smap, 0, 0);
		score[3] = tryRight(realmap, smap, 0, 0);
		int max = 0;
		for (int i = 1; i < 4; i++) {
			if (score[max] < score[i])
				max = i;
		}
		step[times]=max;
		times++;
		if (max == 0) {
			temp = MovingUp(smap, realmap);
			next(realmap,smap);
		}
		else if (max == 1) {
			temp = MovingDown(smap, realmap);
			next(realmap, smap);
		}
		else if (max == 2) {
			temp = MovingLeft(smap, realmap);
			next(realmap, smap);
		}
		else{
			temp = MovingRight(smap, realmap);
			next(realmap, smap);
		}
		AddNum(realmap);

		if (Lose(realmap)) {
			for(int i=0;i<times;i++){
				if(step[i]==0)
					printf("Up\t");
				if(step[i]==1)
					printf("Down\t");
				if(step[i]==2)
					printf("Left\t");
				if(step[i]==3)
					printf("Right\t");
			}
			PrintMap(realmap);
			printf("\ntotal steps: %d\n##########GAME_OVER##########\n\n",times);
			break;
		}
	}
	return 0;
}
