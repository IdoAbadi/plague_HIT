#include<stdio.h>
#include <time.h>
#include "disease.h"
#include "world.h"

void main() {
    srand(time(NULL)); // seed randomness
    int day_counter = 1;
    Disease disease;
    World world; // if pointer decay issues come up allocate dynamically to solve
    Regions* world_regions = malloc(sizeof(Regions*));
    if (!world_regions) {
        printf("world regions allocation failed");
        return;
    }
    //print_disease(&disease);
    //print_region(&world_regions);
    SetUpWorld(&world, world_regions);
    SetUpDisease(&disease);
    while (world.sick_people != 0 && world.healthy_people != 0 || world.disease_cured == 1)
    {
        Regions* current_region = world_regions; // precaution to not run up loop
        DayLoop(current_region, &disease, &world, day_counter);
        day_counter++;
    }
}