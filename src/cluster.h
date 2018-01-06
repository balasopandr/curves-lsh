#ifndef __CLUSTER__
#define __CLUSTER__
#include <stdio.h>
#include "list.h"
#include "global_defs.h"

typedef struct clust *Cluster;
Cluster Cluster_create();
void Cluster_add_point(Cluster cluster, void *point);
void Cluster_print(Cluster cluster, FILE *stream, void (*print_func)(void *, FILE *));
void Cluster_delete(Cluster cluster);
void * Cluster_get_medoid(Cluster cluster);
int Cluster_get_medoid_idx(Cluster cluster);
List Cluster_get_points(Cluster cluster);
unsigned int Cluster_get_points_count(Cluster cluster);
void Cluster_set_medoid(Cluster cluster, void *medoid, int medoid_idx);
bool Cluster_is_medoid(Cluster *clusters, unsigned int num_of_clusters, void *point);
void Cluster_delete_points(Cluster cluster);
Cluster Cluster_copy(Cluster cluster, void * (*copy_medoid)(void *));
#endif
