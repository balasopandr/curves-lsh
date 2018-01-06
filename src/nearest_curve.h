#ifndef __NEAREST_CURVE__
#define __NEAREST_CURVE__
#include "curve.h"
#include "list.h"
#include "hash_table.h"
#include "command_line_parser.h"

Curve NearestCurve_bruteforce(List curve_list, Curve query, double (* curve_metric)(Curve, Curve), double *min_dist);
Curve NearestCurve_LSH(HashTable *hashes, Curve query, Params parameters, unsigned int *rand_for_hash, double ***shift_values, double *min_dist, bool *found_vector);
#endif
