#include "desert.h"

DesertLand::DesertLand(int seed)
    :BiomeBase(seed,WaterAltitude+20,WaterAltitude+1)
{
}

pair<int,int> DesertLand::GetTree()
{
    static int treeFreq = 1000;
    if (rand.rand(0, treeFreq) == 0)return pair<int,int>{1,1};
    return pair<int,int>{0,0};
}

Block DesertLand::GetPlant()
{
    static int plantFreq = 1000;
    if (rand.rand(0, plantFreq) == 0)return {CactusBush};
    return { 0 };
}

int DesertLand::GetSurface()
{
    return SAND;
}

int DesertLand::GetBeneath()
{
    return SAND;
}

int DesertLand::GetBeach()
{
    static int sandFreq = 10;
    if (rand.rand(0, sandFreq) == 0)return SAND;
    return GRASS;
}

void DesertLand::MakeTree(int WoodType, int LeavesType, int x, int y, int z)
{
    static int minHeight = 4;
    static int maxHeight = 8;
    int height = rand.rand(minHeight, maxHeight + 1);
    for (int i = 0; i < height; ++i)
       SetBlock(x, y + i, z, Cactus, 2);
}
