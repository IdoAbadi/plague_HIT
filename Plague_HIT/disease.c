#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "disease.h"

#pragma warning(disable:6031) // gets rid of warning to check if scanf_s was succesful

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
    scanf_s("%d", &tmp);
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value.\n");
        HowContaigous(tmp);
    }
}

int HowSevere(int tmp) {
    printf("How severe is it? (1-100): \n");
    scanf_s("%d", &tmp);
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value.\n");
        HowSevere(tmp);
    }
}

int HowLeathal(int tmp) {
    printf("How deadly is it? (1-100): \n");
    scanf_s("%d", &tmp);
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value.\n");
        HowLeathal(tmp);
    }
}

/*void plague_mutation(Disease* disease) {//incorrect need to fix
    if (disease->infectiousness >= 100) {
        return; // No mutations if already max infectiousness
    }

    int param = rand() % 3;        
	int change = (rand() % 11) - 5;  // From -5 to 5

    if (param == 0) {
        disease->infectiousness += change;
        if (disease->infectiousness < 1)   disease->infectiousness = 1;
        if (disease->infectiousness > 100) disease->infectiousness = 100;
    }
    else if (param == 1) {
        disease->severity += change;
        if (disease->severity < 1)   disease->severity = 1;
        if (disease->severity > 100) disease->severity = 100;
    }
    else { // param == 2
        disease->lethality += change;
        if (disease->lethality < 1)   disease->lethality = 1;
        if (disease->lethality > 100) disease->lethality = 100;
    }
}*/

void mutate_infectiousness(Disease* disease) {
    if (disease->infectiousness >= 100) {
        return; // No mutations if already at max
    }
    int change = (rand() % 11) - 5;  // From -5 to 5
    disease->infectiousness += change;
    if (disease->infectiousness < 1) {
        disease->infectiousness = 1;
    }
}

void mutate_severity(Disease* disease) {
    if (disease->severity >= 100) {
        return; // No mutations if already at max
	}
    int change = (rand() % 11) - 5;  // From -5 to 5
    disease->severity += change;
    if (disease->severity < 0) {
        disease->severity = 0;
    }
}

void mutate_lethality(Disease* disease) {
    if (disease->lethality >= 100) {
        return; // No mutations if already at max
    }
    int change = (rand() % 11) - 5;  // From -5 to 5
    disease->lethality += change;
    if (disease->lethality < 0) {
        disease->lethality = 0;
    }
}

void plague_mutation(Disease* disease) {
    int param = rand() % 3; // Randomly choose which parameter to mutate
    if (param == 0) {
        mutate_infectiousness(disease);
    }
    else if (param == 1) {
        mutate_severity(disease);
    }
    else { 
        mutate_lethality(disease);
    }
}


void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}