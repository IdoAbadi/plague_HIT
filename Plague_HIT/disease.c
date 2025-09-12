#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disease.h"
#include "world.h"

#pragma warning(disable:6031) // gets rid of warning to check if scanf_s was succesful

void print_disease(const Disease* disease) {
	printf("Disease: %s\n", disease->name);
	printf("Infectiousness: %d\n", disease->infectiousness);
	printf("Severity: %d\n", disease->severity);
	printf("Lethality: %d\n", disease->lethality);
}

int Infect(int infectiousness, int infected, int healthy) { // add population density to infect calculation
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
    clean_input_buffer();
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
    clean_input_buffer();
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
    clean_input_buffer();
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

int ChooseContinent() {
    int continent_choice = 0;
    printf("choose a continent for the disease to originate from:");
    printf("1) America \n2) Europe \n3) Africa \n4) Asia \n5) Oceania \n6) Islands\n");
    scanf("%d", &continent_choice);
    clean_input_buffer();
    if (0 < continent_choice < 7) {
        return continent_choice;
    }
    else {
        printf("invalid choice, try again\n");
        ChooseContinent();
    }
}

void SelectDiseaseOrigin(Regions* world_regions,int continent) {// not complete
    int region = 0;
    char region_name[50];
    switch (continent)
    {
    case 1:
        printf("Choose a region:\n");
        printf("1) United States & Canada \n2) Latin America \n3) Central America\n");
        scanf("%d", &region);
        if (region = 1) {
            strcpy(region_name,"United States & Canada");
            SetDiseaseOrigin(world_regions, region_name);

        }
    case 2:
        printf("Choose a region:\n");
        printf("1) Western Europe \n2) Eastern Europe \n3) United Kingdom\n");
        scanf("%d", &region);
        if (region = 1) {
            //function that returns a specific region by name
        }
    case 3:
        printf("Choose a region:\n");
        printf("1) North Africa \n2) Sub-Saharan Africa \n");
        scanf("%d", &region);
        if (region = 1) {
            //function that returns a specific region by name
        }
    case 4:
        printf("Choose a region:\n");
        printf("1) Russia \n2) Middle East \n3) South Asia \n4) India \n5) China \n6) Hong Kong and Macau \n7) Central Asia \n8) Southeast Asia\n");
        scanf("%d", &region);
        if (region = 1) {
            //function that returns a specific region by name
        }
    case 5:
        printf("Choose a region:\n");
        printf("1) Australia \n2) New Zealand \n3) Small Pacific nations\n");
        scanf("%d", &region);
        if (region = 1) {
            //function that returns a specific region by name
        }
    case 6:
        printf("Choose a region:\n");
        printf("1) Japan, Korea, and Taiwan \n2) ECaribbean \n");
        scanf("%d", &region);
        if (region = 1) {
            //function that returns a specific region by name
        }

    default:
        printf("invalid choice, try again\n");
        SelectDiseaseOrigin(world_regions, continent);
        break;
    }
}

void SetDiseaseOrigin(const Regions* world_regions, char chosen_region[50]) {
    Regions* curr_region = world_regions;
    while (curr_region) {
        if (curr_region->name == chosen_region) {
            curr_region->sick_people = 100;
            break;
        }
        curr_region = curr_region->next_region;
    }
}

void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}