#ifndef __HASH_DATA__
#define __HASH_DATA__
#include "curve.h"
#include "vector.h"
#include "list.h"
typedef struct curve_vector *HashData;

HashData HashData_init(Curve curve, Vector grid_vector);
void HashData_destroy(HashData hdata);
unsigned int classic_hash(void *data, void *parameters);
Curve HashData_get_curve(HashData hdata);
Vector HashData_get_grid_vector(HashData hdata);
unsigned int HashData_get_hash_value(HashData hdata);
void HashData_set_hash_value(HashData hdata, unsigned int hash_value);
#endif
