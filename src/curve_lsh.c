#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "free_memory.h"
#include "curve_lsh.h"
#include "grid.h"
#include "helper_functions.h"
#include "data_parser.h"
#include "nearest_curve.h"
#include "main_functions.h"

extern unsigned int dimension;
#define ELEMENTS_PER_BUCKET 90

HashTable * build_hashes(List curve_list, Params parameters, unsigned int *rand_for_hash, double ***shift_values)
{
	/* create L hashtables */
	HashTable *hashes = malloc(sizeof(HashTable)*parameters->number_of_hash_tables);
	int hash_count;
	int max_vector_points = 0;

	/* insert curves and grid curves in the hashtables */
	for(hash_count=0; hash_count<parameters->number_of_hash_tables; hash_count++)
	{
		List vector_list = List_init();

		Node curnode = List_get_first_node(curve_list);
		while(curnode != NULL)
		{
			/* create grid curve */
			Vector cur_vec = Grid_build_vector(parameters->grid_density,
					shift_values[hash_count],
					(Curve) Node_get_data(curnode),
					parameters->number_of_grid_curves,
					&max_vector_points);
			List_insert(vector_list, cur_vec);
			curnode = Node_get_next(curnode); 
		}

		/* calculate hash value and insert curves to the hashtable */
		hashes[hash_count] = HashTable_init(List_get_length(curve_list)/ELEMENTS_PER_BUCKET);
		Node curcurve = List_get_first_node(curve_list);
		Node curgridvec = List_get_first_node(vector_list);
		while(curcurve != NULL)
		{
			HashData hash_data = HashData_init((Curve) Node_get_data(curcurve), (Vector) Node_get_data(curgridvec));
			HashTable_insert(hashes[hash_count], hash_data, rand_for_hash, parameters->hash_func);
			curcurve = Node_get_next(curcurve);
			curgridvec = Node_get_next(curgridvec);
		}
		List_destroy(vector_list, NULL);
	}
	return hashes;
}
