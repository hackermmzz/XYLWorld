#include "forest.h"

ForestLand::ForestLand(int seed)
    :BiomeBase(seed,WaterAltitude+100,WaterAltitude-4)
{
}

pair<int,int> ForestLand::GetTree()
{
    static int treeFreq = 100;
    if (rand.rand(0, treeFreq) == 0)
    {
        float t=rand.rand(0, 1000)/1000.0;
        if(t>=0.9)return rand.rand(0,1)?pair<int,int>{StrippedCherryLog,CherryLogLeaves}:pair<int,int>{CherryLog,CherryLogLeaves};
        else if(t>=0.3)return rand.rand(0,1)?pair<int,int>{StrippedBirchLog,BirchLogLeaves}:pair<int,int>{BirchLog,BirchLogLeaves};
        else return rand.rand(0,1)?pair<int,int>{StrippedAcaciaLog,AcaciaLogLeaves}:pair<int,int>{AcaciaLog,AcaciaLogLeaves};
    }
    return pair<int,int>{0,0};
}

Block ForestLand::GetPlant()
{
    static int plantFreq = 50;
    if (rand.rand(0, plantFreq) == 0)
    {
        int val = rand.rand(0, 100);
        if (val >= 80)return { RedMushRoom };
        else if (val >= 60)return { Pumpkin,(BYTE)rand.rand(0,3)};
        else if (val >= 30)return { BrownMushRoom };
        else return { RoseBush };
    }
    return { 0 };
}

int ForestLand::GetSurface()
{
    return GRASS;
}

int ForestLand::GetBeneath()
{
    return DIRT;
}

int ForestLand::GetBeach()
{
    static int sandFreq = 100;
    int val = rand.rand(0, sandFreq);
    if (val >= 80)return DIRT;
    else if (val >= 1)return GRASS;
    return SAND;
}

void ForestLand::MakeTree(int WoodType,int LeavesType, int x, int y, int z)
{
    static int treeMinHeight = 5;
    static int treeMaxHeight = 20;
    int height = rand.rand(treeMinHeight, treeMaxHeight + 1);
    for (int i = 0; i < height; ++i)
    {
        SetBlock(x, y + i, z, WoodType, 2);
    }
    static int leavesMinWidth = 3;
    static int leavesMaxWidth = 4;
    int leavesWidth = rand.rand(leavesMinWidth, leavesMaxWidth + 1);
    int leavesHeight = rand.rand(1,4);
    ////////////////////////构建二次函数
    float fac = (-leavesWidth * 1.0 / leavesHeight / leavesHeight);
    for (int i = 0; i <leavesHeight; ++i) {
        int width = fac * (i+ leavesHeight) * (i - leavesHeight);
        for (int ii = -width; ii <= width; ++ii) {
            for (int iii = -width; iii <= width; ++iii)
            {
               SetBlock(x + ii, y + height - 1 + i, z + iii, LeavesType, 2);
               SetBlock(x + ii, y + height - 1 - i, z + iii, LeavesType, 2);
            }
        }
    }
}
