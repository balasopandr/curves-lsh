#ifndef __VECTOR__
#define __VECTOR__
#include "global_defs.h"

typedef struct vec *Vector;

Vector Vector_init(double **points, unsigned int length);
void Vector_resize(Vector vector, unsigned int new_length);
void Vector_concat(Vector v1, Vector v2);
void Vector_print(Vector vector, FILE *stream);
void Vector_destroy(Vector vector);
bool Vector_equal(Vector v1, Vector v2);

unsigned int Vector_get_length(Vector vector);
double ** Vector_get_points(Vector vector);
#endif
