#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "main_functions.h"
#include "curve_metrics.h"
#include "curve.h"
#include "helper_functions.h"
#include "silhouette.h"
#include "output.h"
#include "free_memory.h"
#include "global_defs.h" 
#include "cluster_helpers.h"
#include "function_caster.h"

extern unsigned int dimension;

/* reads filename from stdin *
 * returns true if space was allocated for the new string
 */
bool read_filename(char **filename, char *message)
{
	if(*filename == NULL)
	{
		char buf[BUFSIZE];
		printf(message);
		fgets(buf, BUFSIZE, stdin);
		buf[strlen(buf)-1] = '\0';
		*filename = malloc(sizeof(char) * (strlen(buf) + 1));
		strncpy(*filename, buf, strlen(buf)+1);
		return true;
	}
	return false;
}

bool check_metric_func(double (** metric_func)(Curve, Curve, double ***), char **metric_name)
{
	if(*metric_func == NULL)
	{
		char *buf = malloc(sizeof(char)*BUFSIZE);
		printf("Enter curve distance metric:\n");
		fgets(buf, BUFSIZE, stdin);
		if(strncmp(buf, "DFT", 3) == 0)
		{
			*metric_name = buf;
			*metric_func = &Frechet_distance;
			return true;
		}
		else if(strncmp(buf, "CRMSD", 5) ==0 
				|| strncmp(buf, "cRMSD", 5) ==0)
		{
			*metric_name = buf;
			*metric_func = &CRMSD_curve_dist;
		}
		else
		{
			exit(EXIT_FAILURE);
		}
	}
	return false;
}

/* Generate random integers for the hash function */
unsigned int * generate_random_ints(unsigned int size)
{
	unsigned int *rand_for_hash = malloc(sizeof(unsigned int) * size);
	int i;
	for(i=0; i<size; i++)
	{
		rand_for_hash[i] = rand();
	}
	return rand_for_hash;
}

/* generate random shift values for the grids*/
double *** generate_shift_values(Params parameters)
{
	double ***shift_values = malloc(sizeof(double **) * parameters->number_of_hash_tables);
	int i = 0;
	for(i=0; i<parameters->number_of_hash_tables; i++)
	{
		shift_values[i] = create_urandom_vectors(parameters->number_of_grid_curves, 0, dimension);
	}
	return shift_values;
}

List create_curves_list(Curve *curves, unsigned int number_of_curves)
{
	List curves_list = List_init();
	int i;
	for(i=0; i<number_of_curves; i++)
	{
		List_insert(curves_list, curves[i]);
	}
	return curves_list;
}

void execute_clustering(Params parameters, Curve *curves, List curves_list,
		HashTable *hashes, unsigned int *rand_for_hash, double ***shift_values,
		double **dists)
{
	unsigned int number_of_curves = List_get_length(curves_list);
	FunctionCaster function_caster = FunctionCaster_init(parameters);
	printf("***START LOOP***\n");
	printf("I%dA%dU%d\n", i, j, k);
	Cluster *clusters = malloc(sizeof(Cluster)*parameters->number_of_clusters);
	int m;
	for(m=0; m<parameters->number_of_clusters; m++)
	{
		clusters[m] = Cluster_create(object_points);
	}
	KMeans_random_selection(clusters, parameters->number_of_clusters,
			(void **)(curves), number_of_curves);
	// KMeans_plus_plus(clusters, parameters->number_of_clusters, (void **) curves,
	// number_of_curves, dists);

	double min_func = INFINITY; 
	Cluster *best_clusters = malloc(sizeof(Cluster)*parameters->number_of_clusters);
	for(m=0; m<parameters->number_of_clusters; m++)
	{
		best_clusters[m] = NULL;
	}
	double diff = 0.0;
	int count = 0;
	do
	{
		/* delete previous list of assigned points */
		for(m=0; m<parameters->number_of_clusters; m++)
		{
			Cluster_delete_points(clusters[m]);
		}
		Lloyd_assignment(clusters, (void **) curves, number_of_curves,
				parameters, dists, free_double_array, function_caster);
		// LSH_assignment(clusters, (void **) curves, number_of_curves,
		// parameters, dists, hashes,
		// rand_for_hash, shift_values,
		// &free_double_array, function_caster);

		// MeanUpdate(clusters, parameters->number_of_clusters, function_caster->del_mean_curve, dists); 

		double cur_min_func = minimization_function((void **)curves, number_of_curves,
				clusters, parameters, dists, free_double_array, function_caster);
		diff = (min_func - cur_min_func);
		printf("step %d: target: %f diff: %f, min_func: %f\n", count, THRESHOLD, diff, cur_min_func);
		if(cur_min_func < min_func)
		{
			min_func = cur_min_func;
			for(m=0; m<parameters->number_of_clusters; m++)
			{
				best_clusters[m] = Cluster_copy(clusters[m], function_caster->copy_medoid);
			}
		}
		count++;
	}while(diff >= THRESHOLD && count <= MAX_ITERATIONS);

	/* calculate silhouette */
	double *silhouette_cluster;
	double silhouette_average = calculate_silhouette(clusters, parameters,
			&silhouette_cluster, dists, free_double_array, function_caster);

	/* print output */
	print_output(best_clusters, parameters, silhouette_cluster, 
			silhouette_average, i, j, k);
	free_clusters(clusters, parameters);
	free_clusters(best_clusters, parameters);
	free(silhouette_cluster);
	silhouette_cluster = NULL;
	printf("***END LOOP***\n");
	FunctionCaster_delete(function_caster);
}

// void run_all_combinations(Params parameters, Curve *curves, List curves_list,
// HashTable *hashes, unsigned int *rand_for_hash, double ***shift_values,
// double **dists)
// {
// unsigned int number_of_curves = List_get_length(curves_list);
// int i,j,k;
// FunctionCaster function_caster = FunctionCaster_init(parameters);
// for(i=0; i<2; i++)
// {
// for(j=0; j<2; j++)
// {
// for(k=0; k<2; k++)
// {
// if(k==1 ) continue;
// 
// printf("***START LOOP***\n");
// printf("I%dA%dU%d\n", i, j, k);
// Cluster *clusters = malloc(sizeof(Cluster)*parameters->number_of_clusters);
// int m;
// for(m=0; m<parameters->number_of_clusters; m++)
// {
// clusters[m] = Cluster_create(object_points);
// }
// if(i==0)
// {
// KMeans_random_selection(clusters, parameters->number_of_clusters,
// (void **)(curves), number_of_curves);
// }
// else
// {
// KMeans_plus_plus(clusters, parameters->number_of_clusters, (void **) curves,
// number_of_curves, dists);
// }
// 
// double min_func = INFINITY; 
// Cluster *best_clusters = malloc(sizeof(Cluster)*parameters->number_of_clusters);
// for(m=0; m<parameters->number_of_clusters; m++)
// {
// best_clusters[m] = NULL;
// }
// double diff = 0.0;
// int count = 0;
// do
// {
// /* delete previous list of assigned points */
// for(m=0; m<parameters->number_of_clusters; m++)
// {
// Cluster_delete_points(clusters[m]);
// }
// if(j==0)
// {
// Lloyd_assignment(clusters, (void **) curves, number_of_curves,
// parameters, dists, free_double_array, function_caster);
// }
// else
// {
// LSH_assignment(clusters, (void **) curves, number_of_curves,
// parameters, dists, hashes,
// rand_for_hash, shift_values,
// &free_double_array, function_caster);
// }
// 
// if(k==0)
// {
// MeanUpdate(clusters, parameters->number_of_clusters, function_caster->del_mean_curve, dists); 
// }
// else
// {
// /* not implemented */
// printf("PAM not implemented\n");
// }
// 
// double cur_min_func = minimization_function((void **)curves, number_of_curves,
// clusters, parameters, dists, free_double_array, function_caster);
// diff = (min_func - cur_min_func);
// printf("step %d: target: %f diff: %f, min_func: %f\n", count, THRESHOLD, diff, cur_min_func);
// if(cur_min_func < min_func)
// {
// min_func = cur_min_func;
// for(m=0; m<parameters->number_of_clusters; m++)
// {
// best_clusters[m] = Cluster_copy(clusters[m], function_caster->copy_medoid);
// }
// }
// count++;
// }while(diff >= THRESHOLD && count <= MAX_ITERATIONS);
// 
// /* calculate silhouette */
// double *silhouette_cluster;
// double silhouette_average = calculate_silhouette(clusters, parameters,
// &silhouette_cluster, dists, free_double_array, function_caster);
// 
// /* print output */
// print_output(best_clusters, parameters, silhouette_cluster, 
// silhouette_average, i, j, k);
// free_clusters(clusters, parameters);
// free_clusters(best_clusters, parameters);
// free(silhouette_cluster);
// silhouette_cluster = NULL;
// printf("***END LOOP***\n");
// }
// }
// }
// FunctionCaster_delete(function_caster);
// }

double ** calculate_all_dists(Curve *curves, unsigned int num_of_curves, double (*dist_func)(Curve, Curve, double ***))
{
	double **dists = malloc(sizeof(double *)*num_of_curves);
	int i;
	for(i=0; i<num_of_curves; i++)
	{
		printf("calculating dist for curve %d\n", i);
		dists[i] = malloc(sizeof(double)*num_of_curves);
		int j;
		for(j=0; j<i+1; j++)
		{
			double **dists = NULL;
			double dist = dist_func(curves[i], curves[j], &dists);
			dists[i][j] = dist;
			dists[j][i] = dist;
			int k;
			if(dists != NULL)
			{
				for(k=0; k<Curve_get_points_count(curves[i]); k++)
				{
					free(dists[k]);
					dists[k] = NULL;
				}
				free(dists);
				dists = NULL;
			}
		}
	}
	return dists;
}
