#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct StringRefCount StringRefCount;

StringRefCount* initStringRefCount(const char* src, size_t n);
void add_ref(struct StringRefCount* ref);
bool dec_ref(struct StringRefCount* ref);

//---------------------------------------------------------

typedef struct HospitalData HospitalData;

typedef enum Command
{
	INVALID_COMMAND = -1,
	NEW_DISEASE_ENTER_DESCRIPTION = 0,
	NEW_DISEASE_COPY_DESCRIPTION,
	CHANGE_DESCRIPTION,
	PRINT_DESCRIPTION,
	DELETE_PATIENT_DATA,
	COMMAND_COUNT
} Command;

extern const size_t commandArgc[]; // = { 2, 2, 3, 2, 1 };

extern const char* commands[];
//"NEW_DISEASE_ENTER_DESCRIPTION",
//"NEW_DISEASE_COPY_DESCRIPTION",
//"CHANGE_DESCRIPTION",
//"PRINT_DESCRIPTION",
//"DELETE_PATIENT_DATA"

HospitalData *initHospital();
void freeHospital(HospitalData* data);

void printDescriptionCount(HospitalData* data);

// NEW_DISEASE_ENTER_DESCRIPTION name disease_description
// Dodaje informacjê o chorobie pacjenta o nazwisku name. Dalsza czêœæ linii
// stanowi opis przebytej choroby, w tym przypadku jest to "disease description".
void newDiseaseEnter(HospitalData* data, const char* name, const char* desc);

// NEW_DISEASE_COPY_DESCRIPTION name1 name2
// Dodaje informacjê o chorobie pacjenta o nazwisku name1. Opis nowej
// choroby jest taki samy jak aktualny opis ostatnio zarejestrowanej
// choroby pacjenta o nazwisku name2.
void newDiseaseCopy(HospitalData* data, const char* name1, const char* name2);

// CHANGE_DESCRIPTION name n disease_description
// Aktualizuje opis n - tej choroby pacjenta o nazwisku name.Dalsza czêœæ
// linii stanowi nowy opis choroby, w tym przypadku "disease description".
void changeDescription(HospitalData* data, const char* name, int n, const char* desc);

// PRINT_DESCRIPTION name n
// Wypisuje na standardowe wyjœcie opis n - tej choroby pacjenta o nazwisku name.
void printDescription(HospitalData* data, const char* name, int n);

// DELETE_PATIENT_DATA name
// Usuwa historiê chorób pacjenta o nazwisku name. Pacjent z pust¹ list¹ chorób pozostaje w strukturze.
void deletePatientData(HospitalData* data, const char* name);

#endif // _STRUCTURE_H_