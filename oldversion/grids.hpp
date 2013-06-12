/*
 * grids.hpp
 *
 *  Created on: Jun 5, 2013
 *      Author: letrungkien7
 */

#ifndef GRIDS_HPP_
#define GRIDS_HPP_

#include <iostream>
#include <vector>
#include <list>

#include "macro.hpp"
#include "particle.hpp"
using namespace std;

class Grids {
public:
	vector<list<int> > data;
	vector<list<int> > neighbor;
	int M,N;		// size

	Grids(int M, int N);
	virtual ~Grids();
	void build(const vector<Particle> &ps, const Vec &range);
	void clear();
	list<int> &operator()(int i, int j);
};

#endif /* GRIDS_HPP_ */
