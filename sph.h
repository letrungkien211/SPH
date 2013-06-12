/*
 * sph.h
 *
 *  Created on: Jun 12, 2013
 *      Author: letrungkien7
 */

#ifndef SPH_H_
#define SPH_H_

#include <vector>
#include <eigen3/Eigen/Dense>
#include <list>

using namespace Eigen;
using namespace std;

#include "macro.hpp"

class Neigbor{
public:
	int index;
	double r;

	Neigbor(int _index, double _r){
		r = _r;
		index = _index;
	}
};

struct Particle{
	Vec r;	// position
	Vec v;	// velocity

	double P;	// pressure
	double nearP;	// near pressure

	double m;	// mass

	double density;		// density
	double nearDensity;	// near density

	Vector3d color;

	// Supplementary
	Vec rPrev; // previous position
	Vec rRelax; // relaxed position
	Vector2i gridIndex; // grid index

	list<Neigbor> neighbors;
};

struct Wall{
	Vec norm;
	double c;
	Wall(double x=0, double y=0, double c = 0): norm(x,y), c(c){
	}
};

class SPH {
public:
	SPH();

	vector<Particle> particles;	// particles
	vector<list<int> > grids; 	// grids
	vector<Wall> walls;

	int N;		// number of particles

	void init();
	void update();
	void emit();
	void applyGravity();
	void advance();
	void updateGrid();
	void calculatePressure();
	void calculateRelaxedPositions();
	void moveToRelaxedPositions();
	void resolveCollisons();
	void display();
};

#endif /* SPH_H_ */
