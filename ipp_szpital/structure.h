/*Tomasz Kaszlewicz*/
#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int commands_arg[];
extern char *commands[];

typedef struct Hospital Hospital;
typedef struct SameIllness SameIllness;

SameIllness *INIT_ILLNESS(char *description, int n);
void ADD_CASE(struct SameIllness *illness);
bool DELL_CASE(struct SameIllness *illness);

Hospital *INIT_HOSPITAL();
void FREE_HOSPITAL(Hospital* data);

//add information about new disease to the patient
void NEW_DISEASE_ENTER_DESCRIPTION(Hospital *info, char *name, char *description);

//copy disease from one patient to another
void NEW_DISEASE_COPY_DESCRIPTION(Hospital *info, char *name, char *name2);

//change disease description
void CHANGE_DESCRIPTION(Hospital *info, char *name, int n, char *description);

//print patient's nth disease desciption
void PRINT_DESCRIPTION(Hospital *info, char *name, int n);

//delete all diseases form patient. Patient not removed
void DELETE_PATIENT_DATA(Hospital *info, char *name);

//print how many different disease are currently in hospital
void PRINT_DISEASE_CNT(Hospital* data);

#endif