/*
 * grids.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: letrungkien7
 */

#include "grids.hpp"

Grids::Grids(int M, int N) {
	// TODO Auto-generated constructor stub
	this->M = M;
	this->N = N;
	data.resize(M*N);

}

Grids::~Grids() {
	// TODO Auto-generated destructor stub
}

void Grids::build(const vector<Particle> &ps, const Vec &range){
	clear();
	if(neighbor.size() < ps.size())
		neighbor.resize(ps.size());
	Vec factor((double)(M-3)/(range[0]), (double)(N-3)/(range[1]));
	For(i,0,ps.size()){
		int x = max(0.0, ps[i].r[0]*factor[0]);
		int y = max(0.0,ps[i].r[1]*factor[1]);
		assert(y*M+x<M*N);
		if(y*M+x<0)
			cout << x << " , " << y <<endl;
		assert(y*M+x>=0);
		(*this)(x,y).push_back(i);
	}

	For(i,1,M-2){
		For(j,1,N-2){
			list<int> around;
			For(p,-1,2){
				For(q,-1,2){
					around.insert(around.end(), (*this)(i+p,j+q).begin(), (*this)(i+p,j+q).end());
				}
			}
			for(int p : (*this)(i,j)){
				for(int q : around){
					if(p!=q)
						neighbor[p].push_back(q);
				}
			}
		}
	}
}

void Grids::clear(){
	for(list<int> &l : data){
		l.clear();
	}
	for(list<int> &l : neighbor){
		l.clear();
	}
}

list<int>& Grids::operator()(int i, int j){
	return data[j*M+i];
}
