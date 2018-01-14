#ifndef __CLUSTER_HELPERS__
#define __CLUSTER_HELPERS__
#include "cluster.h"
#include "cluster_initialization.h"
#include "command_line_parser.h"
#include "function_caster.h"

unsigned int interval_binary_search(Cluster *clusters, unsigned int num_of_clusters,
		void **points, unsigned int num_of_points, PossibleMedoid *array,
		double element, int left, int right);
int find_min_cluster(Cluster *clusters, unsigned int num_of_clusters,
		void *point, double **dists, double *minimum_distance, 
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster);
double minimization_function(void **points, unsigned int number_of_points, 
		Cluster *clusters, Params parameters, double **dists,
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster);
int find_second_nearest_cluster(Cluster *clusters, unsigned int num_of_clusters,
		Cluster best_cluster, void *point, double **dists,
		double *minimum_distance, 
		void (*garbage_collector)(double **, unsigned int size),
		FunctionCaster function_caster);
void destroy_clusters(Cluster *clusters, unsigned int num_of_clusters);
#endif
