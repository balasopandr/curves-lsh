#ifndef __OUTPUT__
#define __OUTPUT__
#include "command_line_parser.h"
#include "cluster.h"
#include "function_caster.h"

void print_output(Cluster *clusters, Params parameters, double *silhouette_cluster,
		double silhouette_average, FunctionCaster function_caster);
#endif
