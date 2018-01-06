#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "curve.h"
#include "global_defs.h"
#include "helper_functions.h"
#include "command_line_parser.h"
#include "free_memory.h"
#include "main_functions.h"
#include "data_parser.h"
#include "curve_metrics.h"

unsigned int dimension = 2;

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
	unsigned int number_of_curves = read_curve_count(dataset);
	dataset = open_or_die(parameters->input_file, "r", "Couldn't open input file");
	int max_curve_points = 0;
	Curve *curves = read_curves(dataset, number_of_curves, &max_curve_points);
	List curves_list = create_curves_list(curves, number_of_curves);

	unsigned int *rand_for_hash = generate_random_ints(max_curve_points*dimension*
			parameters->number_of_grid_curves);
	double ***shift_values = generate_shift_values(parameters);
	/* build the LSH structure */
	HashTable *hashes = build_hashes(curves_list, parameters, rand_for_hash, shift_values);

	/* calculate all frechet curves */
	double **frechet_dists = calculate_all_frechet_dists(curves, number_of_curves);

	// FILE *tmpf = fopen("./distances.csv", "w");
	// int i,j;
	// for(i=0; i<number_of_curves; i++)
	// {
		// for(j=0; j<number_of_curves; j++)
		// {
			// fprintf(tmpf, "%f", frechet_dists[i][j]);
			// if(j != number_of_curves-1)
			// {
				// fprintf(tmpf, ",");
			// }
		// }
		// fprintf(tmpf, "\n");
	// }
	// fclose(tmpf);
// 
	run_all_combinations(parameters, curves, curves_list, hashes, rand_for_hash, shift_values, frechet_dists);

	free_curves(curves, number_of_curves, curves_list);
	free_double_array(frechet_dists, number_of_curves);
	free_hashes(hashes, parameters);
	free(rand_for_hash);
	rand_for_hash = NULL;
	free_shift_values(shift_values, parameters);
	free_strings(parameters, delete_input, delete_config, delete_output, delete_metric);
	free(parameters);
	return 0;
}
