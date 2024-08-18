#ifndef RAINFOREST_H
#define RAINFOREST_H

#include"BiomeBase.h"
struct RainForest :BiomeBase
{
    RainForest(int seed);
    // 通过 BiomeBase 继承
    pair<int,int> GetTree() override;
    Block GetPlant() override;
    int GetSurface() override;
    int GetBeach() override;

    // 通过 BiomeBase 继承
    int GetBeneath() override;
};
#endif // RAINFOREST_H
