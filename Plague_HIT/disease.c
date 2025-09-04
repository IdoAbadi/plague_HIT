#include <stdio.h>
#include "disease.h"

void print_disease(const Disease* disease) {
	printf("Disease: %s\n", disease->name);
	printf("Infectiousness: %d\n", disease->infectiousness);
	printf("Severity: %d\n", disease->severity);
	printf("Lethality: %d\n", disease->lethality);
}

void get_disease_parameters(Disease* disease) {
	printf("Enter disease name: ");
	scanf("%49s", disease->name);
	printf("Enter infectiousness (0-100): ");
	scanf("%d", &disease->infectiousness);
	printf("Enter severity (0-100): ");
	scanf("%d", &disease->severity);
	printf("Enter lethality (0-100): ");
	scanf("%d", &disease->lethality);
}
