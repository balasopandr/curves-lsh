#ifndef __FUNCTION_CUSTER__
#define __FUNCTION_CUSTER__
#include "global_defs.h"
struct fun_cst
{
	unsigned int (*object_points)(void *point);
	double (*point_dist)(void *P, void *Q, double ***array);
	bool (*dists_condition)(void *point);
	void (*del_mean_curve)(void *point);
	void * (*copy_medoid)(void *medoid);
	unsigned int (*get_object_points)(void *point);
	int (*get_object_index)(void *point);
};

typedef struct fun_cst *FunctionCaster;

FunctionCaster FunctionCaster_init();
void FunctionCaster_delete(FunctionCaster function_caster);
#endif
