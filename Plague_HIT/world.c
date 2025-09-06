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

void ChooseEvent(Regions* current_region, Disease* disease, World* world) {
	int event = rand() % 6;//number of events +1
	switch (event)
	{
	case 0:
		if (world->disease_detected == 1) {
			ClosingBorders(current_region, disease);
		}
		else {
			printf("No event this week in %s.\n", current_region->name);
		}
		break;
	case 1:
		if (world->disease_detected == 1) {
			Curfew(current_region, disease);
		}
		else {
			printf("No event this week in %s.\n", current_region->name);
		}
		break;
	case 2:
		//something
		break;
	default:
		printf("No event this week in %s.\n", current_region->name);
		break;
	}
}

void DayLoop(Regions* current_region, Disease* disease, World* world, int day_counter) {
	while (current_region) {
		//do actions on regions
		Infect(disease->infectiousness, current_region->sick_people, world->healthy_people);
		Kill(current_region->sick_people, disease->lethality);
		if (day_counter % 7 == 0) {
			ChooseEvent(current_region, disease, world);
		}
		//Cure()
		current_region = current_region->next_region; // moves to next item
	}
}

void ClosingBorders(Regions* region, Disease* disease){
	disease->infectiousness = (int)(disease->infectiousness * 0.7);
	printf("Borders closed in %s\n", region->name);
}

void Curfew(Regions* region, Disease* disease) {
	disease->infectiousness = (int)(disease->infectiousness * 0.4);
	printf("Curfew imposed in %s\n", region->name);
}

void InvestInResearch(Regions* region, World* world) {
	if (region->research_investment > 1000) {
		world->disease_cured = 1;
		printf("Disease cured due to research investment in %s!\n", region->name);
	}
	else {
		region->research_investment += 100; // increase investment
		printf("Increased research investment in %s to %d\n", region->name, region->research_investment);
	}
}

void Vaccine(Regions* region, Disease* disease, World* world) {
	if (!world || !region || !disease) {
		return; // if one is null, return (defense the code from crashing)
	}
	if (world->disease_detected == 0) {
		return; // can't vaccinate if disease not detected
	}

	int new_infectiousness = (int)(disease->infectiousness * 0.9);
	if (new_infectiousness < 1) {
		new_infectiousness = 1; // so the disease will never be completely gone
	}

	if (new_infectiousness < disease->infectiousness) {
		disease->infectiousness = new_infectiousness;
		printf("Vaccine deployed in %s, reducing infectiousness to %d\n", region->name, disease->infectiousness);
	}
}

void anti_vaxxers(Regions* region, Disease* disease) {
	if (!region || !disease) {
		return; // defense the code from crashing
	}
	disease->infectiousness += 5; // increase infectiousness due to anti-vaxxer movement
	
	if (disease->infectiousness > 100) {
		disease->infectiousness = 100; // if infectiousness goes over 100, cap it at 100
	}
	printf("Anti-vaxxer movement in %s increased infectiousness to %d\n", region->name, disease->infectiousness);
}

void vaccine_progress_up(Regions* region, World* world) {
	if (!region || !world) {
		return; // defense the code from crashing
	}
	if (world->disease_detected == 0) {
		return; // can't progress vaccine if disease not detected
	}
	if (world->vaccine_progress > 100) {
		world->vaccine_progress = 100; // cap at 100
		return;
	}
	world->vaccine_progress += 10; // increase vaccine progress
	if (world->vaccine_progress > 100) {
		world->vaccine_progress = 100; // cap at 100
	}
	printf("Vaccine progress in %s increased to %d/100.\n", region->name, world->vaccine_progress);
}

void vaccine_progress_down(Regions* region, World* world) {
	if (!region || !world) {
		return; // defense the code from crashing
	}
	if (world->disease_detected == 0) {
		return; // can't regress vaccine if disease not detected
	}
	if (world->vaccine_progress < 0) {
		world->vaccine_progress = 0; // cap at 0
		return;
	}
	world->vaccine_progress -= 30; // decrease vaccine progress
	if (world->vaccine_progress < 0) {
		world->vaccine_progress = 0; // cap at 0
	}
	printf("Vaccine progress in %s decreased to %d/100.\n", region->name, world->vaccine_progress);
}