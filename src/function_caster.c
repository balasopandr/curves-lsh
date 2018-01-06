#include <stdlib.h>
#include "function_caster.h"
#include "curve.h"
#include "curve_metrics.h"

unsigned int object_points(void *point)
{
	return Curve_get_points_count((Curve) point);
}

double point_dist(void *P, void *Q, double ***array)
{
	return Frechet_distance((Curve)P, (Curve) Q, array);
}

bool dists_condition(void *point)
{
	return Curve_is_mean((Curve) point);
}

void del_mean_curve(void *point)
{
	Curve_del_mean_curve((Curve) point);
}

void * copy_medoid(void *medoid)
{
	return (void *) Curve_copy((Curve) medoid);
}

unsigned int get_object_points(void *point)
{
	return Curve_get_points_count((Curve) point);
}

int get_object_index(void *point)
{
	return Curve_get_curve_idx((Curve) point);
}

FunctionCaster FunctionCaster_init()
{
	
	FunctionCaster function_caster = malloc(sizeof(struct fun_cst));
	function_caster->object_points = &object_points;
	function_caster->point_dist = &point_dist;
	function_caster->dists_condition = &dists_condition;
	function_caster->del_mean_curve = &del_mean_curve;
	function_caster->copy_medoid = &copy_medoid;
	function_caster->get_object_points = &get_object_points;
	function_caster->get_object_index = &get_object_index;

	return function_caster;
}

void FunctionCaster_delete(FunctionCaster function_caster)
{
	free(function_caster);
	function_caster = NULL;
}
