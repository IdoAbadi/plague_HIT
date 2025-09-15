#ifndef SORT_H
#define SORT_H
#include "world.h"

typedef struct AffectedRegions {
	char name[50];
	int affected_percentage;
}AffectedRegions;

int RegionAffectedPopulation(Regions* current_region);
AffectedRegions* SortAffectedRegions(Regions* world_regions);
void SaveToFile(AffectedRegions* affected_array);
void AppendToFile(AffectedRegions* affected_array);


#endif 
#pragma once
