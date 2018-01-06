#ifndef __CURVE_METRICS_TEST__
#define __CURVE_METRICS_TEST__
#include "../src/curve.h"
char * frechet_identity_test(Curve P);
char * frechet_non_negativity_test(Curve P, Curve Q);
char * frechet_symmetry_test(Curve P, Curve Q);
char * frechet_triangle_inequality(Curve P, Curve Q, Curve S);

#endif
