#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "glut.h"
#include <Windows.h>

#undef PI						/* Some systems may have this defined. */
#define PI 3.141592657

#define	L_AXIS	100.0f

#define	SAMPLE_TIME		0.001	// 1ms

struct point
{
	double	x, y, z;
};

struct ipoint
{
	int		x, y, z;
};

extern	GLfloat m_xRotation;
extern	GLfloat	m_yRotation;
extern	GLfloat	m_xRotationOld;
extern	GLfloat m_yRotationOld;
extern	GLfloat	m_Zoom;
extern	GLfloat	m_xTranslation;
extern	GLfloat	m_yTranslation;
extern	GLint	windW, windH;
extern	float	time_stamp;
extern	GLfloat MouseMoveX;
extern	GLfloat MouseMoveY;

extern	GLfloat th1;
extern	GLfloat th2;
extern	GLfloat th3;
extern	GLfloat th4;
extern	GLfloat th5;
extern	GLfloat th6;

void DrawCylinder(GLfloat Radius, GLfloat Height, int color);
void DrawCoordinate(void);
void DrawBox(GLfloat Width, GLfloat Height, GLfloat Depth, int color);
void MDrawBox(GLfloat Width, GLfloat Height, GLfloat Depth, GLfloat Offset);
void AnimateRobot(void);
void DrawFrame(void);

int			set, color;
extern	void CableRobotMotion(void);
extern	void Matching(int s1, int s2);

void	DrawScene(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);

	//initialize color buffer and depth buffer
	glClearColor(0.15f, 0.15f, 0.15f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);

	AnimateRobot();
	glFlush();
}

#define	Radius_base		100.0
#define	Thickness_base	25.0

#define	Radius_link1	50.0
#define	Length_link1	250.0

#define	Radius_link2	50.0
#define	Length_link2	250.0

#define	RED		0
#define	GREEN	1
#define	BLUE	2

#define	STEP	1.0

// Robot Draw Here
void AnimateRobot(void)
{
	static	float	i = 0;

	// Save the matrix state and do the rotations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Translate the whole scene out and into view
	glTranslatef(windW / 2.0, windH / 6.0, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

	// Position / translation / scale : Viewing setup
	//glTranslated(m_xTranslation, m_yTranslation, 0.0);
	glRotatef(m_xRotation, 0.0, 0.0, 1.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);

	glScalef(m_Zoom, m_Zoom, m_Zoom);

	// Ground Mesh in X-Y plane
	glLineWidth((GLfloat)1.0);
	glColor3f(80.0f / 255.0f, 80.0f / 255.0f, 100.0f / 255.0f);
	for (float ci = -windW; ci < windW; ci += 30.0)
	{
		glBegin(GL_LINES);
		glVertex3f(ci, -windH, -10.0f);
		glVertex3f(ci, windH, -10.0f);
		glEnd();
	}
	for (float cj = -windH; cj < windH; cj += 30.0)
	{
		glBegin(GL_LINES);
		glVertex3f(-windW, cj, -10.0f);
		glVertex3f(windW, cj, -10.0f);
		glEnd();
	}

	//	Robot World Coordinate
	glPushMatrix();		// Robot World frame Save
	//	Coordinate
	DrawCoordinate();	// Reference coordinate
	glColor3f(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	//	Robot base
	DrawCylinder((GLfloat)160, (GLfloat)10, 2);
	glTranslatef(0.0, 0.0, (GLfloat)10);
	DrawCylinder((GLfloat)140, (GLfloat)10, 3);

	//link1
	glRotated(-th1, 0.0, 0.0, 1.0); //th1
	glTranslatef(0.0, 0.0, (GLfloat)244.5 / 2);
	DrawBox((GLfloat)120, (GLfloat)60, (GLfloat)244.5, 1);
	glTranslatef(0.0, 0.0, (GLfloat)244.5 / 2);
	glRotated(-90, 1.0, 0.0, 0.0);
	DrawCylinder((GLfloat)60, (GLfloat)60, 1);

	//link2
	glRotated(-th2, 0.0, 0.0, 1.0); //th2
	glTranslatef(0.0, 0.0, (GLfloat)30);
	DrawCylinder((GLfloat)50, (GLfloat)10, 3);
	glTranslatef(0.0, 0.0, (GLfloat)23.5);
	DrawCylinder((GLfloat)25, (GLfloat)37, 2);
	glTranslatef(0.0, 0.0, (GLfloat)38.5);
	DrawCylinder((GLfloat)40, (GLfloat)40, 4);
	glRotated(-90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -(GLfloat)85);
	DrawBox((GLfloat)40, (GLfloat)80, (GLfloat)170, 4);
	glTranslatef(0.0, 0.0, -(GLfloat)85);
	glRotated(90, 0.0, 1.0, 0.0);
	DrawCylinder((GLfloat)40, (GLfloat)40, 4);

	//link3
	glRotated(-th3, 0.0, 0.0, 1.0); //th3
	glTranslatef(0.0, 0.0, (GLfloat)-37);
	DrawCylinder((GLfloat)25, (GLfloat)34, 6);
	glRotated(90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, (GLfloat)85.25);
	DrawBox((GLfloat)50, (GLfloat)34, (GLfloat)170.5, 6);
	glTranslatef(0.0, 0.0, (GLfloat)85.25);

	//linke5
	glRotated(-th4, 0.0, 0.0, 1.0); //th4
	glTranslatef(0.0, 0.0, 5);
	DrawBox((GLfloat)20, (GLfloat)20, (GLfloat)10, 2);
	glTranslatef(0.0, 0.0, 5);
	glRotated(-90, 1.0, 0.0, 0.0);
	DrawCylinder((GLfloat)10, (GLfloat)20, 2);
	glRotated(90, 1.0, 0.0, 0.0);
	glRotated(-90, 1.0, 0.0, 0.0);
	glRotated(-th5, 0.0, 0.0, 1.0); //th5
	glTranslatef(0.0, -15, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	glRotated(-th6, 0.0, 0.0, 1.0); //th6
	DrawCoordinate();
	DrawCylinder((GLfloat)4, (GLfloat)12, 5);
	glTranslatef(0.0, 0.0, 12);
	glRotated(90, 1.0, 0.0, 0.0);
	DrawBox((GLfloat)40, (GLfloat)12, (GLfloat)90, 5);
	glRotated(-90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 24);
	glTranslatef(0.0, 40, 0);
	DrawBox((GLfloat)40, (GLfloat)10, (GLfloat)48, 5);
	glTranslatef(0.0, -80, 0);
	DrawBox((GLfloat)40, (GLfloat)10, (GLfloat)48, 5);
}


void DrawCoordinate(void)
{
	glLineWidth((GLfloat)4.0);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //X-coord, Red
	glVertex3f(L_AXIS, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //Y-coord, Green
	glVertex3f(0.0f, -L_AXIS, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); //Z-coord, Blue
	glVertex3f(0.0f, 0.0f, L_AXIS);
	glEnd();
	glLineWidth((GLfloat)1.0);
}


void DrawCylinder(GLfloat Radius, GLfloat Height, int color)
{
	float angle, dAngle, x, y;
	float normal[3], corners[4][3];

	dAngle = (float)PI / 12.0;

	if (color == 1)
		glColor3f(255.0f / 255.0f, 130.0f / 255.0f, 130.0f / 255.0f);
	if (color == 2)
		glColor3f(128.0f / 255.0f, 128.0f / 255.0f, 255.0f / 255.0f);
	if (color == 3)
		glColor3f(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	if (color == 4)
		glColor3f(0.0f, 128.0f / 255.0f, 0.0f);
	if (color == 5)
		glColor3f(0 / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	if (color == 6)
		glColor3f(128.0f / 255.0f / 2, 128.0f / 255.0f / 2, 128.0f / 255.0f);
	glFrontFace(GL_CCW);

	glBegin(GL_TRIANGLES);
	for (angle = 0; angle <= 2 * PI; angle += dAngle)
	{
		x = Radius * (float)sin(angle);
		y = Radius * (float)cos(angle);

		corners[0][0] = x;
		corners[0][1] = y;
		corners[0][2] = -Height / 2;

		corners[1][0] = x;
		corners[1][1] = y;
		corners[1][2] = Height / 2; // 0

		x = Radius * (float)sin(angle + dAngle);
		y = Radius * (float)cos(angle + dAngle);

		if ((angle + dAngle) < (2 * PI))
		{
			corners[2][0] = x;
			corners[2][1] = y;
			corners[2][2] = Height / 2; // 0

			corners[3][0] = x;
			corners[3][1] = y;
			corners[3][2] = -Height / 2;
		}
		else
		{
			corners[2][0] = 0;
			corners[2][1] = Radius;
			corners[2][2] = Height / 2; // 0

			corners[3][0] = 0;
			corners[3][1] = Radius;
			corners[3][2] = -Height / 2;
		}
		normal[0] = corners[0][0];
		normal[1] = corners[0][1];
		normal[2] = corners[0][2];

		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(corners[0][0], corners[0][1], corners[0][2]);

		normal[0] = corners[1][0];
		normal[1] = corners[1][1];
		normal[2] = corners[1][2];

		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(corners[1][0], corners[1][1], corners[1][2]);

		normal[0] = corners[2][0];
		normal[1] = corners[2][1];
		normal[2] = corners[2][2];

		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(corners[2][0], corners[2][1], corners[2][2]);

		normal[0] = corners[0][0];
		normal[1] = corners[0][1];
		normal[2] = corners[0][2];

		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(corners[0][0], corners[0][1], corners[0][2]);

		normal[0] = corners[2][0];
		normal[1] = corners[2][1];
		normal[2] = corners[2][2];

		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(corners[2][0], corners[2][1], corners[2][2]);

		normal[0] = corners[3][0];
		normal[1] = corners[3][1];
		normal[2] = corners[3][2];

		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(corners[3][0], corners[3][1], corners[3][2]);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -0);


	for (angle = 0; angle < 2 * PI; angle += dAngle)
	{
		x = Radius * (float)sin(angle);
		y = Radius * (float)cos(angle);

		glVertex3f(x, y, -Height / 2); // -Height
	}
	glVertex3f(0.0f, Radius, -Height / 2); // -Height
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -Height / 2); // -Height

	for (angle = 0; angle > -2 * PI; angle -= dAngle)
	{
		x = Radius * (float)sin(angle);
		y = Radius * (float)cos(angle);

		glVertex3f(x, y, Height / 2); // 0
	}
	glVertex3f(0.0f, Radius, Height / 2); // 0
	glEnd();
}


void DrawBox(GLfloat Width, GLfloat Height, GLfloat Depth, int color)
{
	if (color == 1)
		glColor3f(255.0f / 255.0f, 130.0f / 255.0f, 130.0f / 255.0f);
	if (color == 2)
		glColor3f(128.0f / 255.0f, 128.0f / 255.0f, 255.0f / 255.0f);
	if (color == 3)
		glColor3f(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	if (color == 4)
		glColor3f(0.0f, 128.0f / 255.0f, 0.0f);
	if (color == 5)
		glColor3f(0 / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	if (color == 6)
		glColor3f(128.0f / 255.0f / 2, 128.0f / 255.0f / 2, 128.0f / 255.0f);

	// +X Plane
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(Width / 2, Height / 2, -Depth / 2);
	glVertex3f(Width / 2, Height / 2, Depth / 2);
	glVertex3f(Width / 2, -Height / 2, Depth / 2);
	glVertex3f(Width / 2, -Height / 2, -Depth / 2);
	glEnd();

	// -X Plane
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-Width / 2, -Height / 2, -Depth / 2);
	glVertex3f(-Width / 2, -Height / 2, Depth / 2);
	glVertex3f(-Width / 2, Height / 2, Depth / 2);
	glVertex3f(-Width / 2, Height / 2, -Depth / 2);
	glEnd();

	// +Y Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-Width / 2, Height / 2, -Depth / 2);
	glVertex3f(-Width / 2, Height / 2, Depth / 2);
	glVertex3f(Width / 2, Height / 2, Depth / 2);
	glVertex3f(Width / 2, Height / 2, -Depth / 2);
	glEnd();

	// -Y Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(Width / 2, -Height / 2, -Depth / 2);
	glVertex3f(Width / 2, -Height / 2, Depth / 2);
	glVertex3f(-Width / 2, -Height / 2, Depth / 2);
	glVertex3f(-Width / 2, -Height / 2, -Depth / 2);
	glEnd();

	// +Z Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(Width / 2, Height / 2, Depth / 2);
	glVertex3f(-Width / 2, Height / 2, Depth / 2);
	glVertex3f(-Width / 2, -Height / 2, Depth / 2);
	glVertex3f(Width / 2, -Height / 2, Depth / 2);
	glEnd();

	// -Z Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(Width / 2, -Height / 2, -Depth / 2);
	glVertex3f(-Width / 2, -Height / 2, -Depth / 2);
	glVertex3f(-Width / 2, Height / 2, -Depth / 2);
	glVertex3f(Width / 2, Height / 2, -Depth / 2);
	glEnd();
}


void MDrawBox(GLfloat Width, GLfloat Height, GLfloat Depth, GLfloat Offset)
{
	glColor3f(130.0f / 255.0f, 130.0f / 255.0f, 255.0f / 255.0f);

	// +X Plane
	glBegin(GL_POLYGON);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(Width - Offset, Height / 2, -Depth / 2);
	glVertex3f(Width - Offset, Height / 2, Depth / 2);
	glVertex3f(Width - Offset, -Height / 2, Depth / 2);
	glVertex3f(Width - Offset, -Height / 2, -Depth / 2);
	glEnd();

	// -X Plane
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-Offset, -Height / 2, -Depth / 2);
	glVertex3f(-Offset, -Height / 2, Depth / 2);
	glVertex3f(-Offset, Height / 2, Depth / 2);
	glVertex3f(-Offset, Height / 2, -Depth / 2);
	glEnd();

	// +Y Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-Offset, Height / 2, -Depth / 2);
	glVertex3f(-Offset, Height / 2, Depth / 2);
	glVertex3f(Width - Offset, Height / 2, Depth / 2);
	glVertex3f(Width - Offset, Height / 2, -Depth / 2);
	glEnd();

	// -Y Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-Offset, -Height / 2, -Depth / 2);
	glVertex3f(Width - Offset, -Height / 2, -Depth / 2);
	glVertex3f(Width - Offset, -Height / 2, Depth / 2);
	glVertex3f(-Offset, -Height / 2, Depth / 2);
	glEnd();

	// +Z Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(Width - Offset, Height / 2, Depth / 2);
	glVertex3f(-Offset, Height / 2, Depth / 2);
	glVertex3f(-Offset, -Height / 2, Depth / 2);
	glVertex3f(Width - Offset, -Height / 2, Depth / 2);
	glEnd();

	// -Z Plane
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(Width - Offset, -Height / 2, -Depth / 2);
	glVertex3f(-Offset, -Height / 2, -Depth / 2);
	glVertex3f(-Offset, Height / 2, -Depth / 2);
	glVertex3f(Width - Offset, Height / 2, -Depth / 2);
	glEnd();
}