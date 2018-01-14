#ifndef __CURVE_LSH__
#define __CURVE_LSH__
#include "curve.h"
#include "vector.h"
#include "list.h"
#include "hash_data.h"
#include "hash_table.h"
#include "command_line_parser.h"

HashTable * build_hashes(List curve_list, Params parameters, unsigned int *rand_for_hash, double ***shift_values,
		unsigned int elements_per_bucket);
#endif
