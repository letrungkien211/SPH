#ifndef MACRO_HPP_
#define MACRO_HPP_

#include <eigen3/Eigen/Dense>
using namespace Eigen;
//////////////////////////////////////////////////////////////////////////
#define For(i,m,n) 	for(int i = m; i < n; i++)
#define Sqr(x) 		(x)*(x)
#define Vec 		Vector2d

//////////////////////////////////////////////////////////////////////////
// Physics constant
#define G			9.78*10e2
#define DT			1e-2f
#define PI			3.14159
//////////////////////////////////////////////////////////////////////////
static const int TIMER = 25;
static const double RED[4] = {0,0,1,1};
static const double LIGHTPOS[4] = {0.0, 3.0, 5.0, 1.0};

static int winWidth = 500.0;
static int winHeight = 500.0;
static int mouseButton = 0;
static int mouseState;
static double mouseX, mouseY;

/* Simulation constants*/
static const int NUM = 1000;

static const double KERNEL_POLY6 = 1.56668147106;
static const double KERNEL_SPIKY = 4.77464829276;
static const double KERNEL_VISCO = 2.38732414638;

//////////////////////////////////////////////////////////////////////////
static const double RANGE = 20;
static const Vec MIN = RANGE*Vec( 0, 0);
static const Vec MAX = RANGE*Vec( 1.0, 1.0);
static const Vec INIT_MIN = RANGE*Vec( 0.1, 0.1);
static const Vec INIT_MAX = RANGE*Vec( 0.9, 0.9);

static const double RADIUS = 0.01; // radius

// Color
static const Vector3d COLOR_RED = Vector3d(1,0,0);
static const Vector3d COLOR_GREEN = Vector3d(0,1,0);
static const Vector3d COLOR_BLUE = Vector3d(0,0,1);

// Auxiliary functions
double randf(double min, double max);
Vec randf(const Vec &min, const Vec &max);
Vector3d randf(const Vector3d &min, const Vector3d &max);
void glCircle(float x, float y, float r, int segments);


#endif
