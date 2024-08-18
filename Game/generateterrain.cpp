#include "generateterrain.h"

void GenerateTerrain(Chunk *chunk)
{

    ////////////////////////////////////
    static  vector<TerrainInfo>ContinentalLevel;
    static  vector<TerrainInfo>ErosionLevel;
    static  vector<TerrainInfo>PeakLevel;
    static thread_local  bool init = 0;
    static thread_local  float ErosionData[SectionBlockWidth][SectionBlockWidth];
    static thread_local  float PeakData[SectionBlockWidth][SectionBlockWidth];
    static const uint FBM=4;
    if (!init)
    {
        init = 1;
        ConfigTerrain(ContinentalLevel, ErosionLevel, PeakLevel);
    }
    //////////////////////////////////////
    memset(chunk->block,0,sizeof(chunk->block));
    auto&TerrainHeight=chunk->TerrainHeight;
    vec2&pos=chunk->pos;
    static auto Lerp=[&](vector<TerrainInfo>&v,float val)->float{
        static auto CMP=[&](const TerrainInfo&t0,float val)->bool
        {
            return t0.first<val;
        };
        auto itr1=lower_bound(v.begin(),v.end(),val,CMP);
        auto itr0=prev(itr1);
        auto f=(val-itr0->first)/(itr1->first-itr0->first);
        return  f*itr1->second+(1.0-f)*itr0->second;
    };
    auto&&ret=GenerateBiomeMap(pos);
    chunk->BiomeId=ret.first;
    float ContinentalHeight=Lerp(ContinentalLevel,ret.second)*ContinentalNoiseWeight;
    //////生成噪声值
    for(int xo=0;xo<SectionBlockWidth;++xo)
    {
        for(int zo=0;zo<SectionBlockWidth;++zo)
        {
            double x=pos.x+xo,z=pos.y+zo;
            ErosionData[xo][zo]=ErosionNoise.generate01(x/ErosionNoiseFreq,z/ErosionNoiseFreq,FBM);
            PeakData[xo][zo]=PeaksNoise.generate01(x/PeakNoiseFreq,z/PeakNoiseFreq,FBM);
        }
    }
    /////获取对应的高度
    for(int xo=0;xo<SectionBlockWidth;++xo)
    {
        for(int zo=0;zo<SectionBlockWidth;++zo)
        {
            ErosionData[xo][zo]=Lerp(ErosionLevel,ErosionData[xo][zo])*ErosionNoiseWeight;
            PeakData[xo][zo]=Lerp(PeakLevel,PeakData[xo][zo])*PeakNoiseWeight;
        }
    }
    ///////生成地形的高度
    for(int xo=0;xo<SectionBlockWidth;++xo)
    {
        for(int zo=0;zo<SectionBlockWidth;++zo)
        {
            TerrainHeight[xo][zo]=PeakData[xo][zo]+ContinentalHeight-ErosionData[xo][zo];
        }
    }
}

void ConfigTerrain(vector<TerrainInfo> &Continental, vector<TerrainInfo> &Erosion, vector<TerrainInfo> &Peak)
{
    static const uint TerrianMaxHeight=SectionBlockWidth*ChunkSectionWidth-1;
    static const uint minHeight=WaterAltitude*4/5;
    static const uint maxHeight=TerrianMaxHeight*3/4;
    static const uint maxErosion=minHeight/2;
    static const float bonus=std::numeric_limits<float>::epsilon();
    ///////////////////////////
    float dis0=1.0-OceanWeight;
    Continental={
        {-bonus,minHeight},
        {OceanWeight,WaterAltitude},
        {OceanWeight+dis0*0.5,WaterAltitude*7/6},
        {OceanWeight+dis0*0.8,WaterAltitude*3/2},
        {1.0+bonus,WaterAltitude*5/2}
    };
    ///////////////////////////
    Erosion={
        {-bonus,0},
        {0.5,5},
        {0.7,maxErosion/2},
        {0.8,maxErosion*3/4},
        {0.9,maxErosion*9/10},
        {1.0+bonus,maxErosion}
    };
    ///////////////////////////
    Peak={
        {-bonus,0},
        {0.4,maxHeight*1/5},
        {0.6,maxHeight*2/5},
        {0.8,maxHeight*1/2},
        {1.0+bonus,maxHeight}
    };
}
