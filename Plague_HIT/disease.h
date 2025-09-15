#ifndef DISEASE_H
#define DISEASE_H

typedef struct Disease {
    char name[50];
    int infectiousness;// 1-100
    int severity;// 0-100
    int lethality;// 0-100
} Disease; 

long long Infect(long long infectiousness, long long infected, long long healthy);
long long Kill(long long infected, int leathality, long long healthy, int severity);
int Cure();
int HowContaigous(int tmp);
int HowSevere(int tmp);
int HowLeathal(int tmp);
int ChooseContinent();

void mutate_lethality(Disease* disease);
void mutate_infectiousness(Disease* disease);
void mutate_severity(Disease* disease);
void print_disease(const Disease* disease);
void SetUpDisease(Disease* disease);
void plague_mutation(Disease* disease, int* enable);
void clean_input_buffer();

#endif
#pragma once
