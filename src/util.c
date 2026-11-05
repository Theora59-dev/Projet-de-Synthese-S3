#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void ShowMessage(char *msg, int interrupt) {
	fprintf(stderr, "%s\n", msg);
	if (interrupt) {
		fprintf(stderr,
		        "An error has occurred. The execution of the program will "
		        "be interrupted!\n");
		exit(EXIT_FAILURE);
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// INT /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void viewInt(const void *i) {
	if (i == NULL)
		printf("NULL");
	else
		printf("%d", *((int *)i));
}

void freeInt(void *i) { free((int *)i); }

int intSmallerThan(const void *a, const void *b) {
	if (*((int *)a) < *((int *)b))
		return 1;
	else
		return 0;
}

int intGreaterThan(const void *a, const void *b) {
	if (*((int *)a) > *((int *)b))
		return 1;
	else
		return 0;
}

int intEqualTo(const void *a, const void *b) {
	if (*((int *)a) == *((int *)b))
		return 1;
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// UNSIGNED INT ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void viewUInt(const void *i) {
	if (i == NULL)
		printf("NULL");
	else
		printf("%u", *((unsigned int *)i));
}

void freeUInt(void *i) { free((unsigned int *)i); }

int UIntEqualTo(const void *a, const void *b) {
	if (*((unsigned int *)a) == *((unsigned int *)b))
		return 1;
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UNSIGNED LONG ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void viewULong(const void *i) {
	if (i == NULL)
		printf("NULL");
	else
		printf("%lu", *((unsigned long *)i));
}

void freeULong(void *i) { free((unsigned long *)i); }

int ULongEqualTo(const void *a, const void *b) {
	if (*((unsigned long *)a) == *((unsigned long *)b))
		return 1;
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// DOUBLE ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void viewDouble(const void *i) { printf("%f", *((double *)i)); }

void freeDouble(void *i) { free((double *)i); }

int doubleSmallerThan(const void *a, const void *b) {
	if (*((double *)a) < *((double *)b))
		return 1;
	else
		return 0;
}

int doubleGreaterThan(const void *a, const void *b) {
	if (*((double *)a) > *((double *)b))
		return 1;
	else
		return 0;
}
