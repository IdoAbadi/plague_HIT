#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "world.h"
#include "design.h"

#define ROWS 3

int FindBiggest(long long healthy, long long sick, long long dead) {
	int temp1 = 0;
	if (temp1 < healthy) {
		temp1 = (int)(healthy / 100000000);
	}
	if (temp1 < sick / 100000000) {
		temp1 = (int)(sick / 100000000);
	}
	if (temp1 < dead / 100000000) {
		temp1 = (int)(dead / 100000000);
	}
	return temp1;
}

// Return type changed to char** so caller can use the matrix
char** CreateMatrix(struct World* world) {
    int cols = FindBiggest(world->healthy_people, world->sick_people, world->dead_people);

    char** matrix = (char**)malloc(ROWS * sizeof(char*));
    if (matrix == NULL) {
        printf("Memory allocation failed for matrix rows\n");
        return NULL;
    }

    // Allocate and initialize rows
    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (char*)malloc(cols * sizeof(char));
        if (matrix[i] == NULL) {
            // Free previously allocated memory on failure
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            printf("Memory allocation failed for row %d\n", i);
            return NULL;
        }
        // Initialize row to spaces
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = ' ';
        }
    }

    // Fill matrix with population data
    for (int i = 0; i < ROWS; i++) {
        int temp;
        if (i == 0) {
            temp = FindBiggest(world->healthy_people, 0, 0);
        }
        else if (i == 1) {
            temp = FindBiggest(0, world->sick_people, 0);
        }
        else {
            temp = FindBiggest(0, 0, world->dead_people);
        }
        for (int j = 0; j < temp && j < cols; j++) {
            matrix[i][j] = '#';
        }
    }

    return matrix;
}

// Update FreeMatrix to match matrix type
void FreeMatrix(char** matrix) {
    if (matrix) {
        for (int i = 0; i < ROWS; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

// Update PrintMatrix to match matrix type
void PrintMatrix(World* world) {
    char** matrix = CreateMatrix(world);
    if (!matrix) {
        return;
    }
    int cols = FindBiggest(world->healthy_people, world->sick_people, world->dead_people);
    for (int i = 0; i < ROWS; i++) {
        if (i == 0) {
            PrintColored("Healthy people:", GREEN);
        }
        else if (i == 1) {
            PrintColored("Sick people:", YELLOW);
        }
        else {
            PrintColored("Dead people:", RED);
        }
        for (int j = 0; j < cols; j++) {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}
