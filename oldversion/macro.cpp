#include "macro.hpp"

#include <cstdlib>
#include <ctime>
#include <GL/glut.h>
double randf(double min, double max){
	return min + (max-min)*((double)rand()/(double)RAND_MAX);
}

Vec randf(const Vec &min, const Vec &max){
	return Vec(randf((double)min[0], (double)max[0]),randf((double)min[1], (double)max[1]));
}

Vector3d randf(const Vector3d &min, const Vector3d &max){
	return Vector3d(randf((double)min[0], (double)max[0]),randf((double)min[1], (double)max[1]), randf((double)min[2], (double)max[2]));
}
