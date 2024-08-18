#ifndef SNOWFIELD_H
#define SNOWFIELD_H
#include"BiomeBase.h"
struct SnowFieldLand :BiomeBase {
    SnowFieldLand(int seed);
    // 通过 BiomeBase 继承
    pair<int,int> GetTree() override;
    Block GetPlant() override;
    int GetSurface() override;
    int GetBeneath() override;
    int GetBeach() override;
    void MakeTree(int WoodType,int LeavesType,int x, int y, int z);
};
#endif // SNOWFIELD_H
