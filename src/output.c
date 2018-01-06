#include <stdio.h>
#include "output.h"
#include "curve.h"
#include "helper_functions.h"

void print_output(Cluster *clusters, Params parameters, double *silhouette_cluster,
		double silhouette_average, int init_idx, int assign_idx, int update_idx)
{
	FILE *output = open_or_die(parameters->output_file, "a",
			"Couldn't open output file");
	fprintf(output, "Algorithm: I%dA%dU%d\n", init_idx, assign_idx, update_idx);
	fprintf(output, "Metric: %s\n", parameters->metric_name);

	int m;
	for(m=0; m<parameters->number_of_clusters; m++)
	{
		fprintf(output, "CLUSTER-%d {size: %d, silhouette: %f, centroid: ", m,
				Cluster_get_points_count(clusters[m]), silhouette_cluster[m]);
		Curve_print(Cluster_get_medoid(clusters[m]), output);
		fprintf(output, "}\n");
	}
	fprintf(output, "overall silhouette: %f\n", silhouette_average);
	fclose(output);
}
