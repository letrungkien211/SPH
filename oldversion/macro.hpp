#ifndef MACRO_HPP_
#define MACRO_HPP_

#include <eigen3/Eigen/Dense>
using namespace Eigen;
//////////////////////////////////////////////////////////////////////////
#define For(i,m,n) 	for(int i = m; i < n; i++)
#define Sqr(x) 		(x)*(x)
#define Vec 		Vector2d

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
static const int TIMER = 25;

static int winWidth = 500.0;
static int winHeight = 500.0;
static int mouseButton = 0;
static int mouseState;
static double mouseX, mouseY;

static const Vector3d COLOR_RED = Vector3d(1,0,0);
static const Vector3d COLOR_GREEN = Vector3d(0,1,0);
static const Vector3d COLOR_BLUE = Vector3d(0,0,1);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/* Simulation constants*/
//////////////////////////////////////////////////////////////////////////
static int MAX_LOOP = 1000;
static Vec INITMIN = Vec(1,1);
static Vec INITMAX = Vec(4,4);
static Vec MIN = Vec(0.5,0.5);
static Vec MAX = 5*Vec(1,1);
static double PI = 3.141;

static Vec G = Vec(0,-9.78);
static double SPH_PMASS = 0.00020543;
//static double SPH_PMASS = 0.0020543;
static double SPH_EXTSTIFF = 10000.0;
static double SPH_EXTDAMP = 256.0;
static double DT = 0.004;
static double EPSILON = 1e-6;
static double SPH_RESTDENSITY = 600.0;
static double SPH_PDIST = pow( SPH_PMASS / SPH_RESTDENSITY, 1/3.0 );
static double SPH_SIMSCALE = 0.04;

static double SPH_LIMIT = 200.0;		// Speed Limit

// Auxiliary functions
double randf(double min, double max);
Vec randf(const Vec &min, const Vec &max);
Vector3d randf(const Vector3d &min, const Vector3d &max);
void glCircle(float x, float y, float r, int segments);


#endif