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
	world->disease_cured = 0;
	world->disease_detected = 0;
	while (world_regions)
	{
		world->healthy_people += world_regions->healthy_people;
		world->sick_people += world_regions->sick_people;
		world_regions = world_regions->next_region;
	}
}
