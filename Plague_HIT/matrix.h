#ifndef MATRIX_H
#define MATRIX_H

#include "world.h"

int FindBiggest(long long healthy, long long sick, long long dead);

char** CreateMatrix(struct World* world);

void FreeMatrix(char** matrix);
void PrintMatrix(World* world);

#endif

