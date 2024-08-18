#ifndef GENERATETERRAIN_H
#define GENERATETERRAIN_H
#include"block.h"
#include"chunk.h"
#include"generatebiomemap.h"
using TerrainInfo=pair<float,int>;
void GenerateTerrain(Chunk*chunk);
void ConfigTerrain(vector<TerrainInfo>& Continental, vector<TerrainInfo>& Erosion, vector<TerrainInfo>& Peak);
#endif // GENERATETERRAIN_H
