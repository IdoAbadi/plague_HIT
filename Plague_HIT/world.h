#ifndef World_H
#define World_H

typedef struct {
    char name[50];
    int healthy_people;
    int sick_people;
    int dead_people;
    int research_investment;
    int development_level;
    int population_density;
    struct Regions* next_region;
} Regions;

typedef struct {
    int disease_detected;
    int disease_cured;
    int healthy_people;
    int sick_people;
    int dead_people;
	int vaccine_progress;
} World;

void DayLoop(Regions* current_region, Disease* disease, World* world, int day_counter);
void print_World(const World* world);
void print_region(const Regions* region);
void SetUpWorld(World* world, const Regions* world_regions);
void ClosingBorders(Regions* region, Disease* disease);
void Curfew(Regions* region, Disease* disease);
void InvestInResearch(Regions* region, World* world);
void ChooseEvent(Regions* current_region, Disease* disease, World* world);
void Vaccine(Regions* region, Disease* disease, World* world);
void anti_vaxxers(Regions* region, Disease* disease);
void vaccine_progress_up(Regions* region, World* world);
void vaccine_progress_down(Regions* region, World* world);



#endif
#pragma once
