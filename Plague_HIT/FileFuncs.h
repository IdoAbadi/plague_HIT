#ifndef FileFuncs_H
#define FileFuncs_H
#include "world.h"
#include "disease.h"

#define region_amount 15
#define SIZE 256

void GetRegionInfo(FILE* RegionData, Regions* region);
void CloseRegionDataFile(FILE* RegionData);
void SetRegionsParams(Regions* start_region, FILE* Region_Data);
void freeRegions(Regions* world_regions);

Regions* AllocateRegions(int regions_amount);

FILE* OpenRegionData();

#endif
#pragma once
