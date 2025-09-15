#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "design.h"
#include "disease.h"
#include "world.h"

void print_region(const Regions* region) {
	printf("%s\n", region->name);
	printf("Region Status:\n");
	printf("Healthy People: %lld\n", region->healthy_people);
	printf("Sick People: %lld\n", region->sick_people);
	printf("Dead People: %lld\n", region->dead_people);
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

void UpdateWorld(World* world, Regions* world_regions) {
    Regions* current_region = world_regions;
    int count = 0;
    while (current_region)
    {
        count++;
        world->healthy_people += current_region->healthy_people;
        world->sick_people += current_region->sick_people;
        world->dead_people += current_region->dead_people;
        current_region = current_region->next_region; // moves to next item
    }
}

void ChooseSimpleEvent(Regions* current_region, Disease* disease, World* world, int* mutation_enable) { // not finished
	int event = rand() % 6;//number of events +1
    int rng = rand() % 20;
	switch (event)
	{
	case 0:
        if (rng < 10) {
            vaccine_progress_up(world);
            PrintColored("There have been major breakthrough in vaccine development in ", BLUE);
            printf("%s. \n", current_region->name);

		}
		else if(rng > 16){
            vaccine_progress_down(world);
            // need to add message
		}
        else {
            printf("No event this week in %s.\n", current_region->name);
        }
		break;
	case 1:
        if (world->disease_detected == 1) {
            if (world->vaccine_progress > 500) {
                if (world->vaccine_progress > 850) {
                    if (world->vaccine_progress > 999) {// becomes more likely as vaccine progresses
                        if (rng > 9) {
                            anti_vaxxers(current_region, disease);
                            // need to add mesage
                            break;
                        }
                    }
                    if (rng > 13) {
                        anti_vaxxers(current_region, disease);
                        // need to add mesage
                        break;
                    }
                }
                if (rng > 16) {
                    anti_vaxxers(current_region, disease);
                    // need to add mesage
                    break;
                }
            }
        }
		else {
			printf("No event this week in %s.\n", current_region->name);
		}
		break;
	case 2:
        if (world->disease_detected == 1) {
            if (rng > 13) {
                public_opinion_escalate(current_region, disease);
                // need to add mesage
            }
            else if (rng < 3) {
                public_opinion_mitigate(current_region, disease);
                // need to add mesage
            }
            else {
                printf("No event this week in %s.\n", current_region->name);
            }
        }
		break;
    case 3:
        if (rng < 13) {
            plague_mutation(disease, mutation_enable);
            // need to add mesage inside the function
        }
        break;
	default:
		printf("No event this week in %s.\n", current_region->name);
		break;
	}
}

void DayLoop(Regions* current_region, Disease* disease, World* world, int day_counter, Regions* world_regions) {
    int mutation_enable = 1;
	while (current_region) {
		//do actions on regions
        if (current_region->healthy_people <= 0) {
            current_region->healthy_people = 0;
        }
        else
        {
            long long new_infected = Infect(disease->infectiousness, current_region->sick_people, current_region->healthy_people);
            current_region->healthy_people -= new_infected;
            current_region->sick_people += new_infected;
        }

        if (current_region->sick_people <= 0) {
            current_region->sick_people = 0;
        }
        else {// only enter if country is infected (sick_people > 0)
            long long new_deceased = Kill(current_region->sick_people, disease->lethality, current_region->healthy_people, disease->severity);
            current_region->sick_people -= new_deceased;
            current_region->dead_people += new_deceased;
            if (current_region->sick_people > 0 && world->disease_detected == 0) {
                DiseaseDetected(current_region, disease, world);
            }

            if (day_counter % 7 == 0) { // event can happen once a week
                ChooseSimpleEvent(current_region, disease, world, &mutation_enable);
                Sleep(1000);
            }
            if (day_counter % 30 == 0) {// special event once a month
                TriggerInfectOtherRegion(current_region, world_regions);
                Sleep(1000);
                print_World(world);
                Sleep(1000);
            }
            //Cure()
        }
		current_region = current_region->next_region; // moves to next item
	}
}

void ClosingBorders(Regions* region, Disease* disease){// implemented
	disease->infectiousness = (int)(disease->infectiousness * 0.7);
	printf("Borders closed in %s.\n", region->name);
}

void Curfew(Regions* region, Disease* disease) {// might cut 
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

void anti_vaxxers(Regions* region, Disease* disease) {// implemented
	disease->infectiousness += 3; // increase infectiousness due to anti-vaxxer movement
	if (disease->infectiousness > 100) {
		return;
	}
	printf("Anti-vaxxer movement in %s increased infectiousness.\n", region->name);
}

void vaccine_progress_up(World* world) { // implemented
	if (world->disease_detected == 0) {
		return; // can't progress vaccine if disease not detected
	}
    else {
        if (world->vaccine_progress > 1000) {
            return;
        }
        else {
            world->vaccine_progress += 10; // increase vaccine progress
            printf("Vaccine progress increased.\n");
        }
    }
}

void vaccine_progress_down(World* world) { // implemented
	if (world->disease_detected == 0) {
		return; // can't regress vaccine if disease not detected
	}
    else {
        world->vaccine_progress -= 30; // decrease vaccine progress
        printf("Vaccine progress decreased.\n");
        if (world->vaccine_progress < 1) {
            world->vaccine_progress = 1; // cap at 1
            return;
        }
    }
}

void public_opinion_escalate(Regions* region, Disease* disease) { // implemented
    if (disease->infectiousness > 40) {
        disease->infectiousness = (int)(disease->infectiousness * 1.04);
        printf("Public in %s dont follow regulations\n", region->name);
    }
    else {
        disease->infectiousness = (int)(disease->infectiousness * 1.1);
        printf("Public in %s dont follow regulations\n", region->name);
    }
}

void public_opinion_mitigate(Regions* region, Disease* disease) {// implemented
    if (disease->infectiousness > 40) {
        disease->infectiousness = (int)(disease->infectiousness * 0.95);
        printf("Public in %s are following regulations more closely\n", region->name);
    }
    else {
        disease->infectiousness = (int)(disease->infectiousness * 0.85);
        printf("Public in %s are following regulations more closely\n", region->name);
    }
}

void public_opinion(Regions* region, Disease* disease) {// might cut
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

void InfectRandomRegion(Regions* world_regions, Regions* exclude_region) {
    // Count regions
    int count = 0;
    Regions* curr = world_regions;
    while (curr) {
        if (curr != exclude_region) count++;
        curr = curr->next_region;
    }
    if (count == 0) return;

    // Pick random region (excluding the current one)
    int target = rand() % count;
    curr = world_regions;
    while (curr) {//run through region list each time reducing target by 1 untill 0
        if (curr != exclude_region) {
            if (target == 0) {
                // Infect this region 
                curr->healthy_people -= 100;
                curr->sick_people += 100;
                printf("Random infection event: A flight with an infected person has left %s and arrived at %s .\n", exclude_region->name , curr->name);
                break;
            }
            target--;
        }
        curr = curr->next_region;
    }
}

void TriggerInfectOtherRegion(Regions* current_region, Regions* world_regions) {
    long long population = current_region->healthy_people + current_region->sick_people + current_region->dead_people;
    if (population > 0 && (current_region->sick_people > (population / 10))) {
        InfectRandomRegion(world_regions, current_region);
    }
}

void DiseaseDetected(Regions* region, Disease* disease, World* world) {
    // Only proceed if disease wasn't already detected
    if (world->disease_detected == 0) {
        // Calculate total population
        double total_population = (double)(region->healthy_people + region->sick_people + region->dead_people);
        // Calculate infection rate (% of population infected)
        double infection_rate = (double)region->sick_people / total_population;
        // Calculate death rate (% of infected who died)
        double death_rate = 0.0;
        if (region->sick_people + region->dead_people > 0) {
            death_rate = (double)region->dead_people / (region->sick_people + region->dead_people);
        }
        // Factor in population density for spread risk
        double spread_risk = infection_rate * (region->population_density / 10.0);
        // Adjust research investment based on spread risk and death rate
        if (spread_risk > 0.15 || death_rate > 0.1) {
            world->disease_detected = 1;
            PrintColored("URGENT: Highly infectious or lethal disease detected in ", RED);
            printf("%s!\n", region->name);
        } else if (spread_risk > 0.1 || death_rate > 0.5) {
            world->disease_detected = 1;
            PrintColored("WARNING: Disease outbreak detected in ", YELLOW);
            printf("%s!\n", region->name);
        } 
        // Print detailed status
        printf("Initial Analysis:\n");
        printf("Infection Rate: %.2f%%\n", infection_rate * 100);
        printf("Death Rate: %.2f%%\n", death_rate * 100);
        printf("Population Density Risk Factor: %d/10\n", region->population_density);
        printf("Initial Research Investment: %d\n", region->research_investment);
        // If highly dangerous, trigger immediate response
        if (death_rate > 0.15 || (spread_risk > 0.25 && death_rate > 0.1)) {
            ClosingBorders(region, disease);
        }
    }
}