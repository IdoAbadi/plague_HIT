#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "FileFuncs.h"
#include "sort.h"
#include "world.h"

int RegionAffectedPopulation(Regions* current_region) {
	long long helthy = current_region->healthy_people;
	long long sick = current_region->sick_people;
	long long dead = current_region->dead_people;
	long long total = helthy + sick + dead;

    if (total == 0) {
		printf("Edge case: total population is zero in region %s.\n", current_region->name);
        return 0; // Avoid division by zero
	}

	long long affected = sick + dead;

	int affected_percentage = (int)((affected * 100) / total);
	return affected_percentage;
}

AffectedRegions* SortAffectedRegions(Regions* world_regions) {
    AffectedRegions* affected_array = malloc(21 * sizeof(AffectedRegions));
    if (affected_array == NULL) {
        printf("Memory allocation failed for affected_array.\n");
        return NULL; // Handle memory allocation failure
    }
    int i, j;
    AffectedRegions temp;

    // Fill affected_array with names + affected percentage
    for (i = 0; i < 21; i++) {
        strcpy(affected_array[i].name, world_regions->name);
        affected_array[i].affected_percentage = RegionAffectedPopulation(world_regions);
		world_regions = world_regions->next_region;
    }
    // Bubble sort: descending order by affected_percentage
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20 - i; j++) {
            if (affected_array[j].affected_percentage < affected_array[j + 1].affected_percentage) {
				strcpy(temp.name, affected_array[j].name);
                temp.affected_percentage = affected_array[j].affected_percentage;
                affected_array[j] = affected_array[j + 1];
                strcpy(affected_array[j + 1].name, temp.name);
                affected_array[j + 1].affected_percentage = temp.affected_percentage;
            }
        }
    }
    return affected_array;
}

void SaveAffectedRegionListToFile(AffectedRegions* affected_array, FILE* file) {
    for (int i = 0; i < 21; i++) {
        fprintf(file, "Region: %s, Affected Percentage: %d%%\n", affected_array[i].name, affected_array[i].affected_percentage);
    }
	fclose(file);
}

void AppendToFile(AffectedRegions* affected_array) {
    FILE* file = fopen("FileFuncs.txt", "a");
    if (file == NULL) {
        printf("Error opening file for appending.\n");
        return;
    }
    for (int i = 0; i < region_amount; i++) {
        fprintf(file, "Region: %s, Affected Percentage: %d%%\n", affected_array->name, affected_array->affected_percentage);
    }
    fclose(file);
}