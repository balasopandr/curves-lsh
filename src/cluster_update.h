#ifndef __CLUSTER_UPDATE__
#define __CLUSTER_UPDATE__
#include "cluster.h"

void MeanUpdate(Cluster *clusters, unsigned int num_of_clusters, void del_func(void *), double **dists);

#endif
