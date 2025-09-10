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
	printf("Borders closed in %s.\n", region->name);
}

void Curfew(Regions* region, Disease* disease) {
	disease->infectiousness = (int)(disease->infectiousness * 0.4);
	printf("Curfew imposed in %s.\n", region->name);
}

void InvestInResearch(Regions* region, World* world) {
	if (region->research_investment < 1000) {
		region->research_investment += 100; // increase investment
		printf("Increased research investment in %s to %d.\n", region->name, region->research_investment);
	}
}

void IsCureReached(World* world) {
	if (world->vaccine_progress >= 1000) {
		world->disease_cured = 1;
		printf("Disease cured! Starting vaccine distribution!\n");
	}
}

void anti_vaxxers(Regions* region, Disease* disease) {
	disease->infectiousness += 5; // increase infectiousness due to anti-vaxxer movement
	if (disease->infectiousness > 100) {
		return;
	}
	printf("Anti-vaxxer movement in %s increased infectiousness.\n", region->name);
}

void vaccine_progress_up(World* world) {
	if (world->disease_detected == 0) {
		return; // can't progress vaccine if disease not detected
	}
	if (world->vaccine_progress > 1000) {
		return;
	}
	world->vaccine_progress += 10; // increase vaccine progress
	printf("Vaccine progress increased.\n");
}

void vaccine_progress_down(World* world) {
	if (world->disease_detected == 0) {
		return; // can't regress vaccine if disease not detected
	}
	if (world->vaccine_progress < 1) {
		world->vaccine_progress = 1; // cap at 1
		return;
	}
	world->vaccine_progress -= 30; // decrease vaccine progress
	printf("Vaccine progress decreased.\n");
}

void public_opinion_escalate(Regions* region, Disease* disease) {
	disease->infectiousness = (int)(disease->infectiousness * 1.15);
	printf("Public opinion has increased the infectiousness to %d\n", disease->infectiousness);
}

void public_opinion_mitigate(Regions* region, Disease* disease) {
	disease->infectiousness = (int)(disease->infectiousness * 0.85);
	printf("Public opinion mitigated the infectiousness to %d", disease->infectiousness);
}

void public_opinion(Regions* region, Disease* disease) {
	if ((rand() % 2) == 0) {
		prublic_opinion_escalate(region, disease);
	}
	else {
		public_opinion_mitigate(region, disease);
	}
}
