#include "grassland.h"
GrassLand::GrassLand(int seed)
    :BiomeBase(seed,WaterAltitude+40,WaterAltitude-5)
{
}

pair<int,int> GrassLand::GetTree()
{
    static int Treefreq = 500;
    if (rand.rand(0, Treefreq) == 0)
    {
        float t=rand.rand(0, 1000)/1000.0;
        if(t>=0.8)return rand.rand(0,1)?pair<int,int>{StrippedCherryLog,CherryLogLeaves}:pair<int,int>{CherryLog,CherryLogLeaves};
        else if(t>=0.3)return rand.rand(0,1)?pair<int,int>{StrippedBirchLog,BirchLogLeaves}:pair<int,int>{BirchLog,BirchLogLeaves};
        else return rand.rand(0,1)?pair<int,int>{StrippedAcaciaLog,AcaciaLogLeaves}:pair<int,int>{AcaciaLog,AcaciaLogLeaves};
    }
    return pair<int,int>{0,0};
}

Block GrassLand::GetPlant()
{
    static int plantFreq = 100;
    if (rand.rand(0, plantFreq) == 0)
    {
        return rand.rand(0, 2) ? Block{WitherRose} : Block{RoseBush};
    }
    return { 0 };
}

int GrassLand::GetSurface()
{
    static int grassBlockFreq = 10;
    if (rand.rand(0, grassBlockFreq))
    {
        return GRASS;
    }
    return DIRT;
}

int GrassLand::GetBeach()
{
    static int sandFreq = 10;
    if (rand.rand(0, sandFreq))return SAND;
    return rand.rand(0, 2) ? DIRT : GRASS;
}

int GrassLand::GetBeneath()
{
    return DIRT;
}
