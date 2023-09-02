#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <C:\Users\Yisooyeong\Desktop\glutdlls\glut.h>
#include <Windows.h>
#include <conio.h>

#define	DEG2RAD	0.017453293
#define	RAD2DEG	57.2957795

#define	SAMPLE_TIME		0.01	// 현재는 의미없는 값이다.

struct point
{
	double	x, y, z;
};

struct ipoint
{
	int		x, y, z;
};

#define	MAX_POINTS	10000
#define	MAX_SETS	19

struct Point_Set
{
	struct ipoint	ipt[MAX_POINTS];
	int				no_pt;
};

extern	struct Point_Set	Pset[MAX_SETS];
extern	int					no_set;

void	CableRobotMotion(void);
extern	float	time_stamp;
extern	int		key_flag;
void	Matching(int s1, int s2);
void	centroid(int set, struct ipoint *cent);

void gotoxy(int x, int y)
{
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

//	매 주기마다 반복되는 함수(애니메이션)
int	set, color = 0;

void	CableRobotMotion(void)
{
	time_stamp += SAMPLE_TIME;

	if(key_flag != NULL)
	{
		key_flag = NULL;
		system("cls");
		gotoxy(0, 0);
		//printf("Key command: (1,2 Px), (3,4 Py), (5,6 Pz), (7,8 Ry), (9,0 Rx), (a,s Rz)\n\n");
		printf("Point Cloud Set to Draw?\n\n");

		printf("Draw Set= %d\n", set);
	}

	return;
}

//struct Point_Set
//{
//	struct ipoint	ipt[MAX_POINTS];
//	int				no_pt;
//};

//	Data set s1(reference)과 s2를 매칭
void Matching(int s1, int s2)
{
	struct ipoint	cent;

	centroid(s2, &cent);
}

//	매칭을 위한 변환의 중심점을 구한다.
void	centroid(int set, struct ipoint *cent)
{
	int	z_mean;
	int	sum_x= 0, sum_y= 0;
	int	i;

	z_mean = 100;	// max height와 min height의 중간(2000*0.1(DOWN_SCALE))

	for (i = 0; i < Pset[set].no_pt; i++)
	{
		sum_x += Pset[set].ipt[i].x;
		sum_y += Pset[set].ipt[i].y;
	}

	cent->x = (int)(sum_x / Pset[set].no_pt);
	cent->y = (int)(sum_y / Pset[set].no_pt);
	cent->z = z_mean;
}
