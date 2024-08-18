#ifndef GENERATEBLOCK_H
#define GENERATEBLOCK_H
#include"block.h"
#include"chunk.h"
#include"biomebase.h"
#include"grassland.h"
#include"desert.h"
#include"plainland.h"
#include"snowfield.h"
#include"rainforest.h"
#include"forest.h"
#include"ocean.h"
void GenerateBlock(Chunk*chunk);
BiomeBase* GetBiomeGenerator(Chunk* chunk);

#endif // GENERATEBLOCK_H
