#ifndef __CURVE_METRICS__
#define __CURVE_METRICS__
#include "curve.h"
#include "complete_binary_tree.h"
#include "crmsd.h"

double Euclidean_distance(double *pointA, double *pointB);
double Frechet_distance(Curve P, Curve Q, double ***distances);
List OptimalTraversal(unsigned int m, unsigned int n, double **distances);
Curve MeanFrechetCurve(Curve P, Curve Q, List traversal);
Curve PostOrderTraversal(CompleteBinaryTree node, double **dists);
#endif
