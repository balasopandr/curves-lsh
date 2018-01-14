#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_parser.h"

extern unsigned int dimension;

unsigned int read_curve_count(FILE *dataset)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	unsigned int number_of_curves = 0;
	while((read = getline(&line, &len, dataset)) != -1)
	{
		if(strstr(line, "@dimension") != NULL)
		{
			continue;
		}
		else if(strncmp(line, "\n", 2) == 0)
		{
			continue;
		}
		number_of_curves++;
	}
	free(line);
	line = NULL;
	fclose(dataset);
	dataset = NULL;

	return number_of_curves;
}
/* reads a curve from the dataset */
Curve parse_curve_dataset_line(char *line, size_t len, ssize_t read, int *max_curve_points, int curve_idx)
{
	char *str_id = NULL;
	char *str_point_count = NULL;
	char *str_vector = NULL;
	char *tok = NULL;
	int point_count;

	tok = strtok(line, "\t");
	str_id = tok;

	tok = strtok(NULL, "\t");
	str_point_count = tok;

	tok = strtok(NULL, "\t");
	str_vector = tok;

	point_count = atoi(str_point_count);
	double **vector = parse_vector(str_vector, point_count, dimension);

	Curve curve = Curve_create(str_id, atoi(str_point_count), vector, curve_idx);

	if(max_curve_points != NULL)
	{
		if(point_count > *max_curve_points)
		{
			*max_curve_points = point_count;
		}
	}
	vector = NULL;
	return curve;
}

/* reads the points of the curve */
double ** parse_vector(char *str_vector, unsigned int point_count, unsigned int dimension)
{
	double **vector = NULL;

	str_vector[strlen(str_vector)-1] = '\0';

	char *tok = NULL;
	/* assuming points are seperated by commas */
	char *delim = "(, )";
	tok = strtok(str_vector,  delim);

	/* initialize vector */
	vector = malloc(sizeof(double *)*point_count);
	int i;
	for(i=0; i < point_count; i++)
	{
		vector[i] = malloc(sizeof(double)*dimension);
		int j;
		for(j=0; j<dimension; j++)
		{
			vector[i][j] = 0.0;
		}
	}

	i = 0;
	do
	{
		int j;
		for(j=0; j<dimension; j++)
		{
			vector[i][j] = atof(tok);
			if(j!=dimension-1)
			{
				tok = strtok(NULL, delim);
			}
		}
		i++;
	} while((tok = strtok(NULL, delim)) != NULL);


	return vector;
}

/* reads the input dataset */
Curve * read_curves(FILE *dataset, unsigned int number_of_curves, int *max_curve_points)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	Curve *curves = malloc(sizeof(Curve)*number_of_curves);
	int i = 0;
	while((read = getline(&line, &len, dataset)) != -1)
	{
		if(strstr(line, "@dimension") != NULL)
		{
			char *tmp = line + strlen("@dimension") + 1;
			dimension = atoi(tmp);
			continue;
		}
		Curve curve;
		curve = parse_curve_dataset_line(line, len, read, max_curve_points, i);
		curves[i] = curve;
		i++;
	}
	free(line);
	line = NULL;
	fclose(dataset);
	dataset = NULL;

	return curves;
}


void parse_config_file(FILE *config, Params parameters)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while((read = getline(&line, &len, config)) != -1)
	{
		char *conf;
		char *delim = ":";
		conf = strtok(line, delim);

		if(strncmp(conf, "number_of_clusters", strlen("number_of_clusters")+1) == 0)
		{
			conf = strtok(NULL, delim);
			parameters->number_of_clusters = atoi(conf);
		}
		else if(strncmp(conf, "number_of_grid_curves", strlen("number_of_grid_curves")+1) == 0)
		{
			conf = strtok(NULL, delim);
			parameters->number_of_grid_curves = atoi(conf);
		}
		else if(strncmp(conf, "number_of_hash_tables", strlen("number_of_hash_tables")+1) == 0)
		{
			conf = strtok(NULL, delim);
			parameters->number_of_hash_tables = atoi(conf);
		}

	}
	free(line);
	line = NULL;
	fclose(config);
	config = NULL;
}

/* reads the input dataset */
Curve* read_conformations(FILE *dataset, unsigned int *number_of_conformations)
{
	char *line = NULL;
	size_t len = 0;

	getline(&line, &len, dataset);
	*number_of_conformations = atoi(line);
	getline(&line, &len, dataset);
	unsigned int conformation_points = atoi(line);

	Curve *conformations = malloc(sizeof(Curve)*(*number_of_conformations));
	for(int i=0; i<*number_of_conformations; i++)
	{
		double **vector = NULL;
		vector = malloc(sizeof(double *)*conformation_points);
		for(int j=0; j<conformation_points; j++)
		{
			vector[j] = malloc(sizeof(double)*dimension);
			getline(&line, &len, dataset);
			char *tok = strtok(line, "\t");
			vector[j][0] = atof(tok);
			tok = strtok(NULL, "\t");
			vector[j][1] = atof(tok);
			tok = strtok(NULL, "\t");
			vector[j][2] = atof(tok);
		}
		char id[10];
		sprintf(id, "%d", i);
		conformations[i] = Curve_create(id, conformation_points, vector, i);
	}

	free(line);
	line = NULL;
	fclose(dataset);
	dataset = NULL;

	return conformations;
}
