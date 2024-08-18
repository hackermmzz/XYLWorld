#ifndef OCEAN_H
#define OCEAN_H

#include"BiomeBase.h"
struct OceanLand :BiomeBase {
    OceanLand(int seed);

    // 通过 BiomeBase 继承
    pair<int,int> GetTree() override;
    Block GetPlant() override;
    int GetSurface() override;
    int GetBeneath() override;
    int GetBeach() override;
};

#endif // OCEAN_H
