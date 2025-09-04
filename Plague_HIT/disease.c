#include <stdio.h>
#include "disease.h"

void print_disease(const Disease* disease) {
	printf("Disease: %s\n", disease->name);
	printf("Infectiousness: %d\n", disease->infectiousness);
	printf("Severity: %d\n", disease->severity);
	printf("Lethality: %d\n", disease->lethality);
}

int Infect(int infectiousness, int infected, int healthy) {
    double expected = (infectiousness / 100.0) * infected;// scale infections by infectiousness
    int variation = rand() % (infected + 1); // random 0..infected
    int new_infected = (int)(expected * 0.7 + variation * 0.3);

    // cap at healthy population
    if (new_infected > healthy) {
        new_infected = healthy;
    }
    return new_infected;
}

int Kill(int infected, int leathality) {
    double expected = (leathality / 100.0) * infected;// scale deaths by leathality
    int variation = rand() % (infected + 1); // random 0..infected
    int to_die = (int)(expected * 0.7 + variation * 0.3);

    if (infected > 100) {
        if (to_die > infected / 3) {
            return infected / 3;
        }
        else {
            return to_die;
        }
    }
    return infected;
}

int Cure() {

}

void infectiosness_details(Disease* disease) {
    printf("How contagious is the disease? (1-100): \n");
    if (scanf("%d", &disease->infectiousness) != 1 || disease->infectiousness < 1 || disease->infectiousness > 100) {
        printf("Invalid input. Please enter a value between 1 and 100.\n");
        disease->infectiousness = 1;
    }
    printf("How strong is it? (1-100): \n");
    if (scanf("%d", &disease->severity) != 1 || disease->severity < 1 || disease->severity > 100) {
        printf("Invalid input. Please enter a value between 1 and 100.\n");
        disease->severity = 1;
    }
    printf("How deadly is it? (1-100): \n");
    if (scanf("%d", &disease->lethality) != 1 || disease->lethality < 1 || disease->lethality > 100) {
        printf("Invalid input. Please enter a value between 1 and 100.\n");
        disease->lethality = 1;
    }
}