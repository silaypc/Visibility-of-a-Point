

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


/* User defined Types and variables for the application */
#define FOV  60    // Field of View Angle

//define structure for object A
typedef struct {
	vec_t pos;
	float angle;
} obj_t;

//define structure for object B
typedef struct {
	vec_t pos;
	bool  visibility; // visibility: true if the player sees the enemy
} point_t;

obj_t A = { { 100, 240 }, 247.6 };
point_t B = { { 70, 280 } };



//
// to draw circle, center at (x,y)
//  radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}


// display text with variables.
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void drawA(obj_t p) {
	vec_t vEnd = addV(p.pos, pol2rec({ 150, p.angle }));
	vec_t E1 = addV(p.pos, pol2rec({ 300, p.angle - FOV }));
	vec_t E2 = addV(p.pos, pol2rec({ 300, p.angle + FOV }));

	// Field of View Area
	if(B.visibility)
	glColor4f(0, 1, 0, 0.2);
	else glColor4f(1, 1, 0, 0.2);
	glBegin(GL_TRIANGLES);
	glVertex2f(p.pos.x, p.pos.y);
	glVertex2f(E1.x, E1.y);
	glVertex2f(E2.x, E2.y);
	glEnd();

	// View Direction Indicator
	glColor4f(0, 0, 0, 0.3);
	glBegin(GL_LINES);
	glVertex2f(p.pos.x, p.pos.y);
	glVertex2f(vEnd.x, vEnd.y);
	glEnd();

	// Body of A.
	glColor4f(0, 0.5, 1, 1);
	circle(p.pos.x, p.pos.y, 5);
	vprint(96, 248, GLUT_BITMAP_HELVETICA_18, "A");
}

void drawB(point_t b) {

	glColor3f(1, 1, 1);
	circle_wire(70, 280, 5);

	// if A sees B, its color is green.
	if (B.visibility) glColor3f(0, 0.70, 0.60);
	else glColor3f(1, 0, 0);
	vprint(50, 280, GLUT_BITMAP_HELVETICA_18, "B");
	circle(70, 280, 4);
}

void display()
{

	//
	// clear window to black
	//
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	drawA(A);
	drawB(B);
	glColor3f(0, 0.5, 1);
	vprint(110, 236, GLUT_BITMAP_HELVETICA_12, "%.0lf", A.angle);
	vprint(115, 255, GLUT_BITMAP_HELVETICA_12, "(100, 240)");
	if (B.visibility) {
		glColor3f(0, 0.7, 0.5);
		vprint(-270, -270, GLUT_BITMAP_HELVETICA_18, "B is visible");
		vprint(20, 265, GLUT_BITMAP_HELVETICA_12, "(70, 280)");
	}
	else {
		glColor3f(1, 0, 0);
		vprint(-270, -270, GLUT_BITMAP_HELVETICA_18, "B is not visible");
		vprint(20,265, GLUT_BITMAP_HELVETICA_12, "(70, 280)");
	}
	glColor3ub(50, 50, 50);
	vprint(-120, -289, GLUT_BITMAP_HELVETICA_12, "The direction angle of the vector V = <-50, -120> is 247.6");
	int a;
	for (a = -240; a <= 240; a += 30)
	{
		glColor3ub(50, 50, 50);
		glBegin(GL_LINES);
		glVertex2f(a, 5);
		glVertex2f(a, -5);
		glVertex2f(5, a);
		glVertex2f(-5, a);
		glEnd();
		if (a > 0)
		{
			vprint(-27, a - 3, GLUT_BITMAP_HELVETICA_12, "%d", a);
			vprint(a - 6, 12, GLUT_BITMAP_HELVETICA_12, "%d", a);
		}
		else if (a < 0){
			vprint(12, a - 3, GLUT_BITMAP_HELVETICA_12, "%d", a);
			vprint(a - 13, -20, GLUT_BITMAP_HELVETICA_12, "%d", a);
		}
	}
	vprint(4, 7, GLUT_BITMAP_HELVETICA_12, "0");
	vprint(-280, 284, GLUT_BITMAP_HELVETICA_12, "Press [left] or [right] arrow keys to change angle");

	glBegin(GL_LINES);
	glVertex2f(0, 270);
	glVertex2f(0, -270);
	glVertex2f(270, 0);
	glVertex2f(-270, 0);
	glEnd();

	glutSwapBuffers();


}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	 // to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	 // to refresh the window it calls display() function	
	glutPostRedisplay();
}

void onMove(int x, int y) {
	// Write your codes here.



	 // to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	// Change angle of A
	if (left) A.angle += 1;
	if (right) A.angle -= 1;
	if (A.angle > 360)
		A.angle = (int)A.angle % 360;
	if (A.angle < 0)
		A.angle = 360 - (int)A.angle % 360;

	// Check if the A sees B, (visibility)
	vec_t W = pol2rec({ 1, A.angle });
	vec_t V = unitV(subV(B.pos, A.pos));
	B.visibility = dotP(V, W) > cos(FOV * D2R);  // E.vis:true , P sees E.

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Sýla Yapýcý");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}
