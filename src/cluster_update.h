#ifndef __CLUSTER_UPDATE__
#define __CLUSTER_UPDATE__
#include "cluster.h"
#include "function_caster.h"
#include "command_line_parser.h"

void MeanUpdate(Cluster *clusters, unsigned int num_of_clusters, void del_func(void *), double **dists);
void PAMUpdate(Cluster **clusters, Params parameters, void **points,
		unsigned int number_of_points, double **dists,
		void (*garbage_collector)(double **, unsigned int),
		FunctionCaster function_caster);
#endif
