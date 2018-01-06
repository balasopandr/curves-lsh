#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "./curve_metrics_test.h"
#include "./mean_curve.h"
#include "../src/curve.h"
#include "../src/curve_metrics.h"
#include "../src/data_parser.h"
#include "../src/helper_functions.h"

unsigned int dimension = 2;
int tests_run = 0;

void print_test_result( char *test_name,  char *result)
{
	if(result != NULL)
	{
		printf("%s\n", result);
	}
	else
	{
		printf("%s test was successful\n", test_name);
	}
}

void run_tests(Curve *curves)
{
	char *result = NULL;
	Curve P = curves[0];
	Curve Q = curves[1];
	Curve S = curves[2];

	result = frechet_identity_test(P);
	tests_run++;
	print_test_result("identity", result);

	result = frechet_non_negativity_test(P, Q);
	tests_run++;
	print_test_result("non-negativity", result);

	result = frechet_symmetry_test(P, Q);
	tests_run++;
	print_test_result("symmetry", result);

	result = frechet_triangle_inequality(P, Q, S);
	tests_run++;
	print_test_result("triangle inequality", result);

	result = MeanCurve_distance_check(P, Q);
	tests_run++;
	print_test_result("mean curve distance check", result);
}

int main(void)
{
	FILE *dataset = open_or_die("./datasets/tiny_input", "r", "Couldn't open input file");
	unsigned int number_of_curves = read_curve_count(dataset);
	dataset = open_or_die("./datasets/tiny_input", "r", "Couldn't open input file");
	int max_curve_points = 0;
	Curve *curves = read_curves(dataset, number_of_curves, &max_curve_points);

	run_tests(curves);

	int i;
	for(i=0; i<number_of_curves; i++)
	{
		Curve_delete(curves[i]);
	}
	free(curves);
	curves = NULL;

	return 0;
}
