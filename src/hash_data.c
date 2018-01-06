#include <stdlib.h>
#include "hash_data.h"
#include "helper_functions.h"

extern unsigned int dimension;

static unsigned long M = 0xfffffffb;

struct curve_vector
{
	Curve curve;
	Vector grid_vector;
	unsigned int hash_value;
};

HashData HashData_init(Curve curve, Vector grid_vector)
{
	HashData hdata = malloc(sizeof(struct curve_vector));
	hdata->curve = curve;
	hdata->grid_vector = grid_vector;

	return hdata;
}

void HashData_destroy(HashData hdata)
{
	Vector_destroy(hdata->grid_vector);
	free(hdata);
	hdata = NULL;
}

unsigned int classic_hash(void *data, void *parameters)
{
	HashData hdata = (HashData) data;
	int *rand_for_hash = (int *) parameters;
	int i;
	int j;
	Vector vector = hdata->grid_vector;
	double curtotal = 0.0;
	for(i=0; i<Vector_get_length(vector); i++)
	{
		for(j=0; j<dimension; j++)
		{
			curtotal += rand_for_hash[dimension*i + j]*Vector_get_points(vector)[i][j];
		}
	}
	/* convert to int by taking the mantissa bits */
	double_cast cast;
	cast.d = curtotal;
	/* make sure modulo is positive */
	hdata->hash_value = (cast.parts.mantisa % M  + M)%M;
	return hdata->hash_value;
}

Curve HashData_get_curve(HashData hdata)
{
	return hdata->curve;
}
Vector HashData_get_grid_vector(HashData hdata)
{
	return hdata->grid_vector;
}
unsigned int HashData_get_hash_value(HashData hdata)
{
	return hdata->hash_value;
}

void HashData_set_hash_value(HashData hdata, unsigned int hash_value)
{
	hdata->hash_value = hash_value;
}
