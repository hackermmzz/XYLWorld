#ifndef PLAINLAND_H
#define PLAINLAND_H
#include"biomebase.h"
struct PlainLand :BiomeBase {
    // 通过 BiomeBase 继承
    PlainLand(int seed);
    pair<int,int> GetTree() override;
    Block GetPlant() override;
    int GetSurface() override;
    int GetBeach() override;

    // 通过 BiomeBase 继承
    int GetBeneath() override;
};
#endif // PLAINLAND_H
