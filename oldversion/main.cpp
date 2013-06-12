#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>

#include <GL/glut.h>
#include <eigen3/Eigen/Dense>
#include <opencv2/opencv.hpp>

#include "system.hpp"

using namespace std;


System sph;


void myDisplay(){
	/*Initialize display*/
	sph.calculate(DT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,1.0);

	glPushMatrix();
	glScaled(40,40,1);
	sph.draw();
	glPopMatrix();
	glutSwapBuffers();
}

void myInit(){
	glEnable(GL_DEPTH_TEST);
	sph.init();
}

void myIdle(){
	//glutPostRedisplay();
}

void myReshape(int w, int h){
	winWidth = w;
	winHeight = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-20, w+20, -20, h+20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0,0,w,h);
}

void myTimer(int value){
	sph.calculate(DT);
	glutPostRedisplay();
	glutTimerFunc(TIMER, myTimer, 0);
}

void myMouse(int button, int state, int x, int y){
	mouseButton = button;
	mouseState = state;
	mouseX = x;
	mouseY = y;
}

void myMotion(int x, int y){
	switch(mouseButton){
	case GLUT_LEFT_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	default:
		break;
	}
}

void myKeyboard(unsigned char c, int x, int y){
	switch(c){
	case 27:
		exit(1);
		break;
	default:
		break;
	}

}

int main(int argc, char* argv[]){
	//---Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(winWidth, winHeight);		// window size
	glutCreateWindow("ARTISTIC CG - Smoothed Particle Hydrodynamics");				// create window

	//---GLUTへの関数の登録lut
	glutDisplayFunc(myDisplay);			//Display関数の登録
	glutIdleFunc(myIdle);				//Idle関数の登録
	glutTimerFunc(TIMER, myTimer, 0);	// timer
	glutReshapeFunc(myReshape);			// reshape
	glutMotionFunc(myMotion); 			// mouse's motion
	glutMouseFunc(myMouse);				// mouse
	glutKeyboardFunc(myKeyboard);		// keyboard
	//glutSpecialFunc();

	srand(time(NULL));
	myInit();

	//---ループの開始
	glutMainLoop();

	return 0;
}
