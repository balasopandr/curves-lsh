#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "command_line_parser.h"
#include "hash_data.h"
#include "curve_metrics.h"

Params command_line_parser(int argc, char **argv)
{
	int c;
	Params parameters = malloc(sizeof(struct par));
	parameters->input_file = NULL;
	parameters->number_of_grid_curves = 2;
	parameters->number_of_hash_tables = 3;
	parameters->grid_density = 0.000001;
	parameters->output_file = NULL;
	parameters->metric_name = NULL;
	parameters->metric_func = NULL;
	parameters->hash_func = &classic_hash;

	while(1)
	{
		static struct option long_options[] =
		{
			/* flags */
			/* non-flag arguments */
			{"input_file", required_argument, 0, 'd'},
			{"output_file", required_argument, 0, 'o'},
			{"config_file", required_argument, 0, 'c'},
			{"function", required_argument, 0},
			{0, 0, 0, 0}
		};

		int option_index = 0;

		c = getopt_long_only(argc, argv, "i:c:o:d:", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;
		switch (c)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				if(strncmp(long_options[option_index].name, "function", strlen("function")) == 0)
				{
					if(strncmp(optarg, "DFT", strlen("DFT")) == 0)
					{
						parameters->metric_name = optarg;
						parameters->metric_func = &Frechet_distance;
					}
					else if(strncmp(optarg, "CRMSD", 5) ==0 
							|| strncmp(optarg, "cRMSD", 5) ==0)
					{
						parameters->metric_name = optarg;
						parameters->metric_func = &CRMSD_curve_dist;
					}
				}
				break;

			case 'i':
				printf ("option -i with value `%s'\n", optarg);
				parameters->input_file = optarg;
				break;
			case 'c':
				printf ("option -c with value `%s'\n", optarg);
				parameters->configuration_file = optarg;
				break;

			case 'o':
				printf ("option -o with value `%s'\n", optarg);
				parameters->output_file = optarg;
				break;

			case 'd':
				printf ("option -d with value `%s'\n", optarg);
				if(strncmp(optarg, "DFT", strlen("DFT")) == 0)
				{
					parameters->metric_name = optarg;
					parameters->metric_func = &Frechet_distance;
				}
				else if(strncmp(optarg, "CRMSD", 5) ==0 
						|| strncmp(optarg, "cRMSD", 5) ==0)
				{
					parameters->metric_name = optarg;
					parameters->metric_func = &CRMSD_curve_dist;
				}
				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;
			default:
				abort ();
		}
	}
	/* Print any remaining command line arguments (not options). */
	if(optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
	}
	return parameters;
}
