#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "curve_metrics.h"
#include "helper_functions.h"
#include "pair_int.h"
#include "free_memory.h"

extern unsigned int dimension;

double Euclidean_distance(double *pointA, double *pointB)
{
	double distance = 0.0;
	int i;

	for(i=0; i<dimension; i++)
	{
		distance += sqrt(pow(fabs(pointA[i] - pointB[i]), 2.0));
	}

	return distance;
}

double Frechet_distance(Curve P, Curve Q, double ***distances)
{
	int i;
	int j;

	(*distances) = malloc(sizeof(double *) * Curve_get_points_count(P));

	for(i=0; i<Curve_get_points_count(P); i++)
	{
		(*distances)[i] = malloc(sizeof(double) * Curve_get_points_count(Q));
	}

	(*distances)[0][0] = Euclidean_distance(Curve_get_points(P)[0], Curve_get_points(Q)[0]);
	for(i=0; i<Curve_get_points_count(P); i++)
	{
		for(j=0; j<Curve_get_points_count(Q); j++)
		{
			if(j>0 && i==0)
			{
				(*distances)[0][j] = fmax((*distances)[0][j-1], Euclidean_distance(Curve_get_points(P)[0], Curve_get_points(Q)[j]));
			}
			else if(i>0 && j==0)
			{
				(*distances)[i][0] = fmax((*distances)[i-1][0], Euclidean_distance(Curve_get_points(P)[i], Curve_get_points(Q)[0]));
			}
			else if(j>0 && j>0)
			{
				(*distances)[i][j] = fmax(fmin(fmin((*distances)[i-1][j], (*distances)[i-1][j-1]), (*distances)[i][j-1]),
						Euclidean_distance(Curve_get_points(P)[i], Curve_get_points(Q)[j]));
			}
		}
	}

	double result = (*distances)[Curve_get_points_count(P)-1][Curve_get_points_count(Q)-1];

	return result;
}

List OptimalTraversal(unsigned int m, unsigned int n, double **distances)
{
	List traversal = List_init();
	unsigned int p = m-1;
	unsigned int q = n-1;

	int arg_min(int p, int q, double **dists)
	{
		if(p==0 && q==0) return 3;
		if(p==0) return 1;
		if(q==0) return 0;
		double min_dist = fmin(fmin(dists[p-1][q], dists[p][q-1]), dists[p-1][q-1]);

		if(dists[p-1][q] == min_dist)
		{
			return 0;
		}
		else if(dists[p][q-1] == min_dist)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}

	List_insert(traversal, PairInt_create(p,q));
	int count = 0;
	while(p!=-1 && q!=-1)
	{
		count++;
		int min_idx = arg_min(p,q,distances);
		if (min_idx == 0)
		{
			List_insert_front(traversal, PairInt_create(--p, q));
		}
		else if (min_idx == 1)
		{
			List_insert_front(traversal, PairInt_create(p, --q));
		}
		else if(min_idx == 2)
		{
			List_insert_front(traversal, PairInt_create(--p, --q));
		}
		else
		{
			p--;
			q--;
		}
	}

	return traversal;
}

Curve MeanFrechetCurve(Curve P, Curve Q, List traversal)
{
	Node cur_node = List_get_first_node(traversal);
	int i,j;

	if(Q == NULL)
	{
		return P;
	}

	double **points = malloc(sizeof(double *)*List_get_length(traversal));
	for(j=0; j<List_get_length(traversal); j++)
	{
		points[j] = malloc(sizeof(double)*dimension);
	}
	i = 0;
	while(cur_node != NULL)
	{
		PairInt cur_pair = (PairInt) Node_get_data(cur_node);
		int *val = PairInt_get_val(cur_pair);
		for(j=0; j<dimension; j++)
		{
			points[i][j] = (Curve_get_points(P)[val[0]][j] + Curve_get_points(Q)[val[1]][j])/2;
		}
		cur_node = Node_get_next(cur_node);
		i++;
	}
	return Curve_create("mean curve", List_get_length(traversal), points, -1);
}

Curve PostOrderTraversal(CompleteBinaryTree node, double **dists)
{
	if(CompleteBinaryTree_isLeaf(node))
	{
		return (Curve) CompleteBinaryTree_get_data(node);
	}
	else
	{
		Curve left_curve = PostOrderTraversal(CompleteBinaryTree_get_left_child(node), dists);
		Curve right_curve;
		if(CompleteBinaryTree_get_right_child(node) != NULL)
		{
			right_curve = PostOrderTraversal(CompleteBinaryTree_get_right_child(node), dists);
		}
		else
		{
			right_curve = NULL;
		}
		/* calculate mean discrete frechet curve */
		double **distances;
		Frechet_distance(left_curve, right_curve, &distances);
		List traversal = OptimalTraversal(Curve_get_points_count(left_curve),
				Curve_get_points_count(right_curve), distances);
		Curve mean_curve = MeanFrechetCurve(left_curve, right_curve, traversal);
		void del_func(void *par)
		{
			PairInt_delete((PairInt)par);
		}
		List_destroy(traversal, &del_func);
		free_double_array(distances, Curve_get_points_count(left_curve));
		distances = NULL;
		/* free intermediate mean curves */
		if(left_curve != NULL 
				&& strncmp(Curve_get_id(left_curve),
					"mean curve", strlen("mean curve")+1) == 0)

		{
			Curve_delete(left_curve);
		}
		if(right_curve != NULL 
				&& strncmp(Curve_get_id(right_curve),
					"mean curve", strlen("mean curve")+1) == 0)

		{
			Curve_delete(right_curve);
		}

		return mean_curve;
	}
}
