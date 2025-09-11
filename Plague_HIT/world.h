#ifndef World_H
#define World_H
#include "disease.h"

typedef struct {
    char name[50];
    int healthy_people; //set to population 
    int sick_people; // starts at 0
    int dead_people; // starts at 0
    int research_investment; // 0-100 starts at 0
    int research_resources; // 1-100
    int development_level; // 1-10
    int population_density; // 1-10
    struct Regions* next_region;
} Regions;

typedef struct {
    int disease_detected;// 0-1
    int disease_cured;// 0-1
    int healthy_people;
    int sick_people;
    int dead_people;
	int vaccine_progress;// 0-1000
} World;

void DayLoop(Regions* current_region, Disease* disease, World* world, int day_counter);
void print_World(const World* world);
void print_region(const Regions* region);
void SetUpWorld(World* world, const Regions* world_regions);
void ClosingBorders(Regions* region, Disease* disease);
void Curfew(Regions* region, Disease* disease);
void InvestInResearch(Regions* region, World* world);
void ChooseEvent(Regions* current_region, Disease* disease, World* world);
void anti_vaxxers(Regions* region, Disease* disease);
void vaccine_progress_up(World* world);
void vaccine_progress_down(World* world);
void IsCureReached(World* world);
void public_opinion_escalate(Regions* region, Disease* disease);
void public_opinion_mitigate(Regions* region, Disease* disease);
void public_opinion(Regions* region, Disease* disease);

#endif
#pragma once
