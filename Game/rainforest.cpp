#include "rainforest.h"
RainForest::RainForest(int seed)
    :BiomeBase(seed,WaterAltitude+70,WaterAltitude-10)
{
}

pair<int,int> RainForest::GetTree()
{
    static int treeFreq = 100;
    if (rand.rand(0, treeFreq)==0){
        float t=rand.rand(0, 1000)/1000.0;
        if(t>=0.9)return rand.rand(0,1)?pair<int,int>{StrippedCherryLog,CherryLogLeaves}:pair<int,int>{CherryLog,CherryLogLeaves};
        else if(t>=0.7)return rand.rand(0,1)?pair<int,int>{StrippedBirchLog,BirchLogLeaves}:pair<int,int>{BirchLog,BirchLogLeaves};
        else if(t>=0.6)return rand.rand(0,1)?pair<int,int>{StrippedAcaciaLog,AcaciaLogLeaves}:pair<int,int>{AcaciaLog,AcaciaLogLeaves};
        else return rand.rand(0,1)?pair<int,int>{StrippedJungleLog,JungleLogLeaves}:pair<int,int>{JungleLog,JungleLogLeaves};
    }
    return pair<int,int>{0,0};
}

Block RainForest::GetPlant()
{
    static int plantFreq =50;
    if (rand.rand(0, plantFreq)==0)
    {
        int val = rand.rand(0,100);
        if (val >= 80)return { WitherRose };
        else if (val >= 75)return { Pumpkin,(BYTE)rand.rand(0,3)};
        else if (val >= 70)return { RedMushRoom };
        else return { TallGrass };
    }
    return { 0 };
}

int RainForest::GetSurface()
{
    return GRASS;
}

int RainForest::GetBeach()
{
    static int sandFreq = 100;
    if (rand.rand(0, sandFreq) == 0)return SAND;
    return GRASS;
}

int RainForest::GetBeneath()
{
    return DIRT;
}
