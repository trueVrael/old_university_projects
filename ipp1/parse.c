#include "parse.h"

#include <stdio.h>
#include <string.h>

int parse(char* str, int n, struct HospitalData* data)
{
	Command comm = INVALID_COMMAND;
	int i = 0;

	// TODO: try reversing search order for faster exit-early scenarios
	for (i = 0; i < COMMAND_COUNT; ++i)
	{
		if (strncmp(commands[i], str, strlen(commands[i])) == 0)
		{
			comm = i;
			break;
		}
	}

	if (comm == INVALID_COMMAND)
		return 1;

	// Currently supports commands having atleast 1 argument
	const int argc = commandArgc[comm];
	char **argv = (char**)malloc(argc * sizeof(char*));
	argv[0] = str + strlen(commands[comm]) + 1;
	// Prepare arguments from str
	char *p = argv[0];
	i = 1;

	while (p < str + n)
	{
		if (*p != ' ')
			++p;
		else
		{
			*p = '\0';
			argv[i++] = ++p;

			if (i >= argc)
				break;
		}
	}
	// Strip trailing newline
	char* const lastChar = str + n - 1;
	if (*lastChar == '\n')
		*lastChar = '\0';

	switch (comm)
	{
	case NEW_DISEASE_ENTER_DESCRIPTION: // name disease_description
	{
		newDiseaseEnter(data, argv[0], argv[1]);
		break;
	}
	case NEW_DISEASE_COPY_DESCRIPTION: // name1 name2
	{
		newDiseaseCopy(data, argv[0], argv[1]);
		break;
	}
	case CHANGE_DESCRIPTION: // name n disease_description
	{
		const size_t n = strtoul(argv[1], NULL, 10);

		changeDescription(data, argv[0], n, argv[2]);
		break;
	}
	case PRINT_DESCRIPTION: // name n
	{
		const size_t n = strtoul(argv[1], NULL, 10);

		printDescription(data, argv[0], n);
		break;
	}
	case DELETE_PATIENT_DATA: // name
	{
		deletePatientData(data, argv[0]);
		break;
	}

	case INVALID_COMMAND:
	{
		printf("Z³a komenda");
		break;
	}
	}

	free(argv);

	return 0;
}
