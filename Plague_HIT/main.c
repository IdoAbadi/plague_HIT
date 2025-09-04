#include<stdio.h>
#include "disease.h"
#include "world.h"

void main() {
    Disease disease = { "LirIdo", 80, 40, 20 }; 
    Region  region = { 1000000, 100, 0, 50, 60, 70 };

    print_disease(&disease);
    print_region(&region);

}