#include <stdlib.h>
#include <math.h>
#include "cluster_assignment.h"
#include "cluster_helpers.h"

void Lloyd_assignment(Cluster *clusters, void **points, unsigned int num_of_points,
		Params parameters, double **dists,
		void (*garbage_collector)(double **, unsigned int),
		FunctionCaster function_caster)
{
	int i;
	/* for each non centoid point */
	for(i=0; i<num_of_points; i++)
	{
		if(!Cluster_is_medoid(clusters, parameters->number_of_clusters, points[i]))
		{
			/* assign it to the nearest centre */
			double minimum_distance;
			int min_cluster = find_min_cluster(clusters, parameters->number_of_clusters,
					points[i], dists, &minimum_distance, garbage_collector,
					function_caster);
			Cluster_add_point(clusters[min_cluster], points[i]);
		}
	}
}

void LSH_assignment(Cluster *clusters, void **points, unsigned int num_of_points,
		Params parameters, double **dists, HashTable *hashes,
		unsigned int *rand_for_hash, double ***shift_values,
		void (*garbage_collector)(double **, unsigned int),
		FunctionCaster function_caster)
{
	double radius = 0.1;
	unsigned int assigned_points = 0;
	unsigned int assigned_to_radius = 2*parameters->number_of_clusters;
	int i,j;

	while(assigned_points != num_of_points 
			&& (double)(assigned_to_radius)/(double)(parameters->number_of_clusters) >= 1.0)
	{
		bool first_run = assigned_points == 0;
		assigned_to_radius = 0;
		/* for each medoid */
		int max_vector_points = 0;
		int count = 0;
		for(i=0; i<parameters->number_of_clusters; i++)
		{
			/* for each hash table*/
			HashData *medoid_hash = malloc(sizeof(HashData)*parameters->number_of_hash_tables);
			for(j=0; j<parameters->number_of_hash_tables; j++)
			{
				Vector grid_vector = Grid_build_vector(parameters->grid_density,
						shift_values[j],
						Cluster_get_medoid(clusters[i]),
						parameters->number_of_grid_curves,
						&max_vector_points);
				medoid_hash[j] = HashData_init((Curve) Cluster_get_medoid(clusters[i]),
						grid_vector);
				/* go to the bucket where the medoid hashes */
				unsigned int hash_val = classic_hash(medoid_hash[j],
						(void *) rand_for_hash);
				List bucket = HashTable_get_buckets(hashes[j])
					[hash_val % HashTable_get_bucket_count(hashes[j])];

				/* for all curves in the bucket */
				Node cur_node = List_get_first_node(bucket);
				while(cur_node != NULL)
				{
					count++;
					HashData hashdata = (HashData) Node_get_data(cur_node);
					Curve curve = HashData_get_curve(hashdata);
					/* edge case ignore medoids */
					if(!(Cluster_is_medoid(clusters, parameters->number_of_clusters,
									(void *) curve)))
					{
						double dist;
						int medoid_idx = Cluster_get_medoid_idx(clusters[i]);
						if(medoid_idx >= 0)
						{
							dist = dists[Curve_get_curve_idx(curve)][medoid_idx];	
						}
						else
						{
							double **distances = NULL;
							dist = function_caster->point_dist(curve,
									Cluster_get_medoid(clusters[i]),
									&distances);
							int m;
							for(m=0; m<Curve_get_points_count(curve); m++)
							{
								free(distances[m]);
								distances[m] = NULL;
							}
							free(distances);
							distances = NULL;
						}
						/* if curve is within radius mark it as assigned */
						if(dist <= radius && 
								(dist > (radius/2.0)
								 || first_run))
						{
							/* resolve medoid assignment conflicts and finally assign points */
							if(dist < Curve_get_cluster_dist(curve))
							{
								Curve_set_cluster_dist(curve, dist);
								Curve_set_cluster_idx(curve, i);
							}
						}
					}
					cur_node = Node_get_next(cur_node);
				}
				HashData_destroy(medoid_hash[j]);
			}
			free(medoid_hash);
			medoid_hash = NULL;
		}
		/* add assigned points to the cluster */
		int m;
		for(m=0; m<num_of_points; m++)
		{
			int idx = Curve_get_cluster_idx((Curve)points[m]); 
			if(idx != -1 && 
					(Curve_get_cluster_dist((Curve)points[m]) > (radius/2.0)
					 || first_run))
			{
				List_insert(Cluster_get_points(clusters[idx]), points[m]);
				assigned_to_radius++;
				assigned_points++;
			}
		}
		/* double the radius */
		radius *= 2;
	}
	/* for every unassigned point proceed as in Lloyd's assignment */
	for(i=0; i<num_of_points; i++)
	{
		{
			if(!Cluster_is_medoid(clusters, parameters->number_of_clusters, points[i])
					&& Curve_get_cluster_idx(points[i]) == -1)
			{
				/* assign it to the nearest centre */
				double minimum_distance;
				int min_cluster = find_min_cluster(clusters, parameters->number_of_clusters,
						points[i], dists, &minimum_distance, garbage_collector,
						function_caster);
				Cluster_add_point(clusters[min_cluster], points[i]);
			}
		}
	}

	/* clear assignment marks */
	for(i=0; i<num_of_points; i++)
	{
		Curve_set_cluster_idx((Curve) points[i], -1);
		Curve_set_cluster_dist((Curve) points[i], INFINITY);
	}
}
