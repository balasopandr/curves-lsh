#ifndef __PAIR_INT__
#define __PAIR_INT__

typedef struct pair_int *PairInt;

PairInt PairInt_create(int p, int q);
void PairInt_print(PairInt pair, FILE *stream);
void PairInt_delete(PairInt pair);
int * PairInt_get_val(PairInt pair);
#endif
