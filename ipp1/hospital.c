#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "structure.h"
#include "parse.h"

#define MAX_LINE_LEN 100000

bool verbose = false;

int main(int argc, char **argv)
{
	// Check for "-v" flag - exit with error if there are others
	if (argc == 2 && strcmp(argv[1], "-v") == 0)
		verbose = true;
	else if (argc > 1)
	{
		printf("ERROR");
		return 1;
	}

	HospitalData *data = initHospital();

	char buf[MAX_LINE_LEN];
	while (fgets(buf, MAX_LINE_LEN, stdin))
	{
		parse(buf, strlen(buf), data); // ignore trailing \n from fgets

		if (verbose)
			printDescriptionCount(data);
	}

	freeHospital(data);

	return 0;
}
