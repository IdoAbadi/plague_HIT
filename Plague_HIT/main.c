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
    while (world.sick_people != 0 && world.healthy_people != 0 || world.disease_cured == 0)
    {
        Regions* current_region = world_regions; // precaution to not run up loop
        if (day_counter % 7 == 0) {
            int week_num = day_counter / 7;
            PrintColored("week ", ORANGE);
            printf("%d:\n", week_num);
        }
        UpdateWorld(&world, world_regions);
        DayLoop(current_region, &disease, &world, day_counter, current_region);
        if (world.disease_detected == 1) {
            // Initialize all regions' research investment when disease is first detected
            Regions* curr = world_regions;
            while (curr) {
                curr->research_investment = curr->development_level * 10;
                curr = curr->next_region;
            }
        }
        if (day_counter % 30 == 0) {
            print_World(&world);
        }
        day_counter++;
    }

    CloseRegionDataFile(Region_Data);
    freeRegions(world_regions);
}