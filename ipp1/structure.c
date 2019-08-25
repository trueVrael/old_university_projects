#include "structure.h"

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

const size_t commandArgc[COMMAND_COUNT] = { 2, 2, 3, 2, 1 };

const char* commands[COMMAND_COUNT] = {
"NEW_DISEASE_ENTER_DESCRIPTION",
"NEW_DISEASE_COPY_DESCRIPTION",
"CHANGE_DESCRIPTION",
"PRINT_DESCRIPTION",
"DELETE_PATIENT_DATA"
};



//---------------------------------------------------------
// Implementation-specific data structures

typedef struct StringRefCount StringRefCount;

StringRefCount* initStringRefCount(const char* src, size_t n);
void add_ref(struct StringRefCount* ref);
bool dec_ref(struct StringRefCount* ref);

typedef struct PatientNode PatientNode;

PatientNode* initPatient(const char *name);
PatientNode* addPatient(HospitalData* data, PatientNode* patient);
PatientNode* findPatient(PatientNode* start, const char *name);

typedef struct DiseaseNode DiseaseNode;

DiseaseNode* initDisease();
DiseaseNode* newDisease(PatientNode* patient);
DiseaseNode* getDiseaseByIndex(PatientNode* patient, size_t index);

//---------------------------------------------------------

struct HospitalData
{
	PatientNode* patientList;
	PatientNode* lastPatient;
	size_t descriptionsCount;
};


struct StringRefCount
{
	int count;
	char str[]; // flexible
};

StringRefCount* initStringRefCount(const char* src, size_t n)
{
	StringRefCount *p = (StringRefCount *)malloc(sizeof(StringRefCount) + n * sizeof(char) + 1); // n is strlen
	p->count = 0;
	strncpy(p->str, src, n + 1);
	add_ref(p);

	return p;
}

void add_ref(struct StringRefCount* ref)
{
	++ref->count;
}

bool dec_ref(struct StringRefCount* ref)
{
	if (--ref->count == 0)
	{
		free(ref);
		return true;
	}

	return false;
}


struct PatientNode
{
	PatientNode *next;
	DiseaseNode *diseases;
	DiseaseNode *lastDisease;
	char name[];
};

PatientNode* initPatient(const char *name)
{
	PatientNode* p = (PatientNode*)malloc(sizeof(PatientNode) + strlen(name) * sizeof(char) + 1);
	p->next = NULL;
	p->diseases = NULL;
	p->lastDisease = NULL;
	//p->name = (char*)(p + sizeof(PatientNode) - sizeof(char*)); // point into own struct flexible memory field
	strncpy(p->name, name, strlen(name) + 1);

	return p;
}

PatientNode* addPatient(HospitalData* data, PatientNode* patient)
{
	assert(patient);

	if (data->lastPatient)
	{
		data->lastPatient->next = patient;
		data->lastPatient = patient;
	}
	else
	{
		data->patientList = patient;
		data->lastPatient = patient;
	}
}

PatientNode* findPatient(PatientNode* start, const char *name)
{
	while (start)
	{
		if (strcmp(start->name, name) == 0)
			return start;
		
		start = start->next;
	}

	return NULL;
}

struct DiseaseNode
{
	DiseaseNode *next;
	StringRefCount *diseaseDesc;
};

DiseaseNode* initDisease()
{
	DiseaseNode* p = (DiseaseNode*)malloc(sizeof(DiseaseNode));
	p->next = NULL;
	p->diseaseDesc = NULL;

	return p;
}

DiseaseNode* newDisease(PatientNode* patient)
{
	assert(patient);

	if (patient->diseases)
	{
		patient->lastDisease->next = initDisease();
		patient->lastDisease = patient->lastDisease->next;
	}
	else
	{
		patient->diseases = initDisease();
		patient->lastDisease = patient->diseases;
	}

	return patient->lastDisease;
}

DiseaseNode* getDiseaseByIndex(PatientNode* patient, size_t index)
{
	// naive search from the first one
	assert(patient);

	size_t i = 1;
	DiseaseNode* disease = patient->diseases;

	while (disease && i < index)
	{
		disease = disease->next;
		++i;
	}

	return disease;
}

HospitalData *initHospital()
{
	HospitalData* p = (HospitalData *)malloc(sizeof(HospitalData));
	p->patientList = NULL;
	p->lastPatient = NULL;
	p->descriptionsCount = 0;

	return p;
}

void freeHospital(HospitalData* data)
{
	PatientNode* patient = data->patientList;
	while (patient)
	{
		PatientNode* next = patient->next;

		DiseaseNode *disease = patient->diseases;
		while (disease)
		{
			DiseaseNode* next = disease->next;

			dec_ref(disease->diseaseDesc);
			free(disease);

			disease = next;
		}

		free(patient);

		patient = next;
	}

	free(data);
}

//---------------------------------------------------------
// Public interface
void printDescriptionCount(HospitalData* data)
{
	fprintf(stderr, "DESCRIPTIONS: %u\n", data->descriptionsCount);
}

void newDiseaseEnter(HospitalData* data, const char* name, const char* desc)
{
	PatientNode* patient = findPatient(data->patientList, name);
	if (patient == NULL)
	{
		patient = initPatient(name);
		addPatient(data, patient);
	}

	DiseaseNode* newDis = newDisease(patient);

	newDis->diseaseDesc = initStringRefCount(desc, strlen(desc));
	++data->descriptionsCount;

	printf("OK\n");
}

void newDiseaseCopy(HospitalData* data, const char* name1, const char* name2)
{
	PatientNode* srcPatient = findPatient(data->patientList, name2);
	DiseaseNode* srcDisease = srcPatient ? srcPatient->lastDisease : NULL;

	if (srcPatient == NULL || srcDisease == NULL)
	{
		printf("IGNORED\n");
		return;
	}

	PatientNode* patient = findPatient(data->patientList, name1);
	if (patient == NULL)
	{
		patient = initPatient(name1);
		addPatient(data, patient);
	}

	DiseaseNode* newDis = newDisease(patient);
	newDis->diseaseDesc = srcDisease->diseaseDesc;
	add_ref(srcDisease->diseaseDesc);

	printf("OK\n");
}

void changeDescription(HospitalData* data, const char* name, int n, const char* desc)
{
	PatientNode* patient = findPatient(data->patientList, name);
	DiseaseNode* disease = patient ? getDiseaseByIndex(patient, n) : NULL;

	if (patient == NULL || disease == NULL)
	{
		printf("IGNORED\n");
		return;
	}

	StringRefCount* newDesc = initStringRefCount(desc, strlen(desc));
	++data->descriptionsCount;

	const bool memFreed = dec_ref(disease->diseaseDesc);
	disease->diseaseDesc = newDesc;

	if (memFreed)
		--data->descriptionsCount;

	printf("OK\n");
}

void printDescription(HospitalData* data, const char* name, int n)
{
	PatientNode* patient = findPatient(data->patientList, name);
	DiseaseNode* disease = patient ? getDiseaseByIndex(patient, n) : NULL;

	if (patient == NULL || disease == NULL)
	{
		printf("IGNORED\n");
		return;
	}

	printf("%s\n", disease->diseaseDesc->str);
}

void deletePatientData(HospitalData* data, const char* name)
{
	PatientNode* patient = findPatient(data->patientList, name);

	if (patient == NULL)
	{
		printf("IGNORED\n");
		return;
	}

	DiseaseNode* disease = patient->diseases;
	DiseaseNode* next;
	while (disease)
	{
		next = disease->next;

		const bool memFreed = dec_ref(disease->diseaseDesc);
		if (memFreed)
			--data->descriptionsCount;
		free(disease);

		disease = next;
	}

	patient->diseases = NULL;
	patient->lastDisease = NULL;

	printf("OK\n");
}