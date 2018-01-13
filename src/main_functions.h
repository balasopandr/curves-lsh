#ifndef __MAIN__FUNCTIONS__
#define __MAIN__FUNCTIONS__
#include <stdio.h>
#include "global_defs.h"
#include "data_parser.h"
#include "command_line_parser.h"
#include "list.h"
#include "hash_table.h"
#include "cluster.h"
#include "cluster_initialization.h"
#include "cluster_assignment.h"
#include "curve_lsh.h"
#include "cluster_update.h"

bool read_filename(char **filename, char *message);
bool check_metric_func(double (** metric_func)(Curve, Curve, double ***), char **metric_name);
unsigned int * generate_random_ints(unsigned int size);
double *** generate_shift_values(Params parameters);
List create_curves_list(Curve *curves, unsigned int number_of_curves);
unsigned int object_points(void *point);
double point_dist(void *P, void *Q, double ***array);
void del_point(void *point);
void execute_clustering(Params parameters, Curve *curves, List curves_list,
		HashTable *hashes, unsigned int *rand_for_hash, double ***shift_values,
		double **dists);
// void run_all_combinations(Params parameters, Curve *curves, List curves_list,
		// HashTable *hashes, unsigned int *rand_for_hash, double ***shift_values,
		// double **dists);
double ** calculate_all_dists(Curve *curves, unsigned int num_of_curves, double (*dist_func)(Curve, Curve, double ***));
#endif
