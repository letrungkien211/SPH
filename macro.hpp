/*
 * macro.hpp
 *
 *  Created on: Jun 12, 2013
 *      Author: letrungkien7
 */
#ifndef MACRO_HPP_
#define MACRO_HPP_

#include <eigen3/Eigen/Dense>

using namespace Eigen;

#define Vec Vector2d
#define For(i,n) for(int i = 0; i <n; i++)

#define kScreenWidth 640
#define kScreenHeight 640
#define kViewWidth 10.0f
static const int kViewHeight = (kScreenHeight*kViewWidth/kScreenWidth);
#define kPi 3.1415926535f
#define kParticleCount 3000
#define kRestDensity 82.0f
#define kStiffness 0.08f
#define kNearStiffness 0.1f
#define kSurfaceTension 0.0004f
#define kLinearViscocity 0.5f
#define kQuadraticViscocity 1.0f

#define kParticleRadius 0.05f
const double kH = (6*kParticleRadius);
const double kH2 = kH*kH;

#define kFrameRate 20
#define kSubSteps 7


static const double kDt = ((1.0f/kFrameRate) / kSubSteps);
static const double kDt2 = (kDt*kDt);
static const double kNorm =(20/(2*kPi*kH*kH));
static const double kNearNorm =(30/(2*kPi*kH*kH));

#define kEpsilon 0.0000001f
static const double kEpsilon2 = (kEpsilon*kEpsilon);

#define kCellSize kH
static const int kGridWidth = (size_t)(kViewWidth / kCellSize);
static const int kGridHeight = (size_t)(kViewHeight / kCellSize);
static const int kGridCellCount = kGridWidth * kGridHeight;


double randf(double min, double max);
Vec randf(const Vec &min, const Vec &max);
Vector3d randf(const Vector3d &min, const Vector3d &max);

#define kMaxNeighbourCount 64
#endif /* MACRO_HPP_ */
