#include <lapacke/lapacke.h>
#include <math.h>
#include "crmsd.h"
#include "curve_metrics.h"
#include "lapacke_helpers.h"
#include "helper_functions.h"
#include "global_defs.h"

static double * array_Ac(double *array, int size);
static void array_subtractAc(double *array, int size, double *Ac);
static Curve convert_matrix_to_curve(double *array, int rows, int cols);
extern unsigned int dimension;

double CRMSD_curve_dist(Curve P, Curve Q, double ***distances)
{
	double **pointsP = Curve_get_points(P);
	double **pointsQ = Curve_get_points(Q);
	unsigned int sizeX = Curve_get_points_count(P)*dimension;
	unsigned int sizeY = Curve_get_points_count(Q)*dimension;
	double *X = convert_matrix(pointsP, sizeX/dimension, dimension);
	double *Y = convert_matrix(pointsQ, sizeY/dimension, dimension);
	double dist = CRMSD_dist(X, sizeX, Y, sizeY);
	free(X);
	X = NULL;
	free(Y);
	Y = NULL;
	return dist;
}

double CRMSD_dist(double *X, int sizeX, double *Y, int sizeY)
{
	/* send copies of X and Y */
	double *newX = array_copy(X, sizeX);
	double *newY = array_copy(Y, sizeY);
	double *Q = CRMSD_translate_and_rotate(newX, sizeX, newY, sizeY);
	double *result = NULL;
	LAPACKE_multiplication(newX, sizeX/3, 3, Q, 3, 3, &result);
	double distance = 0.0;
	for(int i=0; i<sizeX/3; i++)
	{
		for(int j=0; j<3; j++)
		{
			int u = (sizeX/3)*j + i;
			distance += SQR(result[u] - newY[u]);
		}
	}
	distance /= sizeX/3;
	distance = sqrt(distance);

	LAPACKE_free(Q);
	Q = NULL;
	LAPACKE_free(result);
	result = NULL;

	free(newX);
	newX = NULL;
	free(newY);
	newY = NULL;

	return distance;
}

double * CRMSD_translate_and_rotate(double *X, int sizeX, double *Y, int sizeY)
{
	double *Xc = array_Ac(X, sizeX);
	double *Yc = array_Ac(Y, sizeY);

	array_subtractAc(X, sizeX, Xc);
	array_subtractAc(Y, sizeY, Yc);


	free(Xc);
	Xc = NULL;
	free(Yc);
	Yc = NULL;

	double *transposeX = array_transpose(X, sizeX/3, 3);
	double *XtY;
	LAPACKE_multiplication(transposeX, sizeX/3, 3, Y, sizeY/3, 3, &XtY);

	double *U = NULL;
	double *S = NULL;
	double *V = NULL;
	LAPACKE_svd(XtY, 3, 3, &U, &S, &V);
	double *Q;
	LAPACKE_multiplication(U, 3, 3, V, 3, 3, &Q);
	if(LAPACKE_determinant(Q, 3) < 0)
	{
		for(int i=6; i<9; i++)
		{
			U[i] = -U[i];
		}
		LAPACKE_free(Q);
		Q = NULL;
		LAPACKE_multiplication(U, 3, 3, V, 3, 3, &Q);
	}
	LAPACKE_free(XtY);
	XtY = NULL;
	free(transposeX);
	transposeX = NULL;
	LAPACKE_free(U);
	U = NULL;
	LAPACKE_free(S);
	S = NULL;
	LAPACKE_free(V);
	V = NULL;
	return Q;
}

double *array_Ac(double *array, int size)
{
	double *Ac = malloc(sizeof(double)*3);
	int points_count = size/3;
	for(int i=0; i<3; i++)
	{
		Ac[i] = 0.0;
	}
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<points_count; j++)
		{
			Ac[i] += array[i*points_count +j];
		}
		Ac[i] /= points_count;
	}
	return Ac;
}

void array_subtractAc(double *array, int size, double *Ac)
{
	int points_count = size/3;
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<points_count; j++)
		{
			int x = i*points_count + j;
			array[x] -= Ac[i];
		}
	}
}

double CRMSD_frechet_dist(Curve Pc, Curve Qc, double ***dists)
{
	double **pointsPc = Curve_get_points(Pc);
	double **pointsQc = Curve_get_points(Qc);
	unsigned int sizeX = Curve_get_points_count(Pc)*dimension;
	unsigned int sizeY = Curve_get_points_count(Qc)*dimension;
	double *X = convert_matrix(pointsPc, sizeX/dimension, dimension);
	double *Y = convert_matrix(pointsQc, sizeY/dimension, dimension);

	/* send copies of X and Y */
	double *newX = array_copy(X, sizeX);
	double *newY = array_copy(Y, sizeY);
	double *Q = CRMSD_translate_and_rotate(newX, sizeX, newY, sizeY);
	double *XQ = NULL;
	LAPACKE_multiplication(newX, sizeX/3, 3, Q, 3, 3, &XQ);
	Curve XQ_curve = convert_matrix_to_curve(XQ, sizeX/3, 3);
	Curve Y_curve = convert_matrix_to_curve(Y, sizeY/3, 3);

	double distance = Frechet_distance(XQ_curve, Y_curve, dists);

	Curve_delete(XQ_curve);
	Curve_delete(Y_curve);
	LAPACKE_free(Q);
	Q = NULL;
	LAPACKE_free(XQ);
	XQ = NULL;

	free(newX);
	newX = NULL;
	free(newY);
	newY = NULL;

	return distance;
}

/* assuming column major */
Curve convert_matrix_to_curve(double *array, int rows, int cols)
{
	Curve new_curve;
	double **curve_points = malloc(sizeof(double *)*rows);
	for(int i=0; i<rows; i++)
	{
		curve_points[i] = malloc(sizeof(double)*cols);
	}
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			curve_points[i][j] = array[j*rows + i];
		}
	}
	new_curve = Curve_create("XQ", rows, curve_points, -2);
	return new_curve;
}
