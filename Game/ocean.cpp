#include "ocean.h"
OceanLand::OceanLand(int seed)
    :BiomeBase(seed,WaterAltitude-3,WaterAltitude-50)
{
}

pair<int, int> OceanLand::GetTree()
{
    return pair<int,int>{0,0};
}

Block OceanLand::GetPlant()
{
    static int plantFreq = 300;
    if (rand.rand(0, plantFreq) == 0)
    {
        return rand.rand(0,1)?Block{RedMushRoom}:Block{BrownMushRoom};
    }
    return Block{ 0 };
}

int OceanLand::GetSurface()
{
    static int stoneFreq = 10;
    if (rand.rand(0, stoneFreq) == 0)return STONE;
    return DIRT;
}

int OceanLand::GetBeneath()
{
    static int stoneFreq = 10;
    if (rand.rand(0, stoneFreq) == 0)return STONE;
    return DIRT;
}

int OceanLand::GetBeach()
{
    return 0;
}
