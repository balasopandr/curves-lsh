#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <lapacke/lapacke.h>
#include "lapacke_helpers.h"
#include "lapacke_helpers.h"
#include "helper_functions.h"
#include "global_defs.h"
#include "data_parser.h"
#include "curve.h"
#include "command_line_parser.h"
#include "main_functions.h"
#include "free_memory.h"
#include "crmsd.h"

unsigned int dimension = 3;

int main(int argc, char **argv)
{
	srand(time(NULL));
	Params parameters = command_line_parser(argc, argv);
	bool delete_input = read_filename(&parameters->input_file, "Enter input filename:\n");
	bool delete_config = read_filename(&parameters->configuration_file, "Enter config filename:\n");
	bool delete_output = read_filename(&parameters->output_file, "Enter output filename:\n");
	bool delete_metric = check_metric_func(&parameters->metric_func, &parameters->metric_name);

	FILE *config = open_or_die(parameters->configuration_file, "r", "Couldn't open config file");
	parse_config_file(config, parameters);

	FILE *dataset = open_or_die(parameters->input_file, "r", "Couldn't open input file");
	unsigned int number_of_conformations;
	Curve *conformations = read_conformations(dataset, &number_of_conformations);

	// double **dists = calculate_all_dists(conformations, number_of_conformations,
			// parameters->metric_func);

	// double dist = CRMSD_curve_dist(conformations[0], conformations[1], NULL);
	// printf("I found dist: %f\n", dist);
	// Curve_print(conformations[0], stdout);

	free_strings(parameters, delete_input, false, delete_output, delete_metric);
	free_curves(conformations, number_of_conformations, NULL);
	free_double_array(dists, number_of_conformations);
	free(parameters);
	parameters = NULL;
	return 0;
}
