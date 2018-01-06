#ifndef __SILHOUETTE__
#define __SILHOUETTE__
#include "cluster.h"
#include "list.h"
#include "command_line_parser.h"
#include "function_caster.h"

double calculate_silhouette(Cluster *clusters, Params parameters,
		double **silhouette, double **dists, 
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster);

double calculate_average(Cluster cluster, void *point, double **dists,
	FunctionCaster function_caster);

#endif
