/*
 * system.cpp
 *
 *  Created on: May 17, 2013
 *      Author: letrungkien7
 */

#include "system.hpp"
#include <cstdlib>

System::System() :
N(0), grids(30, 30), h(0) {
}

System::~System() {
}

void System::init(int N) {
	this->N = N;
	particles.resize(N);
	For(i,0,N)
	{
		particles[i].r << randf(INIT_MIN, INIT_MAX);
		particles[i].radius = RADIUS;
		particles[i].v << randf(INIT_MIN, INIT_MAX)/100;
		particles[i].color << 1, 0, 0;
		particles[i].rho0 = 600;
		particles[i].k = 1;
		particles[i].myu = 0.2;
		particles[i].m = 0.00020543;
	}
	h = 5*RADIUS;
	cout << " H "<< h <<endl;
	getchar();
}

void System::draw() {
	// Draw boundary
	glBegin(GL_LINE_STRIP);
	glPushMatrix();
	glColor3dv(COLOR_RED.data());
	glVertex2d((double) MIN[0], (double) MIN[1]);
	glVertex2d((double) MIN[0], 2 * (double) MAX[1]);
	glVertex2d(2 * (double) MAX[0], 2 * (double) MAX[1]);
	glVertex2d(2 * (double) MAX[0], (double) MIN[1]);
	glVertex2d((double) MIN[0], (double) MIN[1]);
	glPopMatrix();
	glEnd();


	// Draw particles
	for (Particle p : particles) {
		p.draw();
	}
	getchar();
}

void System::calculate(double dt) {
	// Grids
	grids.build(particles, MAX);

	// Density and Pressure

	For(i,0,N)
	{
		// Density
		particles[i].rho = 0;
		for (int j : grids.neighbor[i]) {
			double r2 = (particles[i].r - particles[j].r).squaredNorm();
			double h2 = h * h;
			if (r2 < h2)
				particles[i].rho += pow(h2 - r2, 3);
		}
		particles[i].rho *= KERNEL_POLY6 / pow(h, 6);
		// Pressure
		particles[i].p = particles[i].k
				* (particles[i].rho - particles[i].rho0);
		if(particles[i].p < 0){
			cout << particles[i].rho <<endl;
			particles[i].p = 1e-5;
		}
		assert(particles[i].p > 0);
	}

	// Force
	For(i,0, N){
		Vec fp(0,0);			// pressure force
		Vec fv(0,0);			// viscosity force
		for(int j: grids.neighbor[i]){
			Vec r = particles[i].r - particles[j].r;
			double r1 = r.norm();
			if(r1<h){
				fp -= particles[j].m*(particles[i].p+particles[j].p)/(2*particles[j].rho)*3.0*pow((h-r1),2)/r1*r;
				fv += particles[j].m*(particles[j].v-particles[i].v)*(4.5*r1/pow(h,3)+4/pow(h,2)+0.5*h/pow(r1,3));
			}
		}
		fp *= KERNEL_SPIKY/pow(h,6);
		fv *= KERNEL_VISCO/pow(h,3)*particles[i].myu;

		particles[i].f = fp;//+fv;
	}


	// 6. position
	For(i,0,N)
	{
		//cout << particles[i].f.transpose() <<endl;
		particles[i].v += (Vec(0,-G) + particles[i].f/particles[i].m)*dt;
		particles[i].r += particles[i].v*dt;
		//cout << "MIN, MAX: " << MIN.transpose() << " : " <<MAX.transpose() <<endl;
		For(j,0,2)
		{
			if (particles[i].r[j] <= MIN[j]) {
				particles[i].v[j] = -particles[i].v[j];
				particles[i].r[j] = MIN[j];
			}
			else if (particles[i].r[j] >= MAX[j]) {
				particles[i].v[j] = -particles[i].v[j];
				particles[i].r[j] = MAX[j];
			}
		}
		cout << particles[i].r.transpose() <<endl;
		assert(particles[i].r[0] >= 0 && particles[i].r[1]>=0);
	}

	grids.clear();
}

