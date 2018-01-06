#ifndef __COMMAND_LINE_PARSER__
#define __COMMAND_LINE_PARSER__
#include "global_defs.h"
#include "curve.h"

struct par
{
	char *input_file;
	char *configuration_file;
	unsigned int number_of_grid_curves;
	unsigned int number_of_hash_tables;
	char *output_file;
	char *metric_name;
	double (* metric_func)(Curve, Curve, double ***); /* curve metric {DFT, DTW} */
	unsigned int (* hash_func)(void *, void *); /* hash function {classic, probabilistic} */
	double grid_density;
	unsigned int number_of_clusters; /* number of clusters */
};

typedef struct par *Params;

Params command_line_parser(int argc, char **argv);
#endif
