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
		particles[i].r << randf(INITMIN, INITMAX);
		particles[i].radius = RADIUS;
		particles[i].v << randf(INITMIN, INITMAX) / 100;
		particles[i].color << 1, 0, 0;
		particles[i].rho0 = 600;
		particles[i].k = 1;
		particles[i].myu = 0.2;
		particles[i].m = 0.00020543;
	}
	h = 5 * RADIUS;
	cout << " H " << h << endl;
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
	double H2 = H * H;
	// Density and Pressure

	For(i,0,N)
	{
		double sum = 0.0;
		Particle &pi = particles[i];

		for (int j : grids.neighbor[i]) {
			Particle &pj = particles[j];
			Vec dr = (pi.r - pj.r) * SPH_SIMSCALE;
			double r2 = dr.squaredNorm();
			if (H2 > r2) {
				double c = H2 - r2;
				sum += c * c * c;
			}
		}
		pi.rho = sum * pi.m * Poly6Kern;
		pi.p = (pi.rho - SPH_RESTDENSITY) * SPH_INTSTIFF;
	}

	// Force
	For(i,0, N)
	{
		Particle &pi = particles[i];
		pi.f = Vec(0,0);
		for (int j : grids.neighbor[i]) {
			Particle &pj = particles[j];
			Vec dr = (pi.r - pj.r) * SPH_SIMSCALE;
			double r = dr.norm();
			if (H > r) {
				double c = H - r;
				double pterm = -0.5 * c * SpikyKern * (pi.p + pj.p) / r;
				double vterm = LapKern * SPH_VISC;
				Vec fcurr = (pterm * dr + vterm * (pj.v - pi.v)) * c * pi.rho* pj.rho;
				;
				pi.f += fcurr;
			}
		}
	}

	// Velocity and Position
	for (Particle &p : particles) {
		Vec accel = p.f * SPH_PMASS;
		double speed = accel.squaredNorm();
		if (speed > SPH_LIMIT * SPH_LIMIT) {
			accel *= SPH_LIMIT / sqrt(speed);
		}


		// X-axis walls
		double diff, adj;

		diff = 2.0 * RADIUS - ( p.r[0] - MIN[0] ) * SPH_SIMSCALE;
		if ( diff > EPSILON )
		{
			Vec norm(1,0);
			double adj = SPH_EXTSTIFF * diff - SPH_EXTDAMP * norm.transpose()*p.v;
			accel += adj * norm;
		}
		diff = 2.0 * RADIUS - ( MAX[0] - p.r[0] ) * SPH_SIMSCALE;
		if ( diff > EPSILON )
		{
			Vec norm(-1,0);
			adj = SPH_EXTSTIFF * diff - SPH_EXTDAMP * norm.transpose()*p.v;
			accel += adj * norm;
		}

		// Y-axis walls
		diff = 2.0 * RADIUS - ( p.r(1) - MIN(1) ) * SPH_SIMSCALE;
		if ( diff > EPSILON )
		{
			Vec norm(0,1);
			adj = SPH_EXTSTIFF * diff - SPH_EXTDAMP * norm.transpose()*p.v;
			accel += adj * norm;
		}
		diff = 2.0 * RADIUS - ( MAX[1] - p.r[1] ) * SPH_SIMSCALE;
		if ( diff > EPSILON )
		{
			Vec norm(0,-1);
			adj = SPH_EXTSTIFF * diff - SPH_EXTDAMP * norm.transpose()*p.v;
			accel += adj * norm;
		}
		accel += G;
		p.v += accel * DT;
		p.r += p.v * DT / SPH_SIMSCALE;
	}

	grids.clear();
}

