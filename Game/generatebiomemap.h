#ifndef GENERATEBIOMEMAP_H
#define GENERATEBIOMEMAP_H
#include"globalsource.h"
#include"chunk.h"
///////////////////////////////////////////////////
pair<BiomeType,float> GenerateBiomeMap(vec2&position);
void ConfigBiomeMap(vector<float>& TemperatureLevelMap, vector<float>& HumidityLevelMap,vector<vector<BiomeType>>&BioTypeMap);
#endif // GENERATEBIOMEMAP_H
