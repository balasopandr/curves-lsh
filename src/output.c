#include <stdio.h>
#include <stdlib.h>
#include "global_defs.h"
#include "output.h"
#include "curve.h"
#include "helper_functions.h"
#include "quicksort.h"


void print_output(Cluster *clusters, Params parameters, double *silhouette_cluster,
		double silhouette_average, FunctionCaster function_caster)
{
	double variance = 0.0;
	for(int i=0; i<parameters->number_of_clusters; i++)
	{
		variance += SQR(silhouette_cluster[i]);
	}
	variance /= parameters->number_of_clusters;
	variance -= SQR(silhouette_average);

	FILE *output = open_or_die(parameters->output_file, "a",
			"Couldn't open output file");
	fprintf(output, "k: %d\n", parameters->number_of_clusters);
	fprintf(output, "s: %f\n", silhouette_average);

	for(int m=0; m<parameters->number_of_clusters; m++)
	{
		int i=0;
		int *sorted_idx = malloc(sizeof(int)*(Cluster_get_points_count(clusters[m])+1));
		sorted_idx[i] = Cluster_get_medoid_idx(clusters[i]);
		i++;

		List points = Cluster_get_points(clusters[m]);
		Node cur_node = List_get_first_node(points);
		while(cur_node != NULL)
		{
			sorted_idx[i] = function_caster->get_object_index(Node_get_data(cur_node));
			cur_node = Node_get_next(cur_node);
			i++;
		}
		quickSort(sorted_idx, 0, Cluster_get_points_count(clusters[m]));
		for(i=0; i<Cluster_get_points_count(clusters[m])+1; i++)
		{
			fprintf(output, "\t%d", sorted_idx[i]);
		}
		fprintf(output, "\n");
		free(sorted_idx);
		sorted_idx = NULL;
	}
	fclose(output);
}
