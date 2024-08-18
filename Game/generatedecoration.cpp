#include "generatedecoration.h"


void GenerateDecoration(Chunk *chunk)
{
    ///////////////////////////////
    auto& heightMap = chunk->TerrainHeight;
    auto& pos = chunk->pos;
    auto& block = chunk->block;
    BiomeBase* biomeGenerator = GetBiomeGenerator(chunk);
    thread_local static  OceanLand oceanBiomeGenerator(0);//如果低于水平线使用这个生成器
    oceanBiomeGenerator.rand.SetSeed(pos.x * pos.y + pos.x + pos.y);
    for (int x = 0; x < SectionBlockWidth; ++x)
    {
        for (int z = 0; z < SectionBlockWidth; ++z)
        {
            int height = heightMap[x][z];
            auto blockType = block[x][height][z].id;
            //////////////////////////////判断是否可以种植植物
            if (!BlockIsPlantable(blockType))continue;
            //////////////////////////////
            int y = height + 1;
            Block decoration;
            if (height < WaterAltitude)
            {
                if (height == WaterAltitude-1)continue;
                decoration = oceanBiomeGenerator.GetPlant();
                if(decoration.id)
                {
                    oceanBiomeGenerator.MakePlant(decoration,x+pos.x,y,z+pos.y);
                }
            }
            else {
                ///////////////////////产生沙滩
                if (height==WaterAltitude)
                {
                    decoration.id =biomeGenerator->GetBeach();
                    decoration.meta = 2;
                    auto pos_ =vec3(x+pos.x,height,z+pos.y);
                    if (decoration.id!= GetBlock(pos_.x, pos_.y, pos_.z).id) {
                        if (GetBlock(pos_.x + 1, pos_.y, pos_.z).id == Water ||
                            GetBlock(pos_.x - 1, pos_.y, pos_.z).id == Water ||
                            GetBlock(pos_.x, pos_.y, pos_.z - 1).id == Water ||
                            GetBlock(pos_.x, pos_.y, pos_.z + 1).id == Water
                            )
                            block[x][height][z].id = decoration.id;
                    }
                }
                ///////////////////////产生tree
                auto&&tree=biomeGenerator->GetTree();
                if (tree.first)
                {
                    biomeGenerator->MakeTree(tree.first,tree.second, x + pos.x, y, z + pos.y);
                    continue;
                }
                decoration = biomeGenerator->GetPlant();
                if (decoration.id)
                {
                    biomeGenerator->MakePlant(decoration, x + pos.x, y, z + pos.y);
                }
            }
        }
    }
    //////////////////////////////////////////
    delete biomeGenerator;
}
