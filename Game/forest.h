#ifndef FOREST_H
#define FOREST_H
#include"BiomeBase.h"
struct ForestLand :BiomeBase
{
    ForestLand(int seed);
    // 通过 BiomeBase 继承
    pair<int,int> GetTree() override;
    Block GetPlant() override;
    int GetSurface() override;
    int GetBeneath() override;
    int GetBeach() override;
    virtual void MakeTree(int WoodType,int LeavesType, int x, int y, int z);
};
#endif // FOREST_H
