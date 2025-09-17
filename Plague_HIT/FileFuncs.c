#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileFuncs.h"
#include "world.h"
#include "disease.h"
#include "sort.h"

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

void PrintEndLog(Disease* disease, World* world, int day_counter, Regions* world_regions) { 
	FILE* EndLog = OpenEndLog(disease->name); //function that returns a file pointer
	if (EndLog == NULL) {
		printf("Error opening end log file.\n");
		return;
	}
	fputs("-----End of Simulation Log-----\n", EndLog);
	if (world->disease_detected == 0 && world->healthy_people <= 0) {
		printf("%s hasn't been detected.\n", disease->name);
		fprintf(EndLog,"%s hasn't been detected.\n", disease->name);
		printf("%s has rampaged around the world for %d days.\n", disease->name, day_counter);
		fprintf(EndLog,"%s has rampaged around the world for %d days.\n", disease->name, day_counter);
		printf("The entire world population has died from the disease.\n");
		fprintf(EndLog,"The entire world population has died from the disease.\n");
		printf("This log was also saved to the file %s.txt", disease->name);
	}
	else if (world->disease_detected == 0 && world->healthy_people >= 0) {
		printf("%s hasn't been detected.\n", disease->name);
		fprintf(EndLog, "%s hasn't been detected.\n", disease->name);
		printf("%s has rampaged around the world for %d days.\n", disease->name, day_counter);
		fprintf(EndLog, "%s has rampaged around the world for %d days.\n", disease->name, day_counter);
		printf("Eventually The disease killed itself\n");
		fprintf(EndLog,"Eventually The disease killed itself\n");
		printf("For a full log with a list of all regions sorted from most affected to least affected check the file called %s.txt", disease->name);
		AffectedRegions* affected_Region_List = SortAffectedRegions(world_regions);
		SaveAffectedRegionListToFile(affected_Region_List, EndLog);
	}
	else if (world->disease_detected == 1 && world->disease_cured == 0) {
		printf("%s hasn been detected.\n", disease->name);
		fprintf(EndLog, "%s hasn been detected.\n", disease->name);
		printf("%s has rampaged around the world for %d days but wasn't stopped in time, the entire world was eradicated.\n", disease->name, day_counter);
		fprintf(EndLog, "%s has rampaged around the world for %d daysbut wasn't stopped in time, the entire world was eradicated.\n", disease->name, day_counter);
		printf("Containment efforts failed and the disease spread uncontrollably, leading to the collapse of societies worldwide.\n");
		fprintf(EndLog, "Containment efforts failed and the disease spread uncontrollably, leading to the collapse of societies worldwide.\n");
		printf("For a full log with a list of all regions sorted from most affected to least affected check the file called %s.txt", disease->name);
		AffectedRegions* affected_Region_List = SortAffectedRegions(world_regions);
		SaveAffectedRegionListToFile(affected_Region_List, EndLog);
	}
	else if (world->disease_detected == 1 && world->disease_cured == 1) {
		printf("%s has been detected.\n", disease->name);
		fprintf(EndLog, "%s has been detected.\n", disease->name);
		printf("%s has rampaged around the world for %d days but was eventually contained and cured.\n", disease->name, day_counter);
		fprintf(EndLog, "%s has rampaged around the world for %d days but was eventually contained and cured.\n", disease->name, day_counter);
		printf("Thanks to global cooperation and rapid medical advancements, humanity managed to develop a cure and eradicate the disease.\n");
		fprintf(EndLog, "Thanks to global cooperation and rapid medical advancements, humanity managed to develop a cure and eradicate the disease.\n");
		printf("For a full log with a list of all regions sorted from most affected to least affected check the file called %s.txt", disease->name);
		AffectedRegions* affected_Region_List = SortAffectedRegions(world_regions);
		SaveAffectedRegionListToFile(affected_Region_List, EndLog);
	}
	fclose(EndLog);
}

FILE* OpenEndLog(const char* filename) {
    char fileN[100];
    strcpy_s(fileN, sizeof(fileN), filename);
    strcat_s(fileN, sizeof(fileN), ".txt");
    FILE* EndLog = fopen(fileN, "w");
    if (EndLog == NULL) {
        printf("Error opening end log file.\n");
        return NULL;
    }
    return EndLog;
}