/*Tomasz Kaszlewicz*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parse.h"
#include "structure.h"

#define MAX_LEN 100000
bool vflag = false;
char buffer[MAX_LEN];

int main(int argc, char **argv) {

	//Check flags
	if ((argc == 2) && (strcmp(argv[1], "-v") == 0))
		vflag = true;
	else if (argc != 1) {
		printf("ERROR");
		return 1;
	}

	Hospital *info = INIT_HOSPITAL();

	while (fgets(buffer, MAX_LEN, stdin)) {

		//parse one line of input
		parse(info, buffer, strlen(buffer));
	
		if (vflag)
			PRINT_DISEASE_CNT(info);
	}
	
	//free memory
	FREE_HOSPITAL(info);

	return 0;
}