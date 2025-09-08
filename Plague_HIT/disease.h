#ifndef Disease_H
#define Disease_H

typedef struct {
    char name[50];
    int infectiousness;// 1-100
    int severity;// 0-100
    int lethality;// 0-100
} Disease; 

int Infect(int infectiousness, int infected, int healthy);
int Kill(int infected, int leathality);
int Cure();
int HowContaigous(int tmp);
int HowSevere(int tmp);
int HowLeathal(int tmp);

void mutate_lethality(Disease* disease);
void mutate_infectiousness(Disease* disease);
void mutate_severity(Disease* disease);
void print_disease(const Disease* disease);
void SetUpDisease(Disease* disease);
void plague_mutation(Disease* disease);
void clean_input_buffer();


#endif
#pragma once
