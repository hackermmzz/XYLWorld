#ifndef GRASSLAND_H
#define GRASSLAND_H
#include"biomebase.h"
struct GrassLand :BiomeBase {
    // 通过 BiomeBase 继承
    GrassLand(int seed);
    // 通过 BiomeBase 继承
    pair<int,int> GetTree() override;

    Block GetPlant() override;

    int GetSurface() override;

    int GetBeach() override;


    // 通过 BiomeBase 继承
    int GetBeneath() override;

};
#endif // GRASSLAND_H
