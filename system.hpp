/*
 * system.hpp
 *
 *  Created on: May 17, 2013
 *      Author: letrungkien7
 */

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include "particle.hpp"
#include "macro.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>
#include <cmath>
#include <cassert>
#include <list>
#include <GL/glut.h>
#include <eigen3/Eigen/Dense>

using namespace std;
#include "grids.hpp"

/* Bounding Box*/

class System {
public:
	// SPH Parameters
	double SPH_VISC;
	double SPH_INTSTIFF;
    //double SPH_EXTSTIFF;
	double H;
	double RADIUS;
	double Poly6Kern;
	double SpikyKern;
	double LapKern;

	System();
	virtual ~System();
	int N;   							// number of particles
	vector<Particle> particles; 		// vector of particles
	Grids grids;

	void init();					// initialize
	void loadParameter(const string &filename);
	void draw();						// draw

	void calculate(double dt);			// calculate density, pressure, force, velocity and position
	void saveState();					// save state
};
#endif /* SYSTEM_HPP_ */

