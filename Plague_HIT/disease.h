#ifndef Disease_H
#define Disease_H

typedef struct {
    char name[50];
    int infectiousness;
    int severity;
    int lethality;
} Disease; 

void print_disease(const Disease* disease);

int Infect(int infectiousness, int infected, int healthy);
int Kill(int infected, int leathality);
int Cure();
void infectiosness_details(Disease* disease);
#endif
#pragma once
