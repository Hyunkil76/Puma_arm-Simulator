#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "glut.h"
#include <Windows.h>

#define	DEG2RAD	0.017453293
#define	RAD2DEG	57.29577951
#undef PI						/* Some systems may have this defined. */
#define PI 3.141592657

#define	SAMPLE_TIME		0.1
float	time_stamp = 0;

GLfloat m_xRotation = 70.0f;
GLfloat	m_yRotation = 15.0f;
GLfloat	m_xRotationOld = 70.0f;
GLfloat m_yRotationOld = 15.0f;
GLfloat	m_Zoom = 0.5f;
GLfloat m_ZoomOld = 0.5f;
GLfloat	m_xTranslation = 0.0f;
GLfloat	m_yTranslation = 0.0f;
GLfloat MousePositionX = 0, MousePositionY = 0;		// Mouse click 위치
GLfloat MouseMoveX = 0, MouseMoveY = 0;				// Mouse 이동량

GLfloat th1 = 0;
GLfloat th2 = 0;
GLfloat th3 = -90;
GLfloat th4 = 0;
GLfloat th5 = 0;
GLfloat th6 = 0;

double l1 = 264.5, l2 = 92, l3 = 170, l4 = 37, l5 = 170.5, l6 = 22;
double px = 362.5, py = -55, pz = 264.5;
double ax = 1, ay = 0, az = 0;
double ox = 0, oy = 1, oz = 0;
double nx = 0, ny = 0, nz = -1;

double d_th1;
double d_th2;
double d_th3;
double d_th4;
double d_th5;
double d_th6;
int is_up = 100;

int		MouseScaleFlag = 0;
int		MouseViewPointFlag = 0;

extern	void	DrawScene(void);

struct point
{
	double	x, y, z;
};

struct ipoint
{
	int		x, y, z;
};

GLenum	doubleBuffer;
GLint	windW = 800, windH = 800;

int dim = 4;

struct st
{
	double m[10][10] = {};
	st operator*(st a) const
	{
		st ret;
		for (int i = 0; i < dim; i++)
			for (int j = 0; j < dim; j++)
				for (int k = 0; k < dim; k++)
					ret.m[i][j] = (ret.m[i][j] + m[i][k] * a.m[k][j]);
		return ret;
	}
}R01, R12, R23, R34, R45, R56, R06;

void timer(int value)
{
	if (is_up < 100)
	{
		th1 = th1 + d_th1;
		th2 = th2 + d_th2;
		th3 = th3 + d_th3;
		th4 = th4 + d_th4;
		th5 = th5 + d_th5;
		th6 = th6 + d_th6;
		is_up++;
		printf("%lf %lf %lf %lf %lf %lf \n", th1, th2, th3, th4, th5, th6);
	}

	glutPostRedisplay();
	glutTimerFunc(10, timer, 1);
}

void	Reshape(int width, int height)
{
	windW = width;
	windH = height;

	glViewport(0, 0, windW, windH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(-0.5, windW + 0.5, -0.5, windH + 0.5);			// 2D projection
	glOrtho(-0.5, windW + 0.5, -0.5, windH + 0.5, 1000, -1000);	// 3D
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glutPostRedisplay();//화면 갱신
}


int	key_flag = 1;
extern	int	set, color;

/* ARGSUSED1 : 그래픽 화면에서 키를 입력했을 때 호출되는 함수 */
static void	Key(unsigned char key, int x, int y)
{
	//printf("%d, %d, %d\n", key, x, y);		// my code
	static int	keys[3] = { 0, 0, 0 };
	static int i = 0;
	int mode_s = 0;

	key_flag = 1;

	keys[i++] = key - 48;	// 48은 '0' 키를 의미함

	if (key == 13)			// 13은 Enter 키를 의미함
	{
		if (i == 3)			set = keys[0] * 10 + keys[1];
		else if (i == 2)	set = keys[0];

		//printf("set= %d\n", set);
		keys[0] = keys[1] = keys[2] = 0;
		i = 0;
	}


	switch (key) {
	case 'e':
		px = px + 5;
		mode_s = 1;
		break;
	case 'q':
		px = px - 5;
		mode_s = 1;
		break;
	case 'd':
		py = py + 5;
		mode_s = 1;
		break;
	case 'a':
		py = py - 5;
		mode_s = 1;
		break;
	case 'w':
		pz = pz + 5;
		mode_s = 1;
		break;
	case 's':
		pz = pz - 5;
		mode_s = 1;
		break;
	case 'z':
		th4 = th4 + 5;
		mode_s = 2;
		break;
	case 'x':
		th4 = th4 - 5;
		mode_s = 2;
		break;
	case 'c':
		th5 = th5 + 5;
		mode_s = 2;
		break;
	case 'v':
		th5 = th5 - 5;
		mode_s = 2;
		break;
	case 'b':
		th6 = th6 + 5;
		mode_s = 2;
		break;
	case 'n':
		th6 = th6 - 5;
		mode_s = 2;
		break;
	case 'i':
		mode_s = 3;
		break;
	case 'o':
		mode_s = 4;
		break;
	case 'p':
		mode_s = 5;
		break;
	case 27:
		exit(0);
	}

	if (mode_s == 1)
	{
		double pxb = px - ax * l6;
		double pyb = py - ay * l6;
		double pzb = pz - az * l6;

		double c_th1 = atan2((pyb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) + pxb * (l2 - l4)), (pxb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) - pyb * (l2 - l4)));

		double c_th3 = atan2(-(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2)), sqrt(4 * pow(l5, 2) * pow(l3, 2) - pow(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2), 2)));

		double a = pzb - l1, b = sqrt(pow(pxb, 2) + pow(pyb, 2) - pow(l2 - l4, 2)), c = l5 * cos(c_th3), d = l3 - l5 * sin(c_th3);
		double c_th2 = atan2(a * (pow(c, 2) + pow(d, 2)) - c * (a * c + b * d), d * (a * c + b * d));

		double z3x = -cos(c_th1) * sin(c_th2 + c_th3);
		double z3y = -sin(c_th1) * sin(c_th2 + c_th3);
		double z3z = cos(c_th2 + c_th3);

		double c_th4 = atan2(sin(c_th1) * ax - cos(c_th1) * ay, -(sin(c_th2 + c_th3) * az + sin(c_th1) * cos(c_th2 + c_th3) * ay + cos(c_th1) * cos(c_th2 + c_th3) * ax));

		double a_y4 = -(ax * (cos(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) - sin(c_th1) * sin(c_th4)) + ay * (sin(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) + cos(c_th1) * sin(c_th4)) + az * sin(c_th2 + c_th3) * cos(c_th4));
		double a_x4 = -ax * cos(c_th1) * sin(c_th2 + c_th3) - ay * sin(c_th1) * sin(c_th2 + c_th3) + az * cos(c_th2 + c_th3);
		double c_th5 = atan2(a_y4, a_x4);

		double y5_n = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * nx - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * ny - (sin(c_th2 + c_th3) * sin(c_th4)) * nz;
		double y5_o = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * ox - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * oy - (sin(c_th2 + c_th3) * sin(c_th4)) * oz;
		double c_th6 = atan2(y5_n, y5_o);
		th1 = c_th1 * 180 / PI;
		th2 = c_th2 * 180 / PI;
		th3 = c_th3 * 180 / PI;
		th4 = c_th4 * 180 / PI;
		th5 = c_th5 * 180 / PI;
		th6 = c_th6 * 180 / PI;
		printf("px : %f py : %f pz: %f\n", px, py, pz);
		printf("%f %f %f %f %f %f\n", th1, th2, th3, th4, th5, th6);
	}

	else if (mode_s == 2)
	{
		double c_th1 = th1 * PI / 180;
		double c_th2 = th2 * PI / 180;
		double c_th3 = th3 * PI / 180;
		double c_th4 = th4 * PI / 180;
		double c_th5 = th5 * PI / 180;
		double c_th6 = th6 * PI / 180;

		R01.m[0][0] = cos(c_th1); R01.m[0][1] = 0;         R01.m[0][2] = sin(c_th1);  R01.m[0][3] = 0;
		R01.m[1][0] = sin(c_th1); R01.m[1][1] = 0;         R01.m[1][2] = -cos(c_th1); R01.m[1][3] = 0;
		R01.m[2][0] = 0;          R01.m[2][1] = 1;         R01.m[2][2] = 0;           R01.m[2][3] = l1;
		R01.m[3][0] = 0;          R01.m[3][1] = 0;         R01.m[3][2] = 0;           R01.m[3][3] = 1;

		R12.m[0][0] = cos(c_th2); R12.m[0][1] = -sin(c_th2); R12.m[0][2] = 0;         R12.m[0][3] = l3 * cos(c_th2);
		R12.m[1][0] = sin(c_th2); R12.m[1][1] = cos(c_th2);  R12.m[1][2] = 0;         R12.m[1][3] = l3 * sin(c_th2);
		R12.m[2][0] = 0;          R12.m[2][1] = 0;           R12.m[2][2] = 1;         R12.m[2][3] = l2;
		R12.m[3][0] = 0;          R12.m[3][1] = 0;           R12.m[3][2] = 0;         R12.m[3][3] = 1;

		R23.m[0][0] = cos(c_th3); R23.m[0][1] = 0;         R23.m[0][2] = -sin(c_th3); R23.m[0][3] = 0;
		R23.m[1][0] = sin(c_th3); R23.m[1][1] = 0;         R23.m[1][2] = cos(c_th3);  R23.m[1][3] = 0;
		R23.m[2][0] = 0;          R23.m[2][1] = -1;        R23.m[2][2] = 0;           R23.m[2][3] = -l4;
		R23.m[3][0] = 0;          R23.m[3][1] = 0;         R23.m[3][2] = 0;           R23.m[3][3] = 1;

		R34.m[0][0] = cos(c_th4); R34.m[0][1] = 0;         R34.m[0][2] = sin(c_th4);  R34.m[0][3] = 0;
		R34.m[1][0] = sin(c_th4); R34.m[1][1] = 0;         R34.m[1][2] = -cos(c_th4); R34.m[1][3] = 0;
		R34.m[2][0] = 0;          R34.m[2][1] = 1;         R34.m[2][2] = 0;           R34.m[2][3] = l5;
		R34.m[3][0] = 0;          R34.m[3][1] = 0;         R34.m[3][2] = 0;           R34.m[3][3] = 1;

		R45.m[0][0] = cos(c_th5); R45.m[0][1] = 0;         R45.m[0][2] = -sin(c_th5); R45.m[0][3] = 0;
		R45.m[1][0] = sin(c_th5); R45.m[1][1] = 0;         R45.m[1][2] = cos(c_th5);  R45.m[1][3] = 0;
		R45.m[2][0] = 0;          R45.m[2][1] = -1;        R45.m[2][2] = 0;           R45.m[2][3] = 0;
		R45.m[3][0] = 0;          R45.m[3][1] = 0;         R45.m[3][2] = 0;           R45.m[3][3] = 1;

		R56.m[0][0] = cos(c_th6); R56.m[0][1] = -sin(c_th6);  R56.m[0][2] = 0;        R56.m[0][3] = 0;
		R56.m[1][0] = sin(c_th6); R56.m[1][1] = cos(c_th6);   R56.m[1][2] = 0;        R56.m[1][3] = 0;
		R56.m[2][0] = 0;          R56.m[2][1] = 0;            R56.m[2][2] = 1;        R56.m[2][3] = l6;
		R56.m[3][0] = 0;          R56.m[3][1] = 0;            R56.m[3][2] = 0;        R56.m[3][3] = 1;

		R06 = R01 * R12 * R23 * R34 * R45 * R56;

		px = R06.m[0][3], py = R06.m[1][3], pz = R06.m[2][3];
		ax = R06.m[0][2], ay = R06.m[1][2], az = R06.m[2][2];
		ox = R06.m[0][1], oy = R06.m[1][1], oz = R06.m[2][1];
		nx = R06.m[0][0], ny = R06.m[1][0], nz = R06.m[2][0];

		printf("ax : %f ay : %f az : %f\n", ax, ay, az);
		printf("ox : %f oy : %f oz : %f\n", ox, oy, oz);
		printf("nx : %f ny : %f nz : %f\n", nx, ny, nz);
	}

	else if (mode_s == 3)
	{
		printf("Insert X Y Z: ");
		scanf_s("%lf %lf %lf", &px, &py, &pz);
		double e_th1 = th1;
		double e_th2 = th2;
		double e_th3 = th3;
		double e_th4 = th4;
		double e_th5 = th5;
		double e_th6 = th6;

		double pxb = px - ax * l6;
		double pyb = py - ay * l6;
		double pzb = pz - az * l6;

		double c_th1 = atan2((pyb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) + pxb * (l2 - l4)), (pxb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) - pyb * (l2 - l4)));

		double c_th3 = atan2(-(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2)), sqrt(4 * pow(l5, 2) * pow(l3, 2) - pow(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2), 2)));

		double a = pzb - l1, b = sqrt(pow(pxb, 2) + pow(pyb, 2) - pow(l2 - l4, 2)), c = l5 * cos(c_th3), d = l3 - l5 * sin(c_th3);
		double c_th2 = atan2(a * (pow(c, 2) + pow(d, 2)) - c * (a * c + b * d), d * (a * c + b * d));

		double z3x = -cos(c_th1) * sin(c_th2 + c_th3);
		double z3y = -sin(c_th1) * sin(c_th2 + c_th3);
		double z3z = cos(c_th2 + c_th3);

		double c_th4 = atan2(sin(c_th1) * ax - cos(c_th1) * ay, -(sin(c_th2 + c_th3) * az + sin(c_th1) * cos(c_th2 + c_th3) * ay + cos(c_th1) * cos(c_th2 + c_th3) * ax));

		double a_y4 = -(ax * (cos(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) - sin(c_th1) * sin(c_th4)) + ay * (sin(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) + cos(c_th1) * sin(c_th4)) + az * sin(c_th2 + c_th3) * cos(c_th4));
		double a_x4 = -ax * cos(c_th1) * sin(c_th2 + c_th3) - ay * sin(c_th1) * sin(c_th2 + c_th3) + az * cos(c_th2 + c_th3);
		double c_th5 = atan2(a_y4, a_x4);

		double y5_n = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * nx - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * ny - (sin(c_th2 + c_th3) * sin(c_th4)) * nz;
		double y5_o = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * ox - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * oy - (sin(c_th2 + c_th3) * sin(c_th4)) * oz;
		double c_th6 = atan2(y5_n, y5_o);

		d_th1 = (c_th1 * 180 / PI - e_th1) / 100;
		d_th2 = (c_th2 * 180 / PI - e_th2) / 100;
		d_th3 = (c_th3 * 180 / PI - e_th3) / 100;
		d_th4 = (c_th4 * 180 / PI - e_th4) / 100;
		d_th5 = (c_th5 * 180 / PI - e_th5) / 100;
		d_th6 = (c_th6 * 180 / PI - e_th6) / 100;
		is_up = 0;
	}

	else if (mode_s == 4)
	{
		printf("Insert ax ay az : ");
		scanf_s("%lf %lf %lf", &ax, &ay, &az);
		printf("Insert ox oy oz : ");
		scanf_s("%lf %lf %lf", &ox, &oy, &oz);
		printf("Insert nx ny nz : ");
		scanf_s("%lf %lf %lf", &nx, &ny, &nz);

		double e_th1 = th1;
		double e_th2 = th2;
		double e_th3 = th3;
		double e_th4 = th4;
		double e_th5 = th5;
		double e_th6 = th6;

		double pxb = px - ax * l6;
		double pyb = py - ay * l6;
		double pzb = pz - az * l6;

		double c_th1 = atan2((pyb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) + pxb * (l2 - l4)), (pxb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) - pyb * (l2 - l4)));

		double c_th3 = atan2(-(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2)), sqrt(4 * pow(l5, 2) * pow(l3, 2) - pow(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2), 2)));

		double a = pzb - l1, b = sqrt(pow(pxb, 2) + pow(pyb, 2) - pow(l2 - l4, 2)), c = l5 * cos(c_th3), d = l3 - l5 * sin(c_th3);
		double c_th2 = atan2(a * (pow(c, 2) + pow(d, 2)) - c * (a * c + b * d), d * (a * c + b * d));

		double z3x = -cos(c_th1) * sin(c_th2 + c_th3);
		double z3y = -sin(c_th1) * sin(c_th2 + c_th3);
		double z3z = cos(c_th2 + c_th3);

		double c_th4 = atan2(sin(c_th1) * ax - cos(c_th1) * ay, -(sin(c_th2 + c_th3) * az + sin(c_th1) * cos(c_th2 + c_th3) * ay + cos(c_th1) * cos(c_th2 + c_th3) * ax));

		double a_y4 = -(ax * (cos(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) - sin(c_th1) * sin(c_th4)) + ay * (sin(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) + cos(c_th1) * sin(c_th4)) + az * sin(c_th2 + c_th3) * cos(c_th4));
		double a_x4 = -ax * cos(c_th1) * sin(c_th2 + c_th3) - ay * sin(c_th1) * sin(c_th2 + c_th3) + az * cos(c_th2 + c_th3);
		double c_th5 = atan2(a_y4, a_x4);

		double y5_n = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * nx - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * ny - (sin(c_th2 + c_th3) * sin(c_th4)) * nz;
		double y5_o = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * ox - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * oy - (sin(c_th2 + c_th3) * sin(c_th4)) * oz;
		double c_th6 = atan2(y5_n, y5_o);

		d_th1 = (c_th1 * 180 / PI - e_th1) / 100;
		d_th2 = (c_th2 * 180 / PI - e_th2) / 100;
		d_th3 = (c_th3 * 180 / PI - e_th3) / 100;
		d_th4 = (c_th4 * 180 / PI - e_th4) / 100;
		d_th5 = (c_th5 * 180 / PI - e_th5) / 100;
		d_th6 = (c_th6 * 180 / PI - e_th6) / 100;
		is_up = 0;
	}

	else if (mode_s == 5)
	{
		printf("Insert X Y Z: ");
		scanf_s("%lf %lf %lf", &px, &py, &pz);
		printf("Insert ax ay az : ");
		scanf_s("%lf %lf %lf", &ax, &ay, &az);
		printf("Insert ox oy oz : ");
		scanf_s("%lf %lf %lf", &ox, &oy, &oz);
		printf("Insert nx ny nz : ");
		scanf_s("%lf %lf %lf", &nx, &ny, &nz);

		double e_th1 = th1;
		double e_th2 = th2;
		double e_th3 = th3;
		double e_th4 = th4;
		double e_th5 = th5;
		double e_th6 = th6;

		double pxb = px - ax * l6;
		double pyb = py - ay * l6;
		double pzb = pz - az * l6;

		double c_th1 = atan2((pyb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) + pxb * (l2 - l4)), (pxb * sqrt(pow(pxb, 2) + pow(pyb, 2) - pow((l2 - l4), 2)) - pyb * (l2 - l4)));

		double c_th3 = atan2(-(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2)), sqrt(4 * pow(l5, 2) * pow(l3, 2) - pow(pow(pxb, 2) + pow(pyb, 2) + pow(pzb - l1, 2) - pow(l5, 2) - pow(l3, 2) - pow(l2 - l4, 2), 2)));

		double a = pzb - l1, b = sqrt(pow(pxb, 2) + pow(pyb, 2) - pow(l2 - l4, 2)), c = l5 * cos(c_th3), d = l3 - l5 * sin(c_th3);
		double c_th2 = atan2(a * (pow(c, 2) + pow(d, 2)) - c * (a * c + b * d), d * (a * c + b * d));

		double z3x = -cos(c_th1) * sin(c_th2 + c_th3);
		double z3y = -sin(c_th1) * sin(c_th2 + c_th3);
		double z3z = cos(c_th2 + c_th3);

		double c_th4 = atan2(sin(c_th1) * ax - cos(c_th1) * ay, -(sin(c_th2 + c_th3) * az + sin(c_th1) * cos(c_th2 + c_th3) * ay + cos(c_th1) * cos(c_th2 + c_th3) * ax));

		double a_y4 = -(ax * (cos(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) - sin(c_th1) * sin(c_th4)) + ay * (sin(c_th1) * cos(c_th2 + c_th3) * cos(c_th4) + cos(c_th1) * sin(c_th4)) + az * sin(c_th2 + c_th3) * cos(c_th4));
		double a_x4 = -ax * cos(c_th1) * sin(c_th2 + c_th3) - ay * sin(c_th1) * sin(c_th2 + c_th3) + az * cos(c_th2 + c_th3);
		double c_th5 = atan2(a_y4, a_x4);

		double y5_n = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * nx - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * ny - (sin(c_th2 + c_th3) * sin(c_th4)) * nz;
		double y5_o = -(cos(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) + sin(c_th1) * cos(c_th4)) * ox - (sin(c_th1) * cos(c_th2 + c_th3) * sin(c_th4) - cos(c_th1) * cos(c_th4)) * oy - (sin(c_th2 + c_th3) * sin(c_th4)) * oz;
		double c_th6 = atan2(y5_n, y5_o);

		d_th1 = (c_th1 * 180 / PI - e_th1) / 100;
		d_th2 = (c_th2 * 180 / PI - e_th2) / 100;
		d_th3 = (c_th3 * 180 / PI - e_th3) / 100;
		d_th4 = (c_th4 * 180 / PI - e_th4) / 100;
		d_th5 = (c_th5 * 180 / PI - e_th5) / 100;
		d_th6 = (c_th6 * 180 / PI - e_th6) / 100;
		is_up = 0;
	}

}

void	Idle(void)
{
	glutPostRedisplay();
}

void	Display(void)
{
	DrawScene();

	if (doubleBuffer) {
		glutSwapBuffers();
	}
	else {
		glFlush();
	}
}


void	Visible(int state)
{
	if (state == GLUT_VISIBLE) {
		glutIdleFunc(Idle);
	}
	else {
		glutIdleFunc(NULL);
	}
}


static void	Args(int argc, char** argv)
{
	GLint i;

	doubleBuffer = GL_TRUE;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-sb") == 0) {
			doubleBuffer = GL_FALSE;
		}
		else if (strcmp(argv[i], "-db") == 0) {
			doubleBuffer = GL_TRUE;
		}
	}
}


void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y)
{
	GLfloat fX = X;
	GLfloat fY = windH - Y;	//GL의 Y축은 증가가 윈도우의 반대라서

	//화면의 정가운데의 좌표가 (0,0)이고, 크기가 가로가 1 세로가 1이므로
	if (fX > (windW / 2.0))		//화면의 절반을 마우스가 넘어갔다면
	{
		fX = fX - (windW / 2.0);//넘어간 만큼을 계산
		fX = fX / (windW / 2.0);//마우스 좌표를 화면 죄표로 정규화
	}
	else
	{
		fX = (windW / 2.0) - fX;
		fX = -fX / (windW / 2.0);
	}

	if (fY > (windH / 2.0))
	{
		fY = fY - (windH / 2.0);
		fY = fY / (windH / 2.0);
	}
	else
	{
		fY = (windH / 2.0) - fY;
		fY = -fY / (windH / 2.0);
	}

	MousePositionX = fX;
	MousePositionY = fY;
	//printf("MousePosition: %f, %f\n", MousePositionX, MousePositionY);

	if (State == GLUT_DOWN)
	{
		if (Button == GLUT_RIGHT_BUTTON)
			MouseScaleFlag = 1;
		if (Button == GLUT_LEFT_BUTTON)
			MouseViewPointFlag = 1;
	}

	if (State == GLUT_UP)
	{
		if (MouseViewPointFlag == 1)
		{
			m_xRotationOld = m_xRotation;
			m_yRotationOld = m_yRotation;
		}
		if (MouseScaleFlag == 1)
		{
			m_ZoomOld = m_Zoom;
		}

		MouseScaleFlag = 0;
		MouseViewPointFlag = 0;
		MouseMoveX = MouseMoveY = 0;
	}

	//glutPostRedisplay();//화면 갱신
}


void MyMouseMove(int X, int Y)
{
	GLfloat fX = X;
	GLfloat fY = windH - Y;	// GL의 Y축은 증가가 윈도우의 반대라서

	//화면의 정가운데의 좌표가 (0,0)이고, 크기가 가로가 1 세로가 1이므로
	if (fX > (windW / 2.0))		//화면의 절반을 마우스가 넘어갔다면
	{
		fX = fX - (windW / 2.0);//넘어간 만큼을 계산
		fX = fX / (windW / 2.0);//마우스 좌표를 화면 죄표로 정규화
	}
	else
	{
		fX = (windW / 2.0) - fX;
		fX = -fX / (windW / 2.0);
	}

	if (fY > (windH / 2.0))
	{
		fY = fY - (windH / 2.0);
		fY = fY / (windH / 2.0);
	}
	else
	{
		fY = (windH / 2.0) - fY;
		fY = -fY / (windH / 2.0);
	}

	MouseMoveX = fX - MousePositionX;
	MouseMoveY = fY - MousePositionY;	// Mouse 이동량
	//printf("mouse move: x= %f, y= %f\n", MouseMoveX, MouseMoveY);

	if (MouseViewPointFlag == 1)
	{
		m_xRotation = m_xRotationOld + MouseMoveX * 90.0;
		m_yRotation = m_yRotationOld + MouseMoveY * 90.0;
		//printf("Rotation: %f %f\n", m_xRotation, m_yRotation);
	}
	if (MouseScaleFlag == 1)
	{
		m_Zoom = m_ZoomOld + 0.5 * (MouseMoveX + MouseMoveY);
		//printf("Zoom: %f\n", m_Zoom, m_yRotation);
	}
}

#define	MAX_POINTS		110000
#define	MAX_SETS		27
#define	MIN_STRENGTH	50

#define	DOWN_SCALE	0.5


int	main(int argc, char** argv)
{
	int		i = 0;
	char	file_name[100];
	GLenum	type;

	glutInitWindowSize(windW, windH);
	glutInit(&argc, argv);
	Args(argc, argv);

	type = GLUT_RGB | GLUT_DEPTH;
	type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;
	glutInitDisplayMode(type);
	//glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutCreateWindow("Robot Manipulator");

	glEnable(GL_DEPTH_TEST);

	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);
	glutVisibilityFunc(Visible);
	glutMouseFunc(MyMouseClick);
	glutMotionFunc(MyMouseMove);
	glutDisplayFunc(Display);
	glutTimerFunc(0, timer, 1);
	glutMainLoop();

	return 0;             /* ANSI C requires main to return int. */
}