#include <stdio.h>
#include "global_defs.h"
#include "output.h"
#include "curve.h"
#include "helper_functions.h"


void print_output(Cluster *clusters, Params parameters, double *silhouette_cluster,
		double silhouette_average)
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
		fprintf(output, "%d", Cluster_get_medoid_idx(clusters[m]));
		List points = Cluster_get_points(clusters[m]);
		Node cur_node = List_get_first_node(points);
		while(cur_node != NULL)
		{
			fprintf(output, "\t");
			fprintf(output, "%d", Curve_get_curve_idx((Curve) Node_get_data(cur_node)));
			cur_node = Node_get_next(cur_node);
		}
		fprintf(output, "\n");
	}
	fclose(output);
}
