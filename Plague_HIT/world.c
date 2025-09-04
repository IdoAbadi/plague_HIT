#include <stdio.h>
#include "world.h"

void print_region(const Regions* region) {
	printf("%s\n", region->name);
	printf("Region Status:\n");
	printf("Healthy People: %d\n", region->healthy_people);
	printf("Sick People: %d\n", region->sick_people);
	printf("Dead People: %d\n", region->dead_people);
	printf("Research Investment: %d\n", region->research_investment);
	printf("Development Level: %d\n", region->development_level);
	printf("Population Density: %d\n", region->population_density);
}

void print_World(const World* world) {
	printf("Healthy People: %d\n", world->healthy_people);
	printf("Sick People: %d\n", world->sick_people);
	printf("Dead People: %d\n", world->dead_people);
}

void SetUpWorld(World* world ,const Regions* world_regions) {
	Regions* current_region = world_regions;
	world->disease_cured = 0;
	world->disease_detected = 0;
	while (current_region)
	{
		world->healthy_people += current_region->healthy_people;
		world->sick_people += current_region->sick_people;
		current_region = current_region->next_region; // moves to next item
	}
}


void DayLoop(Regions* current_region) {
	while (current_region) {
		//do actions on regions
		current_region = current_region->next_region; // moves to next item
	}
}
