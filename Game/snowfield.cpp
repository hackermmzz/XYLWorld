#include "snowfield.h"
SnowFieldLand::SnowFieldLand(int seed)
    :BiomeBase(seed,WaterAltitude+60,WaterAltitude-5)
{
}

pair<int,int> SnowFieldLand::GetTree()
{
    static int treeFreq = 400;
    if (rand.rand(0, treeFreq) == 0){
        float t=rand.rand(0, 1000)/1000.0;
        if(t>=0.8)return rand.rand(0,1)?pair<int,int>{StrippedBirchLog,BirchLogLeaves}:pair<int,int>{BirchLog,BirchLogLeaves};
        else return rand.rand(0,1)?pair<int,int>{StrippedAcaciaLog,AcaciaLogLeaves}:pair<int,int>{AcaciaLog,AcaciaLogLeaves};
    }
    return pair<int,int>{0,0};
}

Block SnowFieldLand::GetPlant()
{
    static int plantFreq = 100;
    if (rand.rand(0, plantFreq) == 0)
    {
        int val = rand.rand(0, 10);
        if (val >= 2)return { BrownMushRoom };
        return { RedMushRoom };
    }
    return { 0 };
}

int SnowFieldLand::GetSurface()
{
    static int snowFreq = 100;
    int val = rand.rand(0, snowFreq );
    if (val == 0)return DIRT;
    else if (val == 1)return SAND;
    return SNOW;
}

int SnowFieldLand::GetBeneath()
{
    return DIRT;
}

int SnowFieldLand::GetBeach()
{
    static int sandFreq = 10;
    if (rand.rand(0, sandFreq) == 0)return SAND;
    return SNOW;
}

void SnowFieldLand::MakeTree(int WoodType,int LeavesType, int x, int y, int z)
{
    static int treeMaxHeight = 6;
    static int treeMinHeight = 4;
    int height = rand.rand(treeMinHeight, treeMaxHeight + 1);
    //生成主干
    for (int i = 0; i < height; ++i)
    {
        SetBlock(x, y + i, z, WoodType, 2);
    }
    //生成树叶
    static int leavesMinWidth = 1;
    static int leavesMaxWidth = 3;
    int leavesWidth = rand.rand(leavesMinWidth, leavesMaxWidth + 1);
    int leavesHeight = rand.rand(1, 4);
    float fac = (-leavesWidth * 1.0 / leavesHeight / leavesHeight);
    for (int i = 0; i < leavesHeight; ++i) {
        int width = fac * (i + leavesHeight) * (i - leavesHeight);
        for (int ii = -width; ii <= width; ++ii) {
            for (int iii = -width; iii <= width; ++iii)
            {

                SetBlock(x + ii, y + height - 1 + i, z + iii, LeavesType,2);
                SetBlock(x + ii, y + height - 1 - i, z + iii, LeavesType,2);
            }
        }
    }
}

