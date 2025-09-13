#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

WorldMatrix* create_world_matrix(size_t initial_capacity){
	WorldMatrix* world_matrix = (WorldMatrix*)calloc(1, sizeof(WorldMatrix));
	if (initial_capacity > 0) {
		world_matrix->rows = (int(*)[3])calloc(initial_capacity, sizeof *world_matrix->rows);
		world_matrix->capacity = initial_capacity;
	}
	world_matrix->count = 0;
	return world_matrix;
}

void free_world_matrix(WorldMatrix* world_matrix) {
	free(world_matrix->rows);
	free(world_matrix);
}

int world_matrix_append_from_totals(WorldMatrix* world_matrix, long long healthy_total, long long sick_total, long long dead_total) {
	if (world_matrix->count >= world_matrix->capacity) {
		size_t new_capacity;
		if (world_matrix->capacity == 0) {
			new_capacity = 8;
		}
		else {
			new_capacity = world_matrix->capacity * 2;
		}
		void* ptr = realloc(world_matrix->rows, new_capacity * sizeof * world_matrix->rows);
		world_matrix->rows = (int(*)[3])ptr;
		world_matrix->capacity = new_capacity;
	}
	long long total = healthy_total + sick_total + dead_total;
	int healthy = 0, sick = 0, dead = 0;
	if (total > 0) {
		healthy = (int)((healthy_total * 100) / (double)total);
		sick = (int)((sick_total * 100) / (double)total);
		dead = (int)((dead_total * 100) / (double)total);
	}
	world_matrix->rows[world_matrix->count][0] = healthy;
	world_matrix->rows[world_matrix->count][1] = sick;
	world_matrix->rows[world_matrix->count][2] = dead;
	world_matrix->count++;
	return 0;
}

int world_matrix_append_percent(WorldMatrix* world_matrix, int healthy, int sick, int dead) {
	if (world_matrix->count >= world_matrix->capacity) {
		size_t new_capacity;
		if (world_matrix->capacity == 0) {
			new_capacity = 8;
		}
		else {
			new_capacity = world_matrix->capacity * 2;
		}
		void* ptr = realloc(world_matrix->rows, new_capacity * sizeof * world_matrix->rows);
		world_matrix->rows = (int(*)[3])ptr;
		world_matrix->capacity = new_capacity;
	}
	world_matrix->rows[world_matrix->count][0] = healthy;
	world_matrix->rows[world_matrix->count][1] = sick;
	world_matrix->rows[world_matrix->count][2] = dead;
	world_matrix->count++;
	return 0;
}

void print_world_matrix(const WorldMatrix* world_matrix, size_t last_n, int bar_width) {
	size_t start = 0;
	if (last_n > 0 && last_n < world_matrix->count) {
		start = world_matrix->count - last_n;
	}
	for (size_t day = start; day < world_matrix->count; day++) {
		printf("Day %zu:\n ", day + 1);
		int lenHealthy = (world_matrix->rows[day][0] * bar_width) / 100;
		printf("Healthy: %3d%% ", world_matrix->rows[day][0]);
		for (int i = 0; i < lenHealthy; i++) {
			putchar('#');
		}
		for (int i = lenHealthy; i < bar_width; i++) {
			putchar(' ');
		}
		puts("");
		int lenSick = (world_matrix->rows[day][1] * bar_width) / 100;
		printf("Sick:   %3d%% ", world_matrix->rows[day][1]);
		for (int i = 0; i < lenSick; i++) {
			putchar('#');
		}
		for (int i = lenSick; i < bar_width; i++) {
			putchar(' ');
		}
		puts("");
		int lenDead = (world_matrix->rows[day][2] * bar_width) / 100;
		printf("Dead:   %3d%% ", world_matrix->rows[day][2]);
		for (int i = 0; i < lenDead; i++) {
			putchar('#');
		}
		for (int i = lenDead; i < bar_width; i++) {
			putchar(' ');
		}
		puts("");
		puts("");
	}
}