#ifndef __FREE_MEMORY__
#define __FREE_MEMORY__
#include "command_line_parser.h"
#include "list.h"
#include "hash_table.h"
#include "hash_data.h"
#include "cluster.h"

void free_strings(Params parameters, bool delete_input, bool delete_config,
		bool delete_output, bool delete_metric);
void free_string(char *string, bool delete_string);
void free_shift_values(double ***shift_values, Params parameters);
void free_curves(Curve *curves, unsigned int number_of_curves, List curves_list);
void free_hashes(HashTable *hashes, Params parameters);
void free_clusters(Cluster *clusters, Params parameters);
void free_double_array(double **array, unsigned int size);
#endif
