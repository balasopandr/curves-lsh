#ifndef __CURVE__
#define __CURVE__
#include <stdio.h>
#include "global_defs.h" 
typedef struct c *Curve;

Curve Curve_create(char *id, unsigned int points_count, double **points, int curve_idx);
void Curve_print(Curve curve, FILE *stream);
void Curve_delete(Curve curve);
unsigned int Curve_get_points_count(Curve curve);
char * Curve_get_id(Curve curve);
double ** Curve_get_points(Curve curve);
unsigned int Curve_get_cluster_idx(Curve curve);
int Curve_get_curve_idx(Curve curve);
void Curve_set_cluster_idx(Curve curve, unsigned int cluster_idx);
double Curve_get_cluster_dist(Curve curve);
void Curve_set_cluster_dist(Curve curve, double cluster_dist);
bool Curve_is_mean(Curve curve);
void Curve_del_mean_curve(Curve curve);
Curve Curve_copy(Curve curve);
#endif
