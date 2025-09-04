#include<stdio.h>
#include "disease.h"
#include "world.h"

void main() {
    Disease* disease;
    World* world = malloc(sizeof(World));
    if (!world) {
        printf("world allocation failed");
    }
    Regions* world_regions = malloc(sizeof(Regions*));
    if (!world_regions) {
        printf("world regions allocation failed");
    }
    //print_disease(&disease);
    //print_region(&world_regions);
    SetUpWorld(world, world_regions);
    while (world->sick_people != 0 && world->healthy_people != 0 || world->disease_cured == 1)
    {
        while (world_regions) {
            //do actions on regions
        }
    }

}