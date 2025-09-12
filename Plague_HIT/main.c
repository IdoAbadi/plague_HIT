#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include "disease.h"
#include "world.h"
#include "FileFuncs.h"

void main() {
    int regions_to_aloocate = region_amount;
    srand(time(NULL)); // seed randomness
    int day_counter = 1;
    Disease disease; // if pointer decay issues come up allocate dynamically to solve
    World world = {0}; // if pointer decay issues come up allocate dynamically to solve
    Regions* world_regions = AllocateRegions(regions_to_aloocate); //creates region list

    FILE* Region_Data = OpenRegionData();// opens regiondata file
    SetRegionsParams(world_regions, Region_Data);//inputs all regions starting data
    SetUpWorld(&world, world_regions); // inputs data to world
    SetUpDisease(&disease); // user inputs data to disease 
    while (world.sick_people != 0 && world.healthy_people != 0 || world.disease_cured == 0)
    {
        Regions* current_region = world_regions; // precaution to not run up loop
        DayLoop(current_region, &disease, &world, day_counter);
        day_counter++;
    }

    CloseRegionDataFile(Region_Data);
    freeRegions(world_regions);
}