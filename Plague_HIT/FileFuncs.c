#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileFuncs.h"
#include "world.h"
#include "disease.h"

FILE* OpenRegionData() {
	FILE* RegionData = fopen("RegionsData.txt", "r");
	if (!RegionData) {
		printf("file didnt open, you may panic now");
		return NULL;
	}
	//printf("opened succesfuly");
	return RegionData;
}

void CloseRegionDataFile(FILE* RegionData) {
	fclose(RegionData);
}

Regions* AllocateRegions(int regions_amount) {
	int regions_to_allocate = regions_amount - 1;
	Regions* start_region = malloc(sizeof(Regions));
	if (!start_region) {
		return NULL;
	}
	Regions* curr_region = start_region;
	for (int i = 0; i < regions_to_allocate; i++) {
		Regions* new_region = malloc(sizeof(Regions));
		if (!new_region) {
			printf("failed allocation on region number %d", i);
			return start_region;
		}
		curr_region->next_region = new_region;
		curr_region = new_region;
	}
	curr_region->next_region = NULL;//sets list end to NULL
	return start_region;
}

void GetRegionInfo(FILE* RegionData, Regions* region) {
	char buffer[SIZE];
	if (fgets(buffer, SIZE, RegionData)) {// read name
		strncpy_s(region->name, sizeof(region->name), buffer, _TRUNCATE);
	}
	region->name[strcspn(region->name, "\r\n")] = 0; // strips new line character
	if (fgets(buffer, SIZE, RegionData)) {// read healthy people
		region->healthy_people = atoi(buffer); // atoi = ascii to interger
	}
	if (fgets(buffer, SIZE, RegionData)) {// read sick people
		region->sick_people = atoi(buffer);
	}
	if (fgets(buffer, SIZE, RegionData)) {// read dead people
		region->dead_people = atoi(buffer);
	}
	if (fgets(buffer, SIZE, RegionData)) {// read development level
		region->research_investment = atoi(buffer);
	}
	if (fgets(buffer, SIZE, RegionData)) {// read development level
		region->research_resources = atoi(buffer);
	}
	if (fgets(buffer, SIZE, RegionData)) {// read development level
		region->development_level = atoi(buffer);
	}
	if (fgets(buffer, SIZE, RegionData)) {// read population density
		region->population_density = atoi(buffer);
	}
}

void SetRegionsParams(Regions* start_region, FILE* Region_Data) {
	Regions* curr_region = start_region;
	while (curr_region)
	{
		GetRegionInfo(Region_Data, curr_region);
		curr_region = curr_region->next_region;
	}
}

void freeRegions(Regions* world_regions) {
	Regions* temp;
	while (world_regions != NULL) {
		temp = world_regions;  
		world_regions = world_regions->next_region;
		free(temp); 
	}
}