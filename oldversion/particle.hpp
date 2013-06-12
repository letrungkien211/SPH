#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>
#include <cmath>

#include <GL/glut.h>
#include <eigen3/Eigen/Dense>

#include "macro.hpp"

using namespace std;
using namespace Eigen;


enum Shape {Sphere, Cubic};

class Particle{
public:
	/* Physics variables*/
	Vec r; 				// position
	Vec v; 				// velocity
	Vec f; 				// force
	double p; 			// pressure
	double rho; 		// density
	/* Display variables*/
	Vector3d color; 	// color
	double radius;		// radius

	/* Substance constants*/
	double m;			// mass
	double rho0;		// restDensity
	double k;			// Gas constant
	double myu;			// viscosity constant
	void draw();
};

#endif
