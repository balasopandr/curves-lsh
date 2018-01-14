#include <stdlib.h>
#include <math.h>
#include "cluster_update.h"
#include "complete_binary_tree.h"
#include "curve_metrics.h"
#include "cluster_helpers.h"

void MeanUpdate(Cluster *clusters, unsigned int num_of_clusters, void del_func(void *), double **dists)
{
	int i;
	for(i=0; i<num_of_clusters; i++)
	{
		int num_of_points = Cluster_get_points_count(clusters[i]);
		if(num_of_points == 0)
		{
			continue;
		}
		del_func(Cluster_get_medoid(clusters[i]));
		List leaves = Cluster_get_points(clusters[i]);
		unsigned int height = log(List_get_length(leaves))/log(2) + 1;

		CompleteBinaryTree cbt = CompleteBinaryTree_create(height, leaves);

		void *new_centre = PostOrderTraversal(cbt, dists);
		Cluster_set_medoid(clusters[i], new_centre, -1);
		CompleteBinaryTree_delete(cbt);
	}
}

void PAMUpdate(Cluster **clusters, Params parameters, void **points,
		unsigned int number_of_points, double **dists,
		void (*garbage_collector)(double **, unsigned int),
		FunctionCaster function_caster)
{
	double minimum_objective_function = minimization_function(points, number_of_points, (*clusters), parameters,
			dists, garbage_collector, function_caster);
	Cluster *tmp_clusters = malloc(sizeof(Cluster)*parameters->number_of_clusters);
	for(int i=0; i<parameters->number_of_clusters; i++)
	{
		tmp_clusters[i] = Cluster_copy((*clusters)[i], function_caster->copy_medoid);
	}
	for(int i=0; i<parameters->number_of_clusters; i++)
	{
		List cluster_points = Cluster_get_points(tmp_clusters[i]);
		Node cur_node = List_get_first_node(cluster_points);
		while(cur_node != NULL)
		{
			void *medoid = Cluster_get_medoid(tmp_clusters[i]);
			Cluster_set_medoid(tmp_clusters[i], Node_get_data(cur_node),
					function_caster->get_object_index(Node_get_data(cur_node)));
			Node_set_data(cur_node, medoid);
			double cur_objective_function = minimization_function(points, number_of_points, tmp_clusters,
					parameters, dists, garbage_collector, function_caster);
			if(cur_objective_function < minimum_objective_function)
			{
				minimum_objective_function = cur_objective_function;
				Cluster_delete((*clusters)[i]);
				(*clusters)[i] = Cluster_copy(tmp_clusters[i], function_caster->copy_medoid);
			}
			cur_node = Node_get_next(cur_node);
		}
	}
	destroy_clusters(tmp_clusters, parameters->number_of_clusters);
}
