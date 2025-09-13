#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	printf("Healthy People: %lld\n", world->healthy_people);
	printf("Sick People: %lld\n", world->sick_people);
	printf("Dead People: %lld\n", world->dead_people);
}

void SetUpWorld(World* world , Regions* world_regions) {
	Regions* current_region = world_regions;
	int count = 0;
	while (current_region)
	{
		count++;
		world->healthy_people += current_region->healthy_people;
		world->sick_people += current_region->sick_people;
		current_region = current_region->next_region; // moves to next item
	}
}

void ChooseEvent(Regions* current_region, Disease* disease, World* world) { // not finished
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
		int new_infected = Infect(disease->infectiousness, current_region->sick_people, world->healthy_people);
        current_region->healthy_people -= new_infected;
        current_region->sick_people += new_infected;
		int new_deceased = Kill(current_region->sick_people, disease->lethality);
        current_region->sick_people -= new_deceased;
        current_region->dead_people += new_deceased;
		if (day_counter % 7 == 0) { // event can happen once a week
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
		public_opinion_escalate(region, disease);
	}
	else {
		public_opinion_mitigate(region, disease);
	}
}

void print_infected_regions(Regions* world_regions) {
	Regions* current_region = world_regions;
	printf("Infected Regions:\n");
	while (current_region) {
		if (current_region->sick_people > 0) {
			printf("%s\n", current_region->name);
		}
		current_region = current_region->next_region;
	}
}

void StartDisease(Regions* world_regions, char chosen_region[50]) {
	Regions* curr_region = world_regions;
	while (curr_region) {
        if (strcmp(curr_region->name, chosen_region) == 0) {
            curr_region->healthy_people -= 100;
			curr_region->sick_people = 100;
			break;
		}
		curr_region = curr_region->next_region;
	}
}

void SelectDiseaseOrigin(struct Regions* world_regions, int continent) {
    int region = 0;
    char region_name[50];
    int valid = 0;
    while (!valid) {
        switch (continent)
        {
        case 1:
            printf("Choose a region:\n");
            printf("1) United States & Canada \n2) Latin America \n3) Central America\n");
            scanf_s("%d", &region);
            if (region == 1) {
                strcpy_s(region_name, sizeof(region_name), "United States & Canada");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 2) {
                strcpy_s(region_name, sizeof(region_name), "Latin America");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 3) {
                strcpy_s(region_name, sizeof(region_name), "Central America");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else {
                printf("Invalid input, try again\n");
                clean_input_buffer();
            }
            break;
        case 2:
            printf("Choose a region:\n");
            printf("1) Western Europe \n2) Eastern Europe \n3) United Kingdom\n");
            scanf_s("%d", &region);
            if (region == 1) {
                strcpy_s(region_name, sizeof(region_name), "Western Europe");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 2) {
                strcpy_s(region_name, sizeof(region_name), "Eastern Europe");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 3) {
                strcpy_s(region_name, sizeof(region_name), "United Kingdom");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else {
                printf("Invalid input, try again\n");
                clean_input_buffer();
            }
            break;
        case 3:
            printf("Choose a region:\n");
            printf("1) North Africa \n2) Sub-Saharan Africa \n");
            scanf_s("%d", &region);
            if (region == 1) {
                strcpy_s(region_name, sizeof(region_name), "North Africa");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 2) {
                strcpy_s(region_name, sizeof(region_name), "Sub-Saharan Africa");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else {
                printf("Invalid input, try again\n");
                clean_input_buffer();
            }
            break;
        case 4:
            printf("Choose a region:\n");
            printf("1) Russia \n2) Middle East \n3) South Asia \n4) India \n5) China \n6) Hong Kong and Macau \n7) Central Asia \n8) Southeast Asia\n");
            scanf_s("%d", &region);
            if (region == 1) {
                strcpy_s(region_name, sizeof(region_name), "Russia");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 2) {
                strcpy_s(region_name, sizeof(region_name), "Middle East");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 3) {
                strcpy_s(region_name, sizeof(region_name), "South Asia");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 4) {
                strcpy_s(region_name, sizeof(region_name), "India");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 5) {
                strcpy_s(region_name, sizeof(region_name), "China");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 6) {
                strcpy_s(region_name, sizeof(region_name), "Hong Kong and Macau");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 7) {
                strcpy_s(region_name, sizeof(region_name), "Central Asia");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 8) {
                strcpy_s(region_name, sizeof(region_name), "Southeast Asia");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else {
                printf("Invalid input, try again\n");
                clean_input_buffer();
            }
            break;
        case 5:
            printf("Choose a region:\n");
            printf("1) Australia \n2) New Zealand \n3) Small Pacific nations\n");
            scanf_s("%d", &region);
            if (region == 1) {
                strcpy_s(region_name, sizeof(region_name), "Australia");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 2) {
                strcpy_s(region_name, sizeof(region_name), "New Zealand");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 3) {
                strcpy_s(region_name, sizeof(region_name), "Small Pacific nations");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else {
                printf("Invalid input, try again\n");
                clean_input_buffer();
            }
            break;
        case 6:
            printf("Choose a region:\n");
            printf("1) Japan, Korea, and Taiwan \n2) Caribbean \n");
            scanf_s("%d", &region);
            if (region == 1) {
                strcpy_s(region_name, sizeof(region_name), "Japan, Korea, and Taiwan");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else if (region == 2) {
                strcpy_s(region_name, sizeof(region_name), "Caribbean");
                StartDisease(world_regions, region_name);
                valid = 1;
            }
            else {
                printf("Invalid input, try again\n");
                clean_input_buffer();
            }
            break;
        default:
            printf("invalid choice, try again\n");
            clean_input_buffer();
            return;
        }
    }
}

