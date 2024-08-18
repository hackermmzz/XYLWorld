#ifndef PERLINNOISE_H
#define PERLINNOISE_H
#include"iostream"
#include"header.h"
#include"set"
#include"random.h"
using namespace std;
//随机索引表(一定要是512大小的，索引值0-255)
extern const int DefaultPermutationTable[];
double Fade(double t);
double Lerp(double a,double b,double f);
double ClampTo11(double r);
double PerlinNoise3D(int*permutation,double x,double y,double z);
double PerlinNoise3D(int*permutation,double x,double y,double z,uint octaves,double persistence=0.5);
double PerlinNoise2D(int*permutation,double x,double y);
double PerlinNoise2D(int*permutation,double x,double y,uint octaves,double persistence=0.5);
///////////////////////////////
struct PerlinNoiseGenerator{
    vector<int>permutationTable;
    ////////////////////////////////////////////
    PerlinNoiseGenerator(int seed);
    double generate01(double x,double y,double z);
    double generate01(double x,double y);
    double generate01(double x,double y,uint octaves,double persistence=0.5);
    double generate01(double x,double y,double z,uint octaves,double persistence=0.5);
    double generate11(double x,double y,double z);
    double generate11(double x,double y);
    double generate11(double x,double y,uint octaves,double persistence=0.5);
    double generate11(double x,double y,double z,uint octaves,double persistence=0.5);
};
#endif // PERLINNOISE_H
