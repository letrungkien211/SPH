/*
 * system.cpp
 *
 *  Created on: May 17, 2013
 *      Author: letrungkien7
 */

#include "system.hpp"
#include <cstdlib>
#include <fstream>

System::System():grids(100, 100){
	loadParameter(string("param.txt"));
}

System::~System() {
}

void System::init() {
	particles.resize(N);
	For(i,0,N)
	{
		particles[i].r << randf(INITMIN, INITMAX);
		particles[i].radius = RADIUS;
		particles[i].v << randf(INITMIN, INITMAX) / 100;
		particles[i].color << randf(Vector3d(0,0,0), Vector3d(1,0,0));
		particles[i].rho0 = SPH_RESTDENSITY;
		particles[i].k = SPH_INTSTIFF;
		particles[i].myu = SPH_VISC;
		particles[i].m = 4*particles[i].radius*particles[i].radius*particles[i].rho0;
	}
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
}

void System::calculate(double dt) {
	// Grids
	grids.build(particles, MAX);

	int sum = 0;
	for(list<int> l : grids.neighbor){
		sum += l.size();
	}
	cout << sum << " , " << sum/N<<endl;

	double H2 = H * H;
	// Density and Pressure

	list<int> nei(N);
	int cnt = 0;
	for(int &i : nei){
		i = cnt++;
	}

	// Density and pressure and
	double max = -1;
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
		if(pi.rho > max)
			max = pi.rho;
		pi.p = (pi.rho - pi.rho0) * pi.k;
		pi.rho = 1.0/pi.rho;
	}

	For(i,0,N){
	    particles[i].color[0] = 0.8/(particles[i].rho*max)+0.2;
	}

	// Force
	For(i,0, N)
	{
		Particle &pi = particles[i];
		pi.f = Vec(0,0);
		for (int j : grids.neighbor[i]) {
		//for (int j : nei) {
			if(i==j)
							continue;
						Particle &pj = particles[j];
			Vec dr = (pi.r - pj.r) * SPH_SIMSCALE;
			double r = dr.norm();
			if (H > r) {
				double c = H - r;
				double pterm = -0.5 * c * SpikyKern * (pi.p + pj.p) / r;
				double vterm = LapKern * SPH_VISC;
				Vec fcurr = (pterm * dr + vterm * (pj.v - pi.v)) * c * pi.rho* pj.rho;
				pi.f += fcurr;
			}
		}
	}




	// Velocity and Position
	for (Particle &p : particles) {
		Vec accel = p.f * p.m;
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
//			p.r[0] = MIN[0] + 2*RADIUS/SPH_SIMSCALE;
		}
		diff = 2.0 * RADIUS - ( MAX[0] - p.r[0] ) * SPH_SIMSCALE;
		if ( diff > EPSILON )
		{
			Vec norm(-1,0);
			adj = SPH_EXTSTIFF * diff - SPH_EXTDAMP * norm.transpose()*p.v;
			accel += adj * norm;
//			p.r[0] = MAX[0] - 2*RADIUS/SPH_SIMSCALE;
		}

		// Y-axis walls
		diff = 2.0 * RADIUS - ( p.r(1) - MIN(1) ) * SPH_SIMSCALE;
		if ( diff > EPSILON )
		{
			Vec norm(0,1);
			adj = SPH_EXTSTIFF * diff - SPH_EXTDAMP * norm.transpose()*p.v;
			accel += adj * norm;
//			p.r[1] = MIN[1] + 2*RADIUS/SPH_SIMSCALE;
		}
		diff = 2.0 * RADIUS - ( MAX[1] - p.r[1] ) * SPH_SIMSCALE;
		if ( diff > EPSILON )
		{
			Vec norm(0,-1);
			adj = SPH_EXTSTIFF * diff - SPH_EXTDAMP * norm.transpose()*p.v;
			accel += adj * norm;
//			p.r[1] = MAX[1] - 2*RADIUS/SPH_SIMSCALE;
		}
		accel += G;
		p.v += accel * DT;
		p.r += p.v * DT / SPH_SIMSCALE;
	}

	grids.clear();
}

void System::loadParameter(const string &filename){
	ifstream input(filename.c_str());
	input >> SPH_INTSTIFF >> SPH_VISC >> H >> RADIUS >> N;
	H *= RADIUS;
	Poly6Kern = 4 / ( PI * pow( H, 8 ) );
	SpikyKern = -10.0 / ( PI * pow( H, 5 ) );
	LapKern = 1 / (0.3* PI * pow( H, 2 ) );
}
