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
} World;

void DayLoop(Regions* current_region);
void print_World(const World* world);
void print_region(const Regions* region);
void SetUpWorld(World* world, const Regions* world_regions);
void closing_borders(Regions* region, Disease* disease);
void curfew(Regions* region);
void invest_in_research(Regions* region, World* world);

#endif
#pragma once
