#include <stdio.h>
#include "disease.h"
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

void DayLoop(Regions* current_region, const Disease* disease, const World* world) {
	while (current_region) {
		//do actions on regions
		Infect(disease->infectiousness, current_region->sick_people, world->healthy_people);
		Kill(current_region->sick_people, disease->lethality);
		//Cure()
		current_region = current_region->next_region; // moves to next item
	}
}

void closing_borders(Regions* region, Disease* disease){
	disease->infectiousness = (int)(disease->infectiousness * 0.7);
	printf("Borders closed in %s, infectiousness reduced to %d\n", region->name);
}

void curfew(Regions* region) {
	region->population_density = (int)(region->population_density * 0.4);
	printf("Curfew imposed in %s, population density reduced to %d\n", region->name);
}

void invest_in_research(Regions* region, World* world) {
	if (region->research_investment > 1000) {
		world->disease_cured = 1;
		printf("Disease cured due to research investment in %s!\n", region->name);
	}
	else {
		region->research_investment += 100; // increase investment
		printf("Increased research investment in %s to %d\n", region->name, region->research_investment);
	}
}