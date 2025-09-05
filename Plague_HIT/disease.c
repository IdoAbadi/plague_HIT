#include <stdio.h>
#include "disease.h"

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6031) // gets rid of warning to check if scanf was succesful

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

void SetUpDisease(Disease* disease) {
    int tmp = 0;
    printf("What's the name of your disease?\n");
    scanf("%49s", &disease->name);
    disease->infectiousness = HowContaigous(tmp);
    disease->severity = HowSevere(tmp);
    disease->lethality = HowLeathal(tmp);
}

int HowContaigous(int tmp) {
    printf("How contagious is the disease? (1-100): \n");
    scanf("%d", &tmp);
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value\n");
        HowContaigous(tmp);
    }
}

int HowSevere(int tmp) {
    printf("How severe is it? (1-100): \n");
    scanf("%d", &tmp);
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value\n");
        HowSevere(tmp);
    }
}

int HowLeathal(int tmp) {
    printf("How deadly is it? (1-100): \n");
    scanf("%d", &tmp);
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value\n");
        HowLeathal(tmp);
    }
}

void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}