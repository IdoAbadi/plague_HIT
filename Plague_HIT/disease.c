#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disease.h"
#include "design.h"
#include "world.h"

#pragma warning(disable:6031) // gets rid of warning to check if scanf_s was succesful

void PrintDisease(const Disease* disease) {
	printf("Disease: %s\n", disease->name);
	printf("Infectiousness: %d\n", disease->infectiousness);
	printf("Severity: %d\n", disease->severity);
	printf("Lethality: %d\n", disease->lethality);
}

long long Infect(int infectiousness, long long infected, long long healthy) {
    // Base infection rate (0.0 - 1.0)
    double infection_rate = infectiousness / 100.0;
    
    // Calculate expected infections
    double expected = infection_rate * infected;
    
    // Add controlled random variation (±30%)
    double random_factor = 0.7 + ((double)rand() / RAND_MAX) * 0.6;
    long long new_infected = (long long)(expected * random_factor);
    
    // Cap at available healthy population
    if (new_infected > (healthy / 4) && healthy > 100000) {
        new_infected = (healthy / 4);
    }
    else if ( healthy < 1000) {
        new_infected = healthy;
    }
    return new_infected;
}

long long Kill(long long infected, int lethality, long long healthy, int severity) {
    if (infected <= 0) {
        return 0;
    }
    else if (infected < 10) {
        return infected;
    }
    double effective_lethality = lethality + (severity / 2.0);// severity can increse leathality by max of 50
    effective_lethality = effective_lethality / 1.5;
    double base_death_rate = effective_lethality / 100.0;
    double living_population = (double)infected + (double)healthy;
    double population_factor;
    if (living_population > 0) {
        population_factor = (double)infected / living_population;
    }
    else {
        population_factor = 1.0;
    }
    double expected_deaths = infected * base_death_rate * population_factor;
    double variation = (((double)rand() / RAND_MAX) * 0.4 - 0.2) * expected_deaths;// Add controlled random variation (+-20%)
    long long to_die = (long long)(expected_deaths + variation);
    // Cap deaths at 25% of infected per day
    long long max_deaths = infected / 4;
    if (to_die > max_deaths) {
        to_die = max_deaths;
    }
    // Ensure we don't kill more than available
    if (to_die > infected) {
        to_die = infected;
    }
    return to_die;
}

void SetUpDisease(Disease* disease) {
    int tmp = 0;
    PrintColored("What's the name of your disease?\n", YELLOW);
    scanf("%49s", &disease->name);
    disease->infectiousness = HowContaigous(tmp);
    disease->severity = HowSevere(tmp);
    disease->lethality = HowLeathal(tmp);
}

int HowContaigous(int tmp) {
    PrintColored("How contagious is the disease? (1-100): \n", YELLOW);
    scanf_s("%d", &tmp);
    clean_input_buffer();
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value.\n");
        return HowContaigous(tmp);
    }
}

int HowSevere(int tmp) {
    PrintColored("How severe is it? (1-100): \n", YELLOW);
    scanf_s("%d", &tmp);
    clean_input_buffer();
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value.\n");
        return HowSevere(tmp);
    }
}

int HowLeathal(int tmp) {
    PrintColored("How deadly is it? (1-100): \n", YELLOW);
    scanf_s("%d", &tmp);
    clean_input_buffer();
    if (tmp > 0 && tmp < 101) {
        return tmp;
    }
    else {
        printf("invalid value.\n");
        return HowLeathal(tmp);
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
    int change = (rand() % 14) - 5;  // From -5 to 8
    if (change == 0) {//mutation always occurs
        change = 1;
    }
    disease->infectiousness += change;
    if (disease->infectiousness < 1) {
        disease->infectiousness = 1;
    }
}

void mutate_severity(Disease* disease) {
    if (disease->severity >= 100) {
        return; // No mutations if already at max
	}
    int change = (rand() % 14) - 5;  // From -5 to 8
    if (change == 0) {//mutation always occurs
        change = 1;
    }
    disease->severity += change;
    if (disease->severity < 0) {
        disease->severity = 0;
    }
}

void mutate_lethality(Disease* disease) {
    if (disease->lethality >= 100) {
        return; // No mutations if already at max
    }
    int change = (rand() % 14) - 5;  // From -5 to 8
    if (change == 0) {//mutation always occurs
        change = 1;
    }
    disease->lethality += change;
    if (disease->lethality < 0) {
        disease->lethality = 0;
    }
}

void plague_mutation(Disease* disease, int* enable) {
    if (enable != NULL && *enable == 1) {
        int param = rand() % 3; // Randomly choose which parameter to mutate
        if (param == 0) {
            mutate_infectiousness(disease);
            *enable = 0;
        }
        else if (param == 1) {
            mutate_severity(disease);
            *enable = 0;
        }
        else {
            mutate_lethality(disease);
            *enable = 0;
        }
    }
}

int ChooseContinent() {
    int continent_choice = 0;
    PrintColored("\nchoose a continent for the disease to originate from:\n", YELLOW);
    PrintColored("1) America \n2) Europe \n3) Africa \n4) Asia \n5) Oceania \n6) Islands\n", BLUE);
    scanf("%d", &continent_choice);
    clean_input_buffer();
    if (0 < continent_choice < 7) {
        return continent_choice;
    }
    else {
        printf("invalid choice, try again\n");
        return ChooseContinent();
    }
}

void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}