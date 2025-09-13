#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>

typedef struct {
	int (*rows)[3];
	size_t count;
	size_t capacity;
} WorldMatrix;

WorldMatrix* create_world_matrix(size_t initial_capacity);
void free_world_matrix(WorldMatrix* matrix);
int world_matrix_append_from_totals(WorldMatrix* world_matrix, long long healthy_total, long long sick_total, long long dead_total);
int world_matrix_append_percent(WorldMatrix* world_matrix, int healthy, int sick, int dead);
void print_world_matrix(const WorldMatrix* world_matrix, size_t last_n, int bar_width);

#endif
#pragma once