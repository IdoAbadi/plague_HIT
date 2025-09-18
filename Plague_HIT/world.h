#ifndef WORLD_H
#define WORLD_H

#include "disease.h"

#define CURE_REACHED 3000

typedef struct Regions {
    char name[50];
    long long healthy_people; //set to population 
    long long sick_people; // starts at 0
    long long dead_people; // starts at 0
    int research_investment; // 0-100 starts at 0
    int research_resources; // 1-100
    int development_level; // 1-10
    int population_density; // 1-10
    struct Regions* next_region;
} Regions;

typedef struct World {
    int disease_detected;// 0-1
    int disease_cured;// 0-1
    long long healthy_people;// 64 bit number neccesary to represent a population of 8 bilion
    long long sick_people;
    long long dead_people;
    double vaccine_progress;// 0-3000
} World;

void DayLoop(Regions* current_region, Disease* disease, World* world, int day_counter, Regions* world_regions);
void print_World(const World* world);
void print_region(const Regions* region);
void SetUpWorld(World* world, Regions* world_regions);
void ClosingBorders(Regions* region, Disease* disease);//implemented
void Curfew(Regions* region, Disease* disease);//not implemented might cut
void ChooseSimpleEvent(Regions* current_region, Disease* disease, World* world, int* mutation_enable);
void anti_vaxxers(Regions* region, World* world);// implemented
void vaccine_progress_up(World* world);// implemented
void vaccine_progress_down(World* world);// implemented
void public_opinion_escalate(Regions* region, Disease* disease);// implemented
void public_opinion_mitigate(Regions* region, Disease* disease);// implemented
void print_infected_regions(Regions* world_regions);
void UpdateWorld(World* world, Regions* world_regions);
void DiseaseDetected(Regions* region, Disease* disease, World* world, Regions* world_regions);
void StartDisease(Regions* world_regions, char chosen_region[50]);
void SelectDiseaseOrigin(Regions* world_regions, int continent);
void InfectRandomRegion(Regions* world_regions, Regions* exclude_region);
void TriggerInfectOtherRegion(Regions* current_region, Regions* world_regions);
void PrintInitialDetectionLog(double infection_rate, double death_rate, Regions* region);
void SetUpInvestment(World* world, Regions* world_regions);
void WeekLoop(int day_counter, Regions* world_regions, Disease* disease, World* world);
void MonthLog(int day_counter, World* world, Regions* world_regions);
void ApplyCure(Regions* region);
void Cure(Disease* disease, World* world, Regions* current_region);
void PrintMonthlySpreadLog(World* world);

double CalculateRegionResearch(struct Regions* region, struct Disease* disease);

#endif
