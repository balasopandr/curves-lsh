#ifndef __CLUSTER_INITIALIZATION__
#define __CLUSTER_INITIALIZATION__
#include "cluster.h"

typedef struct
{
	double partial_sum;
	void *point;
	unsigned int idx;
} PossibleMedoid;

void KMeans_random_selection(Cluster *clusters, unsigned int num_of_clusters,
		void **points, unsigned int num_of_points);
void KMeans_plus_plus(Cluster *clusters, unsigned int num_of_clusters, void **points,
		unsigned int num_of_points, double **dists);
#endif
