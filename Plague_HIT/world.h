#ifndef World_H
#define World_H

typedef struct {
    int healthy_people;
    int sick_people;
    int dead_people;
    int research_investment;
    int development_level;
    int population_density;
} Region;

void print_region(const Region* region);

#endif
#pragma once
