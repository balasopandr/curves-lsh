#include <stdio.h>
#include <stdlib.h>
#include "pair_int.h"

struct pair_int
{
	int *val;
};

PairInt PairInt_create(int p, int q)
{
	PairInt pair = malloc(sizeof(struct pair_int));
	pair->val = malloc(sizeof(int) * 2);
	pair->val[0] = p;
	pair->val[1] = q;
	return pair;
}

void PairInt_print(PairInt pair, FILE *stream)
{
	fprintf(stream, "(%d, %d)", pair->val[0], pair->val[1]);
}

void PairInt_delete(PairInt pair)
{
	free(pair->val);
	pair->val = NULL;
	free(pair);
	pair = NULL;
}

int * PairInt_get_val(PairInt pair)
{
	return pair->val;
}
