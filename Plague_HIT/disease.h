#ifndef Disease_H
#define Disease_H

typedef struct {
    char name[50];
    int infectiousness;
    int severity;
    int lethality;
} Disease; 

void print_disease(const Disease* disease);

#endif
#pragma once
