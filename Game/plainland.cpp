#include "plainland.h"

PlainLand::PlainLand(int seed)
    :BiomeBase(seed,WaterAltitude+5,WaterAltitude-5)
{
}

pair<int,int> PlainLand::GetTree()
{
    return pair<int,int>{0,0};
}

Block PlainLand::GetPlant()
{
    static int plantFreq = 100;
    if (rand.rand(0, plantFreq) == 0)
    {
        int val = rand.rand(0, 100);
        if (val >= 70)return { TallGrass };
        else if (val >= 50)return { WitherRose };
        else return { BrownMushRoom };
    }
    return {0};
}

int PlainLand::GetSurface()
{
    return GRASS;
}

int PlainLand::GetBeach()
{
    static int sandFreq = 10;
    if (rand.rand(0, sandFreq))return SAND;
    return GRASS;
}

int PlainLand::GetBeneath()
{
    return DIRT;
}
