/*Tomasz Kaszlewicz*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structure.h"

int commands_arg[] = { 2,2,3,2,1 };
char *commands[] = {
	"NEW_DISEASE_ENTER_DESCRIPTION",
	"NEW_DISEASE_COPY_DESCRIPTION",
	"CHANGE_DESCRIPTION",
	"PRINT_DESCRIPTION",
	"DELETE_PATIENT_DATA"
};

//structures
typedef struct Patient_list Patient_list;
typedef struct Disease_list Disease_list;
typedef struct SameIllness SameIllness;
typedef struct Hospital Hospital;

struct Patient_list {
	Patient_list *next;
	Disease_list *disease, *last;
	char name[];
};

struct SameIllness {
	int cnt;
	char str[];
};

struct Disease_list {
	Disease_list *next;
	SameIllness *illness;
};

struct Hospital {
	Patient_list *patients, *last;
	int descriptionsCount;
};

void ADD_CASE(struct SameIllness *description) {
	description->cnt++;
}

bool DELL_CASE(struct SameIllness *description) {
	description->cnt--;
	if (!description->cnt) {
		free(description); //noone has this illness anymore
		return true;
	}
	return false;
}

SameIllness *INIT_ILLNESS(char *description, int n) {
	SameIllness *ptr = (SameIllness *)malloc(sizeof(SameIllness) + n * sizeof(char) + 1);
	ptr->cnt = 1;
	strncpy(ptr->str, description, n + 1);
	return ptr;
}

//diseases
Disease_list *INIT_DISEASE() {
	Disease_list *ptr = (Disease_list*)malloc(sizeof(Disease_list));
	ptr->next = NULL;
	ptr->illness = NULL;
	return ptr;
}

Disease_list *NEW_DISEASE(Patient_list *tmp) {
	if (tmp->disease) {
		tmp->last->next = INIT_DISEASE();
		tmp->last = tmp->last->next;
	}
	//first disease
	else {
		tmp->disease = INIT_DISEASE();
		tmp->last = tmp->disease;
	}
	return tmp->last;
}
Disease_list *DISEASE_BY_INDEX(Patient_list *tmp, int x) {
	int i = 1;
	Disease_list *disease = tmp->disease;
	while ((i < x) && (disease != NULL)) {
		disease = disease->next;
		i++;
	}
	return disease;
}

//patients
Patient_list *INIT_PATIENT(char *name) {
	Patient_list* ptr = (Patient_list*)malloc(sizeof(Patient_list) + strlen(name) * sizeof(char) + 1);
	ptr->next = NULL;
	ptr->disease = NULL;
	ptr->last = NULL;
	strncpy(ptr->name, name, strlen(name) + 1);
	return ptr;
}

Patient_list* ADD_PATIENT(Hospital *hospital, Patient_list *tmp){
	if (hospital->last){
		hospital->last->next = tmp;
		hospital->last = tmp;
	}
	//no patients yet
	else{
		hospital->patients = tmp;
		hospital->last = tmp;
	}
}

Patient_list* PATIENT_BY_NAME(Patient_list *tmp, char *name){
	while (tmp != NULL){
		//match found
		if (strcmp(tmp->name, name) == 0)
			return tmp;
		tmp = tmp->next;
	}

	// didn't find the match
	return NULL;
}

//hospital
Hospital *INIT_HOSPITAL() {
	Hospital *ptr = (Hospital *)malloc(sizeof(Hospital));
	ptr->patients = NULL;
	ptr->last = NULL;
	ptr->descriptionsCount = 0;
	return ptr;
}
void FREE_HOSPITAL(Hospital* hospital) {
	Patient_list *current_patient = hospital->patients;

	//delete all patients
	while (current_patient != NULL) {
		Patient_list *tmp_patient = current_patient->next;
		Disease_list *disease = current_patient->disease;

		//delete all diseases from patient
		while (disease != NULL) {
			Disease_list *tmp_disease = disease->next;
			DELL_CASE(disease->illness);
			free(disease);
			disease = tmp_disease;
		}
		free(current_patient);
		current_patient = tmp_patient;
	}

	//delete hospital
	free(hospital);
}

// commands from the task
void PRINT_DISEASE_CNT(Hospital* hospital) {
	fprintf(stderr, "DESCRIPTIONS: %d\n", hospital->descriptionsCount);
}

void NEW_DISEASE_ENTER_DESCRIPTION(Hospital *info, char *name, char *description) {
	Patient_list *patient;
	patient = PATIENT_BY_NAME(info->patients, name);

	//didn't find patient, create new
	if (patient == NULL) {
		patient = INIT_PATIENT(name);
		ADD_PATIENT(info, patient);
	}

	//found or created patient
	Disease_list *disease;
	disease = NEW_DISEASE(patient);
	disease->illness = INIT_ILLNESS(description, strlen(description));
	info->descriptionsCount++;
	printf("OK\n");
}

void NEW_DISEASE_COPY_DESCRIPTION(Hospital *info, char *name, char *name2) {
	Patient_list *patient_zero, *infected;
	Disease_list *disease_to_copy;
	patient_zero = PATIENT_BY_NAME(info->patients, name2);
	if (patient_zero == NULL) {
		printf("IGNORED\n");
		return;
	}
	else
		disease_to_copy = patient_zero->last;

	if (disease_to_copy == NULL) {
		printf("IGNORED\n");
		return;
	}

	infected = PATIENT_BY_NAME(info->patients, name);
	if (infected == NULL) {
		infected = INIT_PATIENT(name);
		ADD_PATIENT(info, infected);
	}

	Disease_list *tmp = NEW_DISEASE(infected);
	ADD_CASE(disease_to_copy->illness);
	tmp->illness = disease_to_copy->illness;
	printf("OK\n");
}

void CHANGE_DESCRIPTION(Hospital *info, char *name, int n, char *description) {
	Patient_list *patient;
	Disease_list *disease;
	patient = PATIENT_BY_NAME(info->patients, name);
	if (patient == NULL) {
		printf("IGNORED\n");
		return;
	}
	else
		disease = DISEASE_BY_INDEX(patient, n);

	if (disease == NULL) {
		printf("IGNORED\n");
		return;
	}

	SameIllness *new_illness = INIT_ILLNESS(description, strlen(description));

	//new illness added
	info->descriptionsCount++;
	bool deleted;
	deleted = DELL_CASE(disease->illness);

	//last reference deleted
	if (deleted)
		info->descriptionsCount--;
	disease->illness = new_illness;
	printf("OK\n");
}

void PRINT_DESCRIPTION(Hospital *info, char *name, int n) {
	Patient_list *patient;
	Disease_list *disease;
	patient = PATIENT_BY_NAME(info->patients, name);
	if (patient == NULL) {
		printf("IGNORED\n");
		return;
	}
	else 
		disease = DISEASE_BY_INDEX(patient, n);

	if (disease == NULL) {
		printf("IGNORED\n");
		return;
	}

	printf("%s\n", disease->illness->str);
}

void DELETE_PATIENT_DATA(Hospital *info, char *name) {
	Patient_list *patient = PATIENT_BY_NAME(info->patients, name);
	Disease_list *disease_current, *next;
	bool deleted;
	if (patient == NULL) {
		printf("IGNORED\n");
		return;
	}
	disease_current = patient->disease;
	while (disease_current != NULL) {
		next = disease_current->next;
		deleted = DELL_CASE(disease_current->illness);
		if (deleted)
			info->descriptionsCount--;
		free(disease_current);
		disease_current = next;
	}

	patient->disease = patient->last = NULL;
	printf("OK\n");
}