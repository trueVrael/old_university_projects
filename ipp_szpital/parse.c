/*Tomasz Kaszlewicz*/
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMANDS 5

char *p;

void get_argument(char *arg, int n, char *str) {
	while (p < str + n) {
		if (*p == ' ') {
			*p = '\0';
			break;
		}
		else
			p++;
	}
}

int parse(Hospital *info, char *str, int n) {
	int option = -1;
	int i = 0;
	int argc;
	char *last = str + n - 1;
	if (*last == '\n'){
		*last = '\0';
	}
	//check which command is it
	for (i = 0; i < COMMANDS; i++) {
		if (strncmp(commands[i], str, strlen(commands[i])) == 0) {
			option = i;
			break;
		}
	}

	//didn't find correct command
	if (option == -1)
		return 1;

	//number of arguments
	argc = commands_arg[option];
	char *tmp1, *tmp2, *tmp3;
	char *argument1 = (char*)malloc(argc * sizeof(char*));
	char *argument2 = (char*)malloc(argc * sizeof(char*));
	char *argument3 = (char*)malloc(argc * sizeof(char*));

	tmp1 = argument1;
	tmp2 = argument2;
	tmp3 = argument3;

	//starting position
	argument1 = str + strlen(commands[option]) + 1;
	p = argument1;
	get_argument(argument1, n, str);
	if (argc > 1) argument2 = ++p;
	if (argc > 2) {
		get_argument(argument3, n, str);
		argument3 = ++p;
	}

	//choose function
	if (option == 0) {
		NEW_DISEASE_ENTER_DESCRIPTION(info, argument1, argument2);
	}
	else if (option == 1) {
		NEW_DISEASE_COPY_DESCRIPTION(info, argument1, argument2);
	}
	else if (option == 2) {
		int index = strtoul(argument2, NULL, 10);
		CHANGE_DESCRIPTION(info, argument1, index, argument3);
	}
	else if (option == 3) {
		int index = strtoul(argument2, NULL, 10);
		PRINT_DESCRIPTION(info, argument1, index);
	}
	else if (option == 4) {
		DELETE_PATIENT_DATA(info, argument1);
	}
	else printf("error\n");
	
	free(tmp1);
	free(tmp2);
	free(tmp3);
	
	return 0;
}