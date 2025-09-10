#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileFuncs.h"
#include "world.h"

#define SIZE 256

FILE* OpenRegionData() {
	FILE* RegionData = fopen("RegionsData", 'r');
	if (!RegionData) {
		printf("file didnt open, you may pnic now");
		return;
	}
	return RegionData;
}

void CloseRegionDataFile(FILE* RegionData) {
	fclose(RegionData);
}

Regions* AllocateRegions(int regions_amount) {
	int regions_to_allocate = regions_amount - 1;
	Regions* start_region = malloc(sizeof(Regions));
	Regions* curr_region = start_region;
	for (int i = 0; i < regions_to_allocate; i++) {
		Regions* new_region = malloc(sizeof(Regions));
		if (!new_region) {
			break;
		}
		curr_region->next_region = new_region;
		curr_region = new_region;
	}
	return start_region;
}

void GetRegionInfo(FILE* RegionData, Regions* region) {
	char buffer[SIZE];
	if (fgets(buffer, SIZE, RegionData)) {// read name
		strncpy(region->name, buffer, sizeof(region->name));
	}
	region->name[strcspn(region->name, "\r\n")] = 0;
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
		region->development_level = atoi(buffer);
	}
	if (fgets(buffer, SIZE, RegionData)) {// read population density
		region->population_density = atoi(buffer);
	}
}