#ifndef __CLUSTER_ASSIGNMENT__
#define __CLUSTER_ASSIGNMENT__
#include "cluster.h"
#include "command_line_parser.h"
#include "hash_table.h"
#include "hash_data.h"
#include "grid.h"
#include "function_caster.h"


void Lloyd_assignment(Cluster *clusters, void **points, unsigned int num_of_points,
		Params parameters, double **dists,
		void (*garbage_collector)(double **, unsigned int),
		FunctionCaster function_caster);

void LSH_assignment(Cluster *clusters, void **points, unsigned int num_of_points,
		Params parameters, double **dists, HashTable *hashes,
		unsigned int *rand_for_hash, double ***shift_values,
		void (*garbage_collector)(double **, unsigned int),
		FunctionCaster function_caster);
#endif
