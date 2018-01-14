#include <stdlib.h>
#include "cluster.h"

struct clust
{
	void *medoid;
	int medoid_idx; /* array index */
	List points;
};

Cluster Cluster_create()
{
	Cluster cluster = malloc(sizeof(struct clust));
	cluster->medoid = NULL;
	cluster->points = List_init();

	return cluster;
}

void Cluster_add_point(Cluster cluster, void *point)
{
	List_insert(cluster->points, point);
}

void Cluster_print(Cluster cluster, FILE *stream, void (*print_func)(void *, FILE *))
{
	print_func(cluster->medoid, stream);
}

void Cluster_delete(Cluster cluster)
{
	cluster->medoid = NULL;
	List_destroy(cluster->points, NULL);
	free(cluster);
	cluster = NULL;
}

void * Cluster_get_medoid(Cluster cluster)
{
	return cluster->medoid;
}

int Cluster_get_medoid_idx(Cluster cluster)
{
	return cluster->medoid_idx;
}

void Cluster_set_medoid(Cluster cluster, void *medoid, int medoid_idx)
{
	cluster->medoid = medoid;
	cluster->medoid_idx = medoid_idx;
}

/* returns true if a point is a medoid */
bool Cluster_is_medoid(Cluster *clusters, unsigned int num_of_clusters, void *point)
{
	int i;
	for(i=0; i<num_of_clusters; i++)
	{
		if(point == Cluster_get_medoid(clusters[i]))
		{
			return true;
		}
	}
	return false;
}

unsigned int Cluster_get_points_count(Cluster cluster)
{
	return List_get_length(cluster->points);
}

List Cluster_get_points(Cluster cluster)
{
	return cluster->points;
}

void Cluster_delete_points(Cluster cluster)
{
	List_destroy(cluster->points, NULL);
	cluster->points = List_init();
}

Cluster Cluster_copy(Cluster cluster, void * (*copy_medoid)(void *))
{
	Cluster new_cluster = Cluster_create();
	List_destroy(new_cluster->points, NULL);
	new_cluster->medoid_idx = cluster->medoid_idx;
	new_cluster->medoid = cluster->medoid;
//	new_cluster->medoid = copy_medoid(cluster->medoid);
	new_cluster->points = List_copy(cluster->points);
	return new_cluster;
}
