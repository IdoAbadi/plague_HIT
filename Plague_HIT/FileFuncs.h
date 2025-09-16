#ifndef FILEFUNCS_H
#define FILEFUNCS_H

#include <stdio.h>
#include "world.h"

#define region_amount 21
#define SIZE 256

void GetRegionInfo(FILE* RegionData, Regions* region);
void CloseRegionDataFile(FILE* RegionData);
void SetRegionsParams(Regions* start_region, FILE* Region_Data);
void freeRegions(Regions* world_regions);
void PrintEndLog(Disease* disease, World* world);

Regions* AllocateRegions(int regions_amount);

FILE* OpenRegionData();

FILE* OpenEndLog(char* filename);
#endif
