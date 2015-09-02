// Flight Simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <string>
#include <vector>
#include "plane.h"
#define PI 3.14159265


//global vars
int nFPS = 60;
float fps = 0;
float rollAngle = 0;
float rollRate = 0;
float pitchAngle = 0;
float pitchRate = 0;
clock_t startClock = 0, curClock;
long long int prevF = 0, curF = 0;
plane planeView;
float sealevel;
int res = 257;

#define ADDR(i,j,k) (3*((j)*res + (i)) + (k))

GLfloat *verts = 0;
GLfloat *norms = 0;
GLuint *faces = 0;

float frand(float x, float y) {
	static int a = 1554340000, b = 1117543401;
	int xi = *(int *)&x;
	int yi = *(int *)&y;
	srand(((xi * a) % b) - ((yi * b) % a));
	return 2.0*((float)rand() / (float)RAND_MAX) - 1.0;
}

void mountain(int i, int j, int s)
{
	if (s > 1) {

		float x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3, x01, y01, z01, x02, y02, z02, x13, y13, z13, x23, y23, z23, x0123, y0123, z0123;

		x0 = verts[ADDR(i, j, 0)];
		y0 = verts[ADDR(i, j, 1)];
		z0 = verts[ADDR(i, j, 2)];

		x1 = verts[ADDR(i + s, j, 0)];
		y1 = verts[ADDR(i + s, j, 1)];
		z1 = verts[ADDR(i + s, j, 2)];

		x2 = verts[ADDR(i, j + s, 0)];
		y2 = verts[ADDR(i, j + s, 1)];
		z2 = verts[ADDR(i, j + s, 2)];

		x3 = verts[ADDR(i + s, j + s, 0)];
		y3 = verts[ADDR(i + s, j + s, 1)];
		z3 = verts[ADDR(i + s, j + s, 2)];

		x01 = 0.5*(x0 + x1);
		y01 = 0.5*(y0 + y1);
		z01 = 0.5*(z0 + z1);

		x02 = 0.5*(x0 + x2);
		y02 = 0.5*(y0 + y2);
		z02 = 0.5*(z0 + z2);

		x13 = 0.5*(x1 + x3);
		y13 = 0.5*(y1 + y3);
		z13 = 0.5*(z1 + z3);

		x23 = 0.5*(x2 + x3);
		y23 = 0.5*(y2 + y3);
		z23 = 0.5*(z2 + z3);

		x0123 = 0.25*(x0 + x1 + x2 + x3);
		y0123 = 0.25*(y0 + y1 + y2 + y3);
		z0123 = 0.25*(z0 + z1 + z2 + z3);

		z01 += 0.5*((float)s / res)*frand(x01, y01);
		z02 += 0.5*((float)s / res)*frand(x02, y02);
		z13 += 0.5*((float)s / res)*frand(x13, y13);
		z23 += 0.5*((float)s / res)*frand(x23, y23);
		z0123 += 0.5*((float)s / res)*frand(x0123, y0123);

		verts[ADDR(i + s / 2, j, 0)] = x01;
		verts[ADDR(i + s / 2, j, 1)] = y01;
		verts[ADDR(i + s / 2, j, 2)] = z01;

		verts[ADDR(i, j + s / 2, 0)] = x02;
		verts[ADDR(i, j + s / 2, 1)] = y02;
		verts[ADDR(i, j + s / 2, 2)] = z02;

		verts[ADDR(i + s, j + s / 2, 0)] = x13;
		verts[ADDR(i + s, j + s / 2, 1)] = y13;
		verts[ADDR(i + s, j + s / 2, 2)] = z13;

		verts[ADDR(i + s / 2, j + s, 0)] = x23;
		verts[ADDR(i + s / 2, j + s, 1)] = y23;
		verts[ADDR(i + s / 2, j + s, 2)] = z23;

		verts[ADDR(i + s / 2, j + s / 2, 0)] = x0123;
		verts[ADDR(i + s / 2, j + s / 2, 1)] = y0123;
		verts[ADDR(i + s / 2, j + s / 2, 2)] = z0123;

		mountain(i, j, s / 2);
		mountain(i + s / 2, j, s / 2);
		mountain(i, j + s / 2, s / 2);
		mountain(i + s / 2, j + s / 2, s / 2);

	}
	else {

		float dx, dy, dz;

		if (i == 0) {
			dx = verts[ADDR(i + 1, j, 2)] - verts[ADDR(i, j, 2)];
		}
		else if (i == res - 1) {
			dx = verts[ADDR(i, j, 2)] - verts[ADDR(i - 1, j, 2)];
		}
		else {
			dx = (verts[ADDR(i + 1, j, 2)] - verts[ADDR(i - 1, j, 2)]) / 2.0;
		}

		if (j == 0) {
			dy = verts[ADDR(i, j + 1, 2)] - verts[ADDR(i, j, 2)];
		}
		else if (j == res - 1) {
			dy = verts[ADDR(i, j, 2)] - verts[ADDR(i, j - 1, 2)];
		}
		else {
			dy = (verts[ADDR(i, j + 1, 2)] - verts[ADDR(i, j - 1, 2)]) / 2.0;
		}

		dx *= res;
		dy *= res;
		dz = 1.0 / sqrt(dx*dx + dy*dy + 1.0);
		dx *= dz;
		dy *= dz;

		norms[ADDR(i, j, 0)] = dx;
		norms[ADDR(i, j, 1)] = dy;
		norms[ADDR(i, j, 2)] = dz;
	}
}

void makemountain()
{
	int i, j;

	if (verts) free(verts);
	if (norms) free(norms);
	if (faces) free(faces);

	verts = (GLfloat *)malloc(res*res * 3 * sizeof(GLfloat));
	norms = (GLfloat *)malloc(res*res * 3 * sizeof(GLfloat));
	faces = (GLuint *)malloc((res - 1)*(res - 1) * 6 * sizeof(GLuint));

	verts[ADDR(0, 0, 0)] = -5.0;
	verts[ADDR(0, 0, 1)] = -5.0;
	verts[ADDR(0, 0, 2)] = 0.0;

	verts[ADDR(res - 1, 0, 0)] = 5.0;
	verts[ADDR(res - 1, 0, 1)] = -5.0;
	verts[ADDR(res - 1, 0, 2)] = 0.0;

	verts[ADDR(0, res - 1, 0)] = -5.0;
	verts[ADDR(0, res - 1, 1)] = 5.0;
	verts[ADDR(0, res - 1, 2)] = 0.0;

	verts[ADDR(res - 1, res - 1, 0)] = 5.0;
	verts[ADDR(res - 1, res - 1, 1)] = 5.0;
	verts[ADDR(res - 1, res - 1, 2)] = 0.0;

	mountain(0, 0, res - 1);

	GLuint *f = faces;
	for (j = 0; j < res - 1; j++) {
		for (i = 0; i < res - 1; i++) {
			*f++ = j*res + i;
			*f++ = j*res + i + 1;
			*f++ = (j + 1)*res + i + 1;
			*f++ = j*res + i;
			*f++ = (j + 1)*res + i + 1;
			*f++ = (j + 1)*res + i;
		}
	}

}

//initialize scene
void init(void)
{
	glClearColor(0.5, 0.5, 1.0, 0.0);
	/*glShadeModel (GL_FLAT); */
	glEnable(GL_DEPTH_TEST);

	gluLookAt(0, 0, 0, 1, 0, 0, 0, 0, 1);

	sealevel = -.2;

	makemountain();
}

//computes lighting
void lighting(void)
{
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lpos[] = { 2.0, 2.0, 2.0, 0.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
}

//display function
void display(void)
{

	glLoadIdentity();             /* clear the matrix */

	//compute fps
	curF++;
	curClock = clock();
	float elapsed = (curClock - startClock) / (float)CLOCKS_PER_SEC;
	if (elapsed>1.0f){
		fps = (float)(curF - prevF) / elapsed;
		prevF = curF;
		startClock = curClock;
		std::string strng = "fps: " + std::to_string(fps);
		std::cout << strng << std::endl;
	}

	//create look at position
	Vector at;
	at.x = planeView.position.x - planeView.heading.x;
	at.y = planeView.position.y - planeView.heading.y;
	at.z = planeView.position.z - planeView.heading.z;

	//look at 'at' vector from planes position
	gluLookAt(planeView.position.x, planeView.position.y, planeView.position.z,
		at.x, at.y, at.z,
		planeView.up.x, planeView.up.y, planeView.up.z);

	//compute lighting
	lighting();

	GLfloat tanamb[] = { 0.2, 0.15, 0.1, 1.0 };
	GLfloat tandiff[] = { 0.4, 0.3, 0.2, 1.0 };
	GLfloat tanspec[] = { 0.0, 0.0, 0.0, 1.0 };	// dirt doesn't glisten
	GLfloat seaamb[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat seadiff[] = { 0.0, 0.0, 0.8, 1.0 };
	GLfloat seaspec[] = { 0.5, 0.5, 1.0, 1.0 };	// Single polygon, will only have highlight if light hits a vertex just right
	GLfloat lpos[] = { 0.0, 0.0, 10.0, 0.0 };	// sun, high noon

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	// send the light position down as if it was a vertex in world coordinates
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	// load terrain material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tanspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

	// Send terrain mesh through pipeline
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glNormalPointer(GL_FLOAT, 0, norms);
	glDrawElements(GL_TRIANGLES, 6 * (res - 1)*(res - 1), GL_UNSIGNED_INT, faces);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// load water material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

	// Send water as a single quad
	glNormal3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-5, -5, sealevel);
	glVertex3f(5, -5, sealevel);
	glVertex3f(5, 5, sealevel);
	glVertex3f(-5, 5, sealevel);
	glEnd();

	glutSwapBuffers();
	glFlush();

	glutPostRedisplay();
}

void timer(int v){
	//update planes position
	planeView.update();

	//set roll angles accoring to rate set by button press
	rollAngle = rollRate;
	pitchAngle = pitchRate;

	//compute roll and pitch
	planeView.Roll(rollAngle);
	planeView.Pitch(pitchAngle);

	glutPostRedisplay(); // trigger display function by sending redraw into message queue
	glutTimerFunc(1000 / nFPS, timer, v);
}

//handle reshaping of window
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.0001, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

//handle keypress
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '-':
		sealevel -= 0.01;
		break;
	case '+':
	case '=':
		sealevel += 0.01;
		break;
	case 'a':
		rollRate = -.03;
		break;
	case 'd':
		rollRate = .03;
		break;
	case 'w':
		pitchRate = .025;
		break;
	case 's':
		pitchRate = -.025;
		break;
	case 'q':
		if (planeView.speed >= -.01) //maximum speed
			planeView.speed -= .0001;
		break;
	case 'z':
		if (planeView.speed < -0.0005) //minimum speed
			planeView.speed += .0001;
		break;
	case 27:
		exit(0);
		break;
	}
}

//handle key release
//stop rolling and pitching
void releaseKey(unsigned char key, int x, int y) {

	switch (key) {
	case 'a':
		rollRate = 0;
		break;
	case 'd':
		rollRate = 0;
		break;
	case 'w':
		pitchRate = .0;
		break;
	case 's':
		pitchRate = .0;
		break;
	}
}

//handles arrow key press
void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		pitchRate = .025;
		break;
	case GLUT_KEY_DOWN:
		pitchRate = -.025;
		break;
	case GLUT_KEY_LEFT:
		rollRate = -.03;
		break;
	case GLUT_KEY_RIGHT:
		rollRate = .03;
		break;
	}
}

//handles arrow key release
//stop rolling and pitching
void SpecialInputRelease(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		pitchRate = .0;
		break;
	case GLUT_KEY_DOWN:
		pitchRate = .0;
		break;
	case GLUT_KEY_LEFT:
		rollRate = .0;
		break;
	case GLUT_KEY_RIGHT:
		rollRate = .0;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow((const char*)"Flight simulator");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutSpecialFunc(SpecialInput);
	glutSpecialUpFunc(SpecialInputRelease);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(releaseKey);

	glutTimerFunc(100, timer, 60);

	startClock = clock();

	glutMainLoop();
	return 0;
}