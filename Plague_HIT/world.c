#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "design.h"
#include "disease.h"
#include "world.h"
#include "matrix.h"

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
	PrintColored("World Status:\n", PURPLE);
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
    // Reset totals before summing
    world->healthy_people = 0;
    world->sick_people = 0;
    world->dead_people = 0;
    
    while (current_region)
    {
        world->healthy_people += current_region->healthy_people;
        world->sick_people += current_region->sick_people;
        world->dead_people += current_region->dead_people;
        current_region = current_region->next_region;
    }
}

void ChooseSimpleEvent(Regions* current_region, Disease* disease, World* world, int* mutation_enable) { 
	int event = rand() % 5;//number of events +2
    int rng = rand() % 20;
	switch (event)
	{
	case 0:
        if (world->disease_detected == 1) {
            if (rng < 10) {
                vaccine_progress_up(world);
                PrintColored("There have been major breakthrough in vaccine development in ", GREEN);
                printf("%s.\n", current_region->name);
            }
            else if (rng > 16) {
                vaccine_progress_down(world);
                PrintColored("Vaccine progress is going down because of lack of resources in ", ORANGE);
                printf("%s.\n", current_region->name);
            }
        }
        else {
            PrintColored("No event this week in ", CYAN);
            printf("%s.\n", current_region->name);
        }
		break;
	case 1:
        if (world->disease_detected == 1) {
            if (world->vaccine_progress > 500) {
                if (world->vaccine_progress > 850) {
                    if (world->vaccine_progress > 999) {// becomes more likely as vaccine progresses
                        if (rng > 9) {
                            anti_vaxxers(current_region, world);
                            break;
                        }
                        else {
                            PrintColored("No event this week in ", CYAN);
                            printf("%s.\n", current_region->name);
                        }
                    }
                    if (rng > 13) {
                        anti_vaxxers(current_region, world);
                        break;
                    }
                    else {
                        PrintColored("No event this week in ", CYAN);
                        printf("%s.\n", current_region->name);
                    }
                }
                if (rng > 16) {
                    anti_vaxxers(current_region, world);
                    break;
                }
                else {
                    PrintColored("No event this week in ", CYAN);
                    printf("%s.\n", current_region->name);
                }
            }
            else {
                PrintColored("No event this week in ", CYAN);
                printf("%s.\n", current_region->name);
            }
        }
		else {
            PrintColored("No event this week in ", CYAN);
            printf("%s.\n", current_region->name);
		}
		break;
	case 2:
        if (world->disease_detected == 1) {
            if (rng > 13) {
                public_opinion_escalate(current_region, disease);
                PrintColored("Public opinion escalates, increasing the crisis in ", ORANGE);
                printf("%s. \n", current_region->name);
            }
            else if (rng < 3) {
                public_opinion_mitigate(current_region, disease);
                PrintColored("Public cooperation helps soften the virus impact in ", BLUE);
                printf("%s. \n", current_region->name);
            }
            else {
                PrintColored("No event this week in ", CYAN);
                printf("%s.\n", current_region->name);
            }
        }
        else {
            PrintColored("No event this week in ", CYAN);
            printf("%s.\n", current_region->name);
        }
		break;
    case 3:
        if (rng < 13) {
            plague_mutation(disease, mutation_enable);
            PrintColored("The disease has been mutated, new strain of the virus was found in ", RED);
            printf("%s.\n", current_region->name);
        }
        else {
            PrintColored("No event this week in ", CYAN);
            printf("%s.\n", current_region->name);
        }
        break;
	default:
        PrintColored("No event this week in ", CYAN);
        printf("%s.\n", current_region->name);
		break;
	}
}

void DayLoop(Regions* current_region, Disease* disease, World* world, int day_counter, Regions* world_regions) {
    double total_research_progress = 0.0;
    while (current_region) {
		//do actions on regions
        if (current_region->healthy_people < 0) {
            current_region->healthy_people = 0;
        }
        else
        {
            long long new_infected = Infect(disease->infectiousness, current_region->sick_people, current_region->healthy_people);
            current_region->healthy_people -= new_infected;
            current_region->sick_people += new_infected;
        }
        if (current_region->sick_people < 0) {
            current_region->sick_people = 0;
        }
        else {// only enter if country is infected (sick_people > 0)
            long long new_deceased = Kill(current_region->sick_people, disease->lethality, current_region->healthy_people, disease->severity, current_region->population_density);
            current_region->sick_people -= new_deceased;
            current_region->dead_people += new_deceased;
            // Calculate research progress if disease is detected
            if (world->disease_detected == 1 && current_region->sick_people > 0) {
                double region_research = CalculateRegionResearch(current_region, disease);
                total_research_progress += region_research;
            }
            
        }
        Cure(disease, world, current_region);
        UpdateWorld(world, world_regions);
		current_region = current_region->next_region; // moves to next item
	}

    // Apply accumulated research progress
    if (world->disease_detected == 1) {
        world->vaccine_progress += (int)total_research_progress;
        if (world->vaccine_progress > CURE_REACHED) {
            world->vaccine_progress = CURE_REACHED;
        }
    }
}

void WeekLoop(int day_counter, Regions* world_regions, Disease* disease, World* world) {
    Regions* current_region = world_regions;
    int mutation_enable = 1;// mutation can only happen in one region each week
    if (day_counter % 7 == 0) { // event can happen once a week
        int week_num = day_counter / 7;
        PrintColored("week ", ORANGE);
        printf("%d:\n", week_num);
        while (current_region) {
            if (current_region->sick_people > 0 && world->disease_detected == 0) {
                DiseaseDetected(current_region, disease, world, world_regions);
            }
            if (current_region->sick_people > 0) {
                ChooseSimpleEvent(current_region, disease, world, &mutation_enable);
                Sleep(500);
            }
            current_region = current_region->next_region;
        }
    }
}

void MonthLog(int day_counter, World* world, Regions* world_regions) {
    if (day_counter % 30 == 0) {
        Regions* current_region = world_regions;
        print_World(world);
        PrintMatrix(world);
        if (world->disease_detected == 1) {
            PrintMonthlySpreadLog(world);
        }
        while (current_region) {
            if (current_region->sick_people > 0) {
                TriggerInfectOtherRegion(current_region, world_regions);
                Sleep(1000);
            }
            current_region = current_region->next_region;
        }
    }

}

void ClosingBorders(Regions* region, Disease* disease){// implemented
	disease->infectiousness = (int)(disease->infectiousness * 0.75);
	PrintColored("Borders closed in ", RED);
    printf("%s\n", region->name);
}

void Curfew(Regions* region, Disease* disease) {// might cut 
	disease->infectiousness = (int)(disease->infectiousness * 0.8);
	PrintColored("Curfew imposed in \n", RED);
    printf("%s\n", region->name);
}

void anti_vaxxers(Regions* region, World* world) {// implemented
	if (world->vaccine_progress < 10000) {
        world->vaccine_progress -= (int)((world->vaccine_progress * 3) / 100);
        PrintColored("Anti-vaxxer movement in ", RED);
        printf("%s", region->name);
        PrintColored(" decreased vaccine progress.\n", RED);
	}
    else {// vaccine reached
        return;
    }
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
            world->vaccine_progress += 50; // increase vaccine progress
        }
    }
}

void vaccine_progress_down(World* world) { // implemented
	if (world->disease_detected == 0) {
		return; // can't regress vaccine if disease not detected
	}
    else {
        world->vaccine_progress -= 120; // decrease vaccine progress
        PrintColored("Vaccine progress decreased.\n", YELLOW);
        if (world->vaccine_progress < 1) {
            world->vaccine_progress = 1; // cap at 1
            return;
        }
    }
}

void public_opinion_escalate(Regions* region, Disease* disease) { // implemented
    if (disease->infectiousness > 40) {
        disease->infectiousness = (int)(disease->infectiousness * 1.04);
        PrintColored("Public in ", YELLOW);
        printf("%s", region->name); 
        PrintColored(" dont follow regulations\n", YELLOW);
    }
    else {
        disease->infectiousness = (int)(disease->infectiousness * 1.1);
        PrintColored("Public in ", YELLOW);
        printf("%s", region->name);
        PrintColored(" dont follow regulations\n", YELLOW);
    }
}

void public_opinion_mitigate(Regions* region, Disease* disease) {// implemented
    if (disease->infectiousness > 40) {
        disease->infectiousness = (int)(disease->infectiousness * 0.95);
        PrintColored("Public in ", BLUE);
        printf("%s", region->name); 
        PrintColored(" are following regulations more closely\n", BLUE);
    }
    else {
        disease->infectiousness = (int)(disease->infectiousness * 0.85);
        PrintColored("Public in ", BLUE);
        printf("%s", region->name);
        PrintColored(" are following regulations more closely.\n", BLUE);
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
            PrintColored("Choose a region:\n", YELLOW);
            PrintColored("1) United States & Canada \n2) Latin America \n3) Central America\n", BLUE);
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
            PrintColored("Choose a region:\n", YELLOW);
            PrintColored("1) Western Europe \n2) Eastern Europe \n3) United Kingdom\n", BLUE);
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
            PrintColored("Choose a region:\n", YELLOW);
            PrintColored("1) North Africa \n2) Sub-Saharan Africa \n", BLUE);
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
            PrintColored("Choose a region:\n", YELLOW);
            PrintColored("1) Russia \n2) Middle East \n3) South Asia \n4) India \n5) China \n6) Hong Kong and Macau \n7) Central Asia \n8) Southeast Asia\n", BLUE);
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
            PrintColored("Choose a region:\n", YELLOW);
            PrintColored("1) Australia \n2) New Zealand \n3) Small Pacific nations\n", BLUE);
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
            PrintColored("Choose a region:\n", YELLOW);
            PrintColored("1) Japan, Korea, and Taiwan \n2) Caribbean \n", BLUE);
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
                PrintColored("Random infection event: A flight with an infected person has left ", ORANGE);
                printf("%s", exclude_region->name);
                PrintColored(" and arrived at ", ORANGE);
                printf("%s.\n", curr->name);
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

void DiseaseDetected(Regions* region, Disease* disease, World* world, Regions* world_regions) {
    if (world->disease_detected == 0 && region->dead_people > 100) {
        double total_population = (double)(region->healthy_people + region->sick_people + region->dead_people);
        // Calculate % of population infected
        double infection_rate = (double)region->sick_people / total_population;
        // Calculate death rate (% of infected who died)
        double death_rate = 0.0;
        if (region->sick_people + region->dead_people > 0) {
            death_rate = (double)region->dead_people / (region->sick_people + region->dead_people);
        }
        // Factor in population density for spread risk
        double spread_risk = infection_rate * (region->population_density / 10.0);
        // Adjust research investment based on spread risk and death rate
        if (death_rate > 0.3 || (spread_risk > 0.2 && death_rate > 0.25)) {
            world->disease_detected = 1;
            PrintColored("URGENT: Extremely infectious and lethal disease detected in ", RED);
            printf("%s!\n", region->name);
            Sleep(500);
            PrintInitialDetectionLog(infection_rate, death_rate, region);
            Sleep(500);
            ClosingBorders(region, disease);
        }
        else if (spread_risk > 0.15 || death_rate > 0.15) {
            world->disease_detected = 1;
            PrintColored("URGENT: Highly infectious or lethal disease detected in ", RED);
            printf("%s!\n", region->name);
            Sleep(500);
            PrintInitialDetectionLog(infection_rate, death_rate, region);
            Sleep(500);
        }
        else if (spread_risk > 0.1 || death_rate > 0.1) {
            world->disease_detected = 1;
            PrintColored("WARNING: Disease outbreak detected in ", ORANGE);
            printf("%s!\n", region->name);
            Sleep(500);
            PrintInitialDetectionLog(infection_rate, death_rate, region);
            Sleep(500);
        }
    }
}

void PrintInitialDetectionLog(double infection_rate, double death_rate, Regions* region) {
    PrintColored("Initial Analysis:\n", PINK);
    printf("Infection Rate: %.2f%%\n", infection_rate * 100);
    printf("Death Rate: %.2f%%\n", death_rate * 100);
    printf("Population Density Risk Factor: %d/10\n", region->population_density);
    printf("Initial Research Investment: %d\n", region->research_investment);

    //PrintColored("Research begins in ", BLUE);
    //printf("%s \n", region->name);
}

void PrintMonthlySpreadLog(World* world) {
    //calculation
    double total_population = (double)(world->healthy_people + world->sick_people + world->dead_people);
    // Calculate % of population infected
    double infection_rate = (double)world->sick_people / total_population;
    // Calculate death rate (% of infected who died)
    double death_rate = 0.0;
    if (world->sick_people + world->dead_people > 0) {
        death_rate = (double)world->dead_people / (world->sick_people + world->dead_people);
    }
    PrintColored("\nMonthly world Analysis:\n", PINK);
    PrintColored("Infection Rate: ", CYAN);
    printf(" % .2f % %\n", infection_rate * 100);
    PrintColored("Death Rate: ", CYAN);
    printf("% .2f % %\n", death_rate * 100);
    PrintColored("Global Vaccine Research Progress: ", BLUE);
    printf("%.1f%%\n\n", (world->vaccine_progress / 30.0));
}

// Add this new function to calculate research contribution from a region
double CalculateRegionResearch(Regions* region, Disease* disease) {
    double research_power = 0.0;
    double total_population = (double)(region->healthy_people + region->sick_people + region->dead_people);
    
    // Base research from healthy population
    double healthy_ratio = (double)region->healthy_people / total_population;
    research_power = healthy_ratio * region->research_resources;
    
    // Add research from sick people if disease isn't too severe
    if (disease->severity < 70 && region->sick_people > 0) {
        double sick_ratio = (double)region->sick_people / total_population;
        research_power += (sick_ratio * region->research_resources * 0.5); // Sick people research at 50% efficiency
    }
    
    // Scale by research investment level
    research_power *= (region->research_investment / 100.0);
    
    // A region with 100 research resources should take 2000 days to cure alone
    // So one day of full research = 0.05% progress (100/2000)
    return (research_power * 0.05);
}

void SetUpInvestment(World* world, Regions* world_regions) {
    if (world->disease_detected == 1) {
        // Initialize all regions' research investment when disease is first detected
        Regions* curr = world_regions;
        while (curr) {
            curr->research_investment = curr->development_level * 10;
            curr = curr->next_region;
        }
    }
}

void ApplyCure(Regions* region) {
    if (region->sick_people > 0) {
        // Calculate base cure amount (25% of sick people)
        long long percent_based_cure = (long long)(region->sick_people * 0.25); 
        // Ensure at least 1,000,000 people are cured if there are enough sick people
        long long min_cure = 1000000;
        long long cured;
        if (percent_based_cure > min_cure) {
            cured = percent_based_cure;
        }
        else {
            cured = min_cure;
        }
        // Don't cure more people than are actually sick
        if (cured > region->sick_people) {
            cured = region->sick_people;
        }
        region->sick_people -= cured;
        region->healthy_people += cured;
        if (cured > 0) {
            PrintColored("Distributing cure in ", GREEN);
            printf("%s: ", region->name);
            PrintColored("Cured ", GREEN);
            printf("%lld ", cured);
            PrintColored("people!\n", GREEN);
        }
    }
}

void Cure(Disease* disease, World* world, Regions* current_region) {
    if (world->vaccine_progress >= CURE_REACHED && !world->disease_cured) {
        // First time reaching cure - announce it once
        world->disease_cured = 1;
        disease->infectiousness = (int)(disease->infectiousness * 0.5);
        PrintColored("\n=== CURE DISCOVERED! Global vaccination campaign begins! ===\n\n", GREEN);
    }

    // Apply cure to current region only if cure is discovered
    if (world->disease_cured) {
        ApplyCure(current_region);
    }
}