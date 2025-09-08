#ifndef FileFuncs_H
#define FileFuncs_H

void GetRegionInfo(FILE* RegionData, Regions* region);
void CloseRegionDataFile(FILE* RegionData);

FILE* OpenRegionData();

#endif
#pragma once
