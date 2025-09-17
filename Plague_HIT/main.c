#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include "disease.h"
#include "world.h"
#include "FileFuncs.h"
#include "design.h"
#include "sort.h"

void main() {
    int regions_to_aloocate = region_amount;
    srand((unsigned int)time(NULL)); // seed randomness
    int day_counter = 1;
    PrintWelcomeMessage();
    Disease disease; // if pointer decay issues come up allocate dynamically to solve
    World world = {0}; // if pointer decay issues come up allocate dynamically to solve
    Regions* world_regions = AllocateRegions(regions_to_aloocate); //creates region list
    FILE* Region_Data = OpenRegionData();// opens regiondata file
    SetRegionsParams(world_regions, Region_Data);//inputs all regions starting data
    SetUpWorld(&world, world_regions); // inputs data to world
    SetUpDisease(&disease); // user inputs data to disease 
    int start_continent = ChooseContinent();
    SelectDiseaseOrigin(world_regions, start_continent);
    ClearConsole();
    CloseRegionDataFile(Region_Data);
    UpdateWorld(&world, world_regions);
    Regions* world_regions_copy = world_regions;// precaution to not run up nested loops
    while (world.sick_people >= 0 || world.disease_cured == 1)
    {
        Regions* current_region = world_regions; // precaution to not run up nested loops
        DayLoop(current_region, &disease, &world, day_counter, world_regions_copy);
        WeekLoop(day_counter, world_regions_copy, &disease, &world);
        SetUpInvestment(&world, world_regions_copy);
        MonthLog(day_counter, &world, world_regions_copy);
        day_counter++;
    }
    PrintEndLog(&disease, &world, day_counter, world_regions_copy);
    printf("end");
    freeRegions(world_regions);
}