#ifndef __CRMSD__
#define __CRMSD__
#include "curve.h"

double CRMSD_curve_dist(Curve P, Curve Q, double ***distances);
double CRMSD_dist(double *X, int sizeX, double *Y, int sizeY);
double * CRMSD_translate_and_rotate(double *X, int sizeX, double *Y, int sizeY);
double CRMSD_frechet_dist(Curve Pc, Curve Qc, double ***dists);

#endif
