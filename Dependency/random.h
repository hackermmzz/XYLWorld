#ifndef RANDOM_H
#define RANDOM_H
#include"include.h"
#include"chrono"
int randomInt(int a,int b);
float randomFloat();
float randomFloat(float a,float b);
struct Random {
    std::uniform_int_distribution<int> randomInt;
    std::default_random_engine generator;
    Random();
    void SetSeed(int seed);
    int rand(int begin,int end);
};
#endif // RANDOM_H
