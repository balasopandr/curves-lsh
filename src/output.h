#ifndef __OUTPUT__
#define __OUTPUT__
#include "command_line_parser.h"
#include "cluster.h"

void print_output(Cluster *clusters, Params parameters, double *silhouette_cluster,
		double silhouette_average, int init_idx, int assign_idx, int update_idx);
#endif
