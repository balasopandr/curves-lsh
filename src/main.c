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
	List conformations_list = create_curves_list(conformations, number_of_conformations);

	/* generate hashes */
	unsigned int *rand_for_hash = generate_random_ints(Curve_get_points_count(conformations[0])
			*dimension*parameters->number_of_grid_curves);
	double ***shift_values = generate_shift_values(parameters);
	/* build the LSH structure */
	HashTable *hashes = build_hashes(conformations_list, parameters, rand_for_hash, shift_values,
			number_of_conformations/parameters->number_of_clusters);

	 double **dists = calculate_all_dists(conformations, number_of_conformations,
			 parameters->metric_func);

	execute_clustering(parameters, conformations, conformations_list, hashes, rand_for_hash, shift_values, dists);

	free_hashes(hashes, parameters);
    free(rand_for_hash);
    rand_for_hash = NULL;
    free_shift_values(shift_values, parameters);
	free_strings(parameters, delete_input, delete_config, delete_output, delete_metric);
	free_curves(conformations, number_of_conformations, NULL);
	free_double_array(dists, number_of_conformations);
	List_destroy(conformations_list, NULL);
	free(parameters);
	parameters = NULL;
	return 0;
}
