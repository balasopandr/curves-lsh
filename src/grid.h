#ifndef __GRID__
#define __GRID__
#include "curve.h"
#include "global_defs.h"
#include "vector.h"

Vector Grid_curve_project(double grid_density, double *shift_value, Curve curve, int *max_points);
double * Grid_point_project(double grid_density, double *shift_value, double *point);
Vector Grid_build_vector(double grid_density, double **shift_value, Curve curve, unsigned int k, int *max_points);
#endif
