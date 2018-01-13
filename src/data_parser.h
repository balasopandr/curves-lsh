#ifndef __DATA_PARSER__
#define __DATA_PARSER__
#include "curve.h"
#include "command_line_parser.h"

Curve parse_curve_dataset_line(char *line, size_t len, ssize_t read, int *max_curve_points, int curve_idx);
double ** parse_vector(char *str_vector, unsigned int point_count, unsigned int dimension);
Curve * read_curves(FILE *dataset, unsigned int number_of_curves, int *max_curve_points);
void parse_config_file(FILE *config, Params parameters);
unsigned int read_curve_count(FILE *dataset);
Curve* read_conformations(FILE *dataset, unsigned int *number_of_conformations);
#endif
