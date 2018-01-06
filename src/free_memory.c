#include <stdlib.h>
#include "free_memory.h"

/* free's the memory from all the strings allocated from stdin */
void free_strings(Params parameters, bool delete_input, bool delete_config,
		bool delete_output, bool delete_metric)
{
	free_string(parameters->input_file, delete_input);
	parameters->input_file = NULL;
	free_string(parameters->configuration_file, delete_config);
	parameters->configuration_file = NULL;
	free_string(parameters->output_file, delete_output);
	parameters->output_file = NULL;
	free_string(parameters->metric_name, delete_metric);
	parameters->metric_name = NULL;
}

void free_string(char *string, bool delete_string)
{
	if(delete_string && string!=NULL)
	{
		free(string);
		string = NULL;
	}
}

void free_shift_values(double ***shift_values, Params parameters)
{
	int i;
	int j;
	for(i=0; i<parameters->number_of_hash_tables; i++)
	{
		for(j=0; j<parameters->number_of_grid_curves; j++)
		{
			free(shift_values[i][j]);
			shift_values[i][j] = NULL;
		}
		free(shift_values[i]);
		shift_values[i] = NULL;
	}
	free(shift_values);
	shift_values = NULL;
}

void free_curves(Curve *curves, unsigned int number_of_curves, List curves_list)
{
	int i;
	for(i=0; i<number_of_curves; i++)
	{
		Curve_delete(curves[i]);
		curves[i] = NULL;
	}
	free(curves);
	curves = NULL;
	List_destroy(curves_list, NULL);
}

void free_hashes(HashTable *hashes, Params parameters)
{
	int i;
	for(i=0; i<parameters->number_of_hash_tables; i++)
	{
		void hdata_destroy(void *data)
		{
			HashData hdata = (HashData) data;
			HashData_destroy(hdata);
		}
		HashTable_destroy(hashes[i], &hdata_destroy);
	}
	free(hashes);
	hashes = NULL;
}

void free_clusters(Cluster *clusters, Params parameters)
{
	int m;
	for(m=0; m<parameters->number_of_clusters; m++)
	{
		Curve_del_mean_curve(Cluster_get_medoid(clusters[m]));
		Cluster_delete(clusters[m]);
	}
	free(clusters);
	clusters = NULL;
}

void free_double_array(double **array, unsigned int size)
{
		int i;
		for(i=0; i<size; i++)
		{
			free(array[i]);
			array[i] = NULL;
		}
		free(array);
		array = NULL;
}
