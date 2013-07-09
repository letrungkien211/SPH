/*
 * sph.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: letrungkien7
 */

#include "sph.h"
#include <GL/glut.h>
#include <iostream>

#define CHUNKSIZE 100

using namespace std;

SPH::SPH() {
	// TODO Auto-generated constructor stub
}

void SPH::init() {
    N = 0;
	grids.resize(kGridCellCount); // move to init functions
	particles.resize(kParticleCount);

	walls.resize(4);
	walls[0] = Wall(1, 0, 0);
	walls[1] = Wall(0, 1, 0);
	walls[2] = Wall(-1, 0, -kViewWidth);
	walls[3] = Wall(0, -1, -kViewHeight);

}
void SPH::display() {
	For(i,N)
					{
		Particle p = particles[i];
		if(i%2)
		    glColor3d(0,1,0);
		else
		    glColor3d(0,0,1);
		glVertex2d(p.r[0], p.r[1]);
					}
}

void SPH::update() {
    int step;
    step = (N== kParticleCount) ? kSubSteps-1 : 0;
    for (step = 0; step <kSubSteps; step++) {
	    For(i,20)
		emit();
		applyGravity();
		advance();
		updateGrid();
		calculatePressure();
		calculateRelaxedPositions();
		moveToRelaxedPositions();
		updateGrid();
		resolveCollisons();
	}
}

void SPH::emit(){
    if(N==kParticleCount)
	return;
    N++;
    Particle &p = particles[N-1];

    if(N%2){
	p.v = Vec(10,-4);
	p.r = randf(Vec(2,8), Vec(3,9));
	p.m = 1.0f;
    }
    else{
	p.v = Vec(-10,-4);
	p.r = randf(Vec(7,8), Vec(8,9));
	p.m = 1.4f;
    }
}

// Apply gravitational force
void SPH::applyGravity() {
#pragma omp parallel for
	For(i,N)
					{
		particles[i].v[1] -= 9.78f * kDt;
					}
}

// Move particles
void SPH::advance() {
#pragma omp parallel for
	For(i,N){
		Particle &p = particles[i];
		p.rPrev = p.r;
		p.r += p.v * kDt;
	}
}

// Update grids
void SPH::updateGrid() {
	for (list<int> &l : grids) {
		l.clear();
	}
#pragma omp parallel for
	For(i,N)
	{
		Particle &p = particles[i];
		int x = p.r[0] / kCellSize;
		int y = p.r[1] / kCellSize;

		x = max(min(x, kGridWidth - 2), 1);
		y = max(min(y, kGridHeight - 2), 1);

		p.gridIndex[0] = x;
		p.gridIndex[1] = y;
		grids[x + y * kGridWidth].push_back(i);
	}
}

void SPH::calculatePressure() {
#pragma omp parallel for
	For(i,N)
					{
		Particle &pi = particles[i];
		pi.neighbors.clear();
		double density = 0;
		double nearDensity = 0;

		int gi = pi.gridIndex[0];
		int gj = pi.gridIndex[1] * kGridWidth;

		for (int ni = gi - 1; ni <= gi + 1; ++ni) {
			for (int nj = gj - kGridWidth; nj <= gj + kGridWidth; nj +=
					kGridWidth) {
				for (int j : grids[ni + nj]) {
					if (i == j) {
						continue;
					}
					const Particle &pj = particles[j];
					Vec dr = pj.r - pi.r;
					double r2 = dr.squaredNorm();

					if (r2 < kEpsilon2 || r2 > kH2) {
						continue;
					}
					double r = sqrt(r2);
					double a = (1 - r / kH);

					density += pj.m * a * a * a;
					nearDensity += pj.m * a * a * a * a;

					if (pi.neighbors.size() < kMaxNeighbourCount) {
						pi.neighbors.push_back(Neigbor(j, r));
					}
				}
			}
		}
		pi.density = density * kNorm;
		pi.nearDensity = nearDensity * kNearNorm;
		pi.P = kStiffness * (pi.density - pi.m * kRestDensity);
		pi.nearP = kNearStiffness * pi.nearDensity;
					}

}

void SPH::calculateRelaxedPositions() {
#pragma omp parallel for
	For(i,N)
					{
		Particle &pi = particles[i];
		Vec pos = pi.r;
		for (Neigbor nei : pi.neighbors) {
			const Particle &pj = particles[nei.index];
			double r = nei.r;
			Vec dr = pj.r - pi.r;
			double a = 1 - r / kH;
			float d = kDt2
					* ((pi.nearP + pj.nearP) * a * a * a * kNearNorm
							+ (pi.P + pj.P) * a * a * kNorm) / 2;

			// relax
			pos -= d * dr / (r * pi.m);

			// surface tension
			if (pi.m == pj.m) {
				pos += (kSurfaceTension) * a * a * kNorm * dr;
			}

			// viscocity
			Vec dv = pi.v - pj.v;
			double u = dv.dot(dr);
			if (u > 0) {
				u /= r;
				float a = 1 - r / kH;
				float I = 0.5f * kDt * a
						* (kLinearViscocity * u + kQuadraticViscocity * u * u);
				pos -= I * kDt * dr;
			}
		}
		pi.rRelax = pos;
					}
}

void SPH::moveToRelaxedPositions() {

#pragma omp parallel for
	For(i,N)
					{
		Particle &p = particles[i];
		p.r = p.rRelax;
		p.v = (p.r - p.rPrev) / kDt;
					}
}

void SPH::resolveCollisons() {
#pragma omp parallel for
	For(i,N)
					{
		Particle &pi = particles[i];
		for (Wall wall : walls) {
			float dis = wall.norm.dot(pi.r) - wall.c;
			if (dis < kParticleRadius) {
				float d = pi.v.dot(wall.norm);
				if (dis < 0)
					dis = 0;
				pi.v += (kParticleRadius - dis) * wall.norm / kDt;

			}
		}
					}
}

