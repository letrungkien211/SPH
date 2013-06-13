/*
 * sph.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: letrungkien7
 */
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <cassert>

using namespace std;
using namespace Eigen;

#include "sph.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

SPH sph;

void myDisplay(){
	glClearColor(0.02f, 0.01f, 0.01f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(6);
	glBegin(GL_POINTS);

	sph.display();

	glEnd();

	glutSwapBuffers();
}

void myInit(int argc, char**argv){
	glEnable(GL_PROGRAM_POINT_SIZE);
	sph.output.open(argv[1]);
	sph.init();
	getchar();
}

void myReshape(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, kViewWidth, 0, kViewHeight, 0, 1);
}

void myUpdate(){
	sph.update();
	glutPostRedisplay();
}
void myKeyboard(unsigned char c, int x, int y){
	switch(c){
	case 27:
	    sph.output.close();
		exit(1);
		break;
	default:
		break;
	}

}


int main(int argc, char**argv){
	glutInitWindowSize(kScreenWidth, kScreenHeight);
	glutInit(&argc, argv);
	glutInitDisplayString("samples stencil>=3 rgb double depth");
	glutCreateWindow("Artistic CG Final Project");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutIdleFunc(myUpdate);
	glutKeyboardFunc(myKeyboard);
	srand(time(NULL));
	myInit(argc, argv);

	glutMainLoop();

	return 0;
}
