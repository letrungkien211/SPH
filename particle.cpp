#include "particle.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>
#include <cmath>

#include <GL/glut.h>
#include <eigen3/Eigen/Dense>

void Particle::draw(){
	glPushMatrix();
	glTranslated((double)r[0], (double)r[1], 0.0);
	glColor3d(1,0,0);
	glCircle((double)r[0], (double)r[1], radius, 10);
	glPopMatrix();
}
