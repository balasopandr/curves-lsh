#include <stdlib.h>
#include <math.h>
#include "cluster_initialization.h"
#include "cluster_helpers.h"
#include "helper_functions.h"
#include "global_defs.h"

void KMeans_random_selection(Cluster *clusters, unsigned int num_of_clusters, 
		void **points, unsigned int num_of_points)
{
	int i,j,u;
	for(i=0; i<num_of_clusters; i++)
	{
		bool found_unique = false;
		do
		{
			u = uniform_random_int(0, num_of_points-1);

			/* search previous clusters so that we don't select the same centre */
			found_unique = true;
			for(j=0; j<i; j++)
			{
				if(points[u] == Cluster_get_medoid(clusters[j]))
				{
					found_unique = false;
					break;
				}
			}
		} while(!found_unique);
		Cluster_set_medoid(clusters[i], points[u], u);
	}
}

void KMeans_plus_plus(Cluster *clusters, unsigned int num_of_clusters, void **points,
		unsigned int num_of_points, double **dists)
{
	int t,j,u;
	/* step 1: choose a centroid uniformly at random */
	u = uniform_random_int(0, num_of_points-1);
	Cluster_set_medoid(clusters[0], points[u], u);
	t = 1;

	double *min_distances = malloc(sizeof(double)*num_of_points);
	for(j=0; j<num_of_points; j++)
	{
		min_distances[j] = INFINITY;
	}
	while(t != num_of_clusters)
	{
		/* step 2: for each point compute the minimum distance with the new centre */
		for(j=0; j<num_of_points; j++)
		{
			double dist = dists[j][Cluster_get_medoid_idx(clusters[t-1])];
			min_distances[j] = fmin(min_distances[j], dist);
		}

		/* step 3: 
		 * compute partial sums, normalize by dividing them by the maximum distance
		 * and store them on an array (or a binary tree) */

		int arg_max;
		double max_dist = max_double_array(min_distances, num_of_points, &arg_max);
		double sum = 0;
		int medoids_so_far = 0;
		PossibleMedoid *possible_medoids = malloc(sizeof(PossibleMedoid)*(num_of_points - t));
		for(j=0; j<num_of_points; j++)
		{
			if(min_distances[j] == 0.0)
			{

				medoids_so_far++;
			}
			else
			{
				sum += pow(min_distances[j], 2)/max_dist;
				possible_medoids[j - medoids_so_far].partial_sum = sum;
				possible_medoids[j - medoids_so_far].point = points[j];
				possible_medoids[j - medoids_so_far].idx = j;
			}
		}

		/* step 4: create intervals based on the partial sums 
		 * pick a random integer on [0, P(n-t)] */
		double r = uniform_random(0, possible_medoids[num_of_points-t-1].partial_sum);
		/* find r by binary search in array P */
		unsigned int new_medoid_idx = interval_binary_search(clusters, num_of_clusters,
				points, num_of_points, possible_medoids, r, 0, num_of_points-1);
		Cluster_set_medoid(clusters[t], points[new_medoid_idx], new_medoid_idx);
		free(possible_medoids);
		possible_medoids = NULL;
		t++;
	}
	free(min_distances);
	min_distances = NULL;
}
