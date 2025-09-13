#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "world.h"

void crete_matrix(int** matrix, World* world) {
	int rows = 3;
	int cols = 80;

	for (int i = 0; i < rows; i++) {
		matrix[i] = (int*)malloc(cols * sizeof(int));
		if (matrix[i] == NULL) {
			printf("Memory allocation failed for row %d\n", i);
			return;
		}
	}
}

void free_matrix(int** matrix, int rows) {
	for (int i = 0; i < rows; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void print_matrix(int** matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		if (i == 0) {
			printf("Healthy People: ");
		}
		else if (i == 1) {
			printf("Sick people: ");
		}
		else {
			printf("Dead people: ");
		}
		for (int j = 0; j < cols; j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}
