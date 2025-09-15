#ifndef MATRIX_H
#define MATRIX_H

#include "world.h"

int FindBiggest(long long healthy, long long sick, long long dead);

void CreateMatrix(struct World* world);
void FreeMatrix(int** matrix, int rows);
void PrintMatrix(int** matrix, World* world);

#endif

