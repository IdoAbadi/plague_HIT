#ifndef FileFuncs_H
#define FileFuncs_H

void GetRegionInfo(FILE* RegionData, Regions* region);
void CloseRegionDataFile(FILE* RegionData);

Regions* AllocateRegions(int regions_amount);

FILE* OpenRegionData();

#endif
#pragma once
