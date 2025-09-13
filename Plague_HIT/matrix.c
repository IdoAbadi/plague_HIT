#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "world.h"

void create_matrix(World* world) {
	int rows = 3;
	int cols = FindBiggest(world);

	char** matrix = (char**)malloc(rows * sizeof(char*));
	if (matrix == NULL) {
		printf("Memory allocation failed for matrix rows\n");
		return;
	}

	for (int i = 0; i < rows; i++) {
		matrix[i] = (char*)malloc(cols * sizeof(char));
		if (matrix[i] == NULL) {
			printf("Memory allocation failed for row %d\n", i);
			return;
		}
	}
	for (int i = 0; i < rows; i++) {
		if (i == 0) {
			int temp = world->healthy_people / 100000000;
			for (int j = 0; j < temp; j++) {
				matrix[i][j] = "#";
			}
		}
		else if (i == 1) {
			int temp = world->sick_people / 100000000;
			for (int j = 0; j < temp; j++) {
				matrix[i][j] = "#";
			}
		}
		else {
			int temp = world->dead_people / 100000000;
			for (int j = 0; j < temp; j++) {
				matrix[i][j] = "#";
			}
		}
	}
}

int FindBiggest(World* world) {
	int temp1 = 0;
	if (temp1 < world->healthy_people) {
		temp1 = world->healthy_people / 100000000;
	}
	if (temp1 < world->sick_people / 100000000){
		temp1 = world->sick_people / 100000000;
	}
	if (temp1 < world->dead_people / 100000000) {
		temp1 = world->dead_people / 100000000;
	}
	return temp1;
}


void free_matrix(int** matrix, int rows) {
	for (int i = 0; i < rows; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void print_matrix(int** matrix) {
	int rows = 3;
	int cols = FindBiggest(matrix);
	for (int i = 0; i < rows; i++) {
		if (i == 0) {
			printf("Healthy People:  ");
			for (int j = 0; j < cols; j++) {
				if (matrix[i][j] == '#') {
					printf("%c", matrix[i][j]);
				}
				else {
					printf(" ");
				}
			}
		}
		else if (i == 1) {
			printf("Sick people:  ");
			for (int j = 0; j < cols; j++) {
				if (matrix[i][j] == '#') {
					printf("%c", matrix[i][j]);
				}
				else {
					printf(" ");
				}
			}
		}
		else {
			printf("Dead people:  ");
			for (int j = 0; j < cols; j++) {
				if (matrix[i][j] == '#') {
					printf("%c", matrix[i][j]);
				}
				else {
					printf(" ");
				}
			}
		}
		printf("\n");
	}
}
