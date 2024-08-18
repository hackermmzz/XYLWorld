#include "generatebiomemap.h"

pair<BiomeType, float> GenerateBiomeMap(glm::vec2 &pos)
{
    /////////////////////////////////////////////温湿度等级对照表
     static vector<float>TemperatureLevelMap;
     static vector<float>HumidityLevelMap;
     static vector<vector<BiomeType>>BiomeTypeMap;
     static bool init = 0;
    static const uint FBM = 4;
    if (!init)
    {
        init = 1;
        ConfigBiomeMap(TemperatureLevelMap, HumidityLevelMap, BiomeTypeMap);
    }
    BiomeType biome;
    /////////////////////////////////////////////先判断为陆地还是海洋
    float Continental = ContinentalNoise.generate01(pos.x / ContinentalNoiseFreq, pos.y / ContinentalNoiseFreq, FBM);
    if (Continental < OceanWeight) {
        biome = OCEAN;
    }
    else {
        /////////////////////////////////////////////产生温度和湿度
        float temperature = TemperatureNoise.generate11(pos.x / TemperatureNoiseFreq, pos.y / TemperatureNoiseFreq, FBM);
        float humidity = HumidityNoise.generate11(pos.x / HumidityNoiseFreq, pos.y / HumidityNoiseFreq,FBM);
        //获得温度和湿度等级
        int temperatureLevel = upper_bound(TemperatureLevelMap.begin(), TemperatureLevelMap.end(), temperature) - TemperatureLevelMap.begin();
        int humidityLevel = upper_bound(HumidityLevelMap.begin(), HumidityLevelMap.end(), humidity) - HumidityLevelMap.begin();
        biome=BiomeTypeMap[temperatureLevel][humidityLevel];
    }
    return {biome,Continental};
}

void ConfigBiomeMap(vector<float> &TemperatureLevelMap, vector<float> &HumidityLevelMap, vector<vector<BiomeType> > &BiomeTypeMap)
{
    TemperatureLevelMap = {-0.6,-0.3,0.0,0.5,1.1};
    HumidityLevelMap = { -0.8,-0.5,0.0,0.5,1.1 };
    BiomeTypeMap = vector<vector<BiomeType>>(TemperatureLevelMap.size(),vector<BiomeType>(HumidityLevelMap.size()));
    ////////////////////////////////
    BiomeTypeMap[0][0] = SNOWFIELD;
    BiomeTypeMap[0][1] = SNOWFIELD;
    BiomeTypeMap[0][2] = SNOWFIELD;
    BiomeTypeMap[0][3] = SNOWFIELD;
    BiomeTypeMap[0][4] = SNOWFIELD;
    ////
    BiomeTypeMap[1][0] = PLAIN;
    BiomeTypeMap[1][1] =PLAIN;
    BiomeTypeMap[1][2] = GRASSLAND;
    BiomeTypeMap[1][3] = GRASSLAND;
    BiomeTypeMap[1][4] = GRASSLAND;
    ////
    BiomeTypeMap[2][0] = PLAIN;
    BiomeTypeMap[2][1] = PLAIN;
    BiomeTypeMap[2][2] = FOREST;
    BiomeTypeMap[2][3] = FOREST;
    BiomeTypeMap[2][4] = FOREST;
    ////
    BiomeTypeMap[3][0] = DESERT;
    BiomeTypeMap[3][1] = DESERT;
    BiomeTypeMap[3][2] = DESERT;
    BiomeTypeMap[3][3] = RAINFOREST;
    BiomeTypeMap[3][4] = RAINFOREST;
    ///
    BiomeTypeMap[4][0] = DESERT;
    BiomeTypeMap[4][1] = DESERT;
    BiomeTypeMap[4][2] = DESERT;
    BiomeTypeMap[4][3] = RAINFOREST;
    BiomeTypeMap[4][4] = RAINFOREST;
}
