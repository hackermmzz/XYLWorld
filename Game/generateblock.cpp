#include "generateblock.h"
void GenerateBlock(Chunk*chunk)
{
    /////////////////////////////////
    auto biome = chunk->BiomeId;
    auto*data = chunk->block;
    auto*TerrainHeight=chunk->TerrainHeight;
    auto& pos = chunk->pos;
    static int MAX_HEIGHT = SectionBlockWidth * ChunkSectionWidth - 1;
    //////////////////////////////根据对应的生物群系获得对应的生物群系生成器
    BiomeBase* BiomeGenerator = GetBiomeGenerator(chunk);
    static  OceanLand oceanBiomeGenerator(0);//如果低于水平线使用这个生成器
    oceanBiomeGenerator.rand.SetSeed(pos.x * pos.y + pos.x + pos.y);
    //////////////////////////////
    vec3 BlockPos;
    auto MapEnd=BlockChanged.end();
    for (int x = 0; x < SectionBlockWidth; ++x)
    {
        BlockPos.x = x + pos.x;
        for (int z = 0; z < SectionBlockWidth; ++z)
        {
            BlockPos.z = z + pos.y;
            auto height =TerrainHeight[x][z];
            if (height > MAX_HEIGHT)height = MAX_HEIGHT;
            for (int y = 0; y <=MAX_HEIGHT; ++y)
            {
                auto& block=data[x][y][z];
                block.meta = 2;//正面的朝向默认为2
                auto& blockID = block.id;
                ///////////////////////////////////////
                auto itr=BlockChanged.find(vec3(BlockPos.x,y, BlockPos.z));
                if (itr != MapEnd)
                {
                 block.id=itr->second;
                 continue;
                }
                else if(y>height)
                    continue;
                blockID=randomInt(1,10);
            ///////////////////////////////////////
                if (y <= BedRockDeep)//生成基岩
                    blockID = BedRock;
                else if (y == height && height >=WaterAltitude)blockID = BiomeGenerator->GetSurface();
                else if (y > height - 4)blockID = BiomeGenerator->GetBeneath();
                else blockID = STONE;
            }
            ////////////////////////////////////////////生成水和河床
            if (height < WaterAltitude)data[x][height][z].id =oceanBiomeGenerator.GetSurface();
            for (int y = height+1; y <= WaterAltitude; ++y)
            {
                auto& blockID = data[x][y][z].id;
                blockID = Water;
            }
        }
    }
   ///////////////////////////////////////////释放生物群系产生器
    delete BiomeGenerator;
}

BiomeBase *GetBiomeGenerator(Chunk *chunk)
{
    auto& biome = chunk->BiomeId;
    auto& pos = chunk->pos;
    int seed = pos.x * pos.y + pos.x + pos.y;
    if (biome == GRASSLAND)return new GrassLand(seed);
    else if (biome == PLAIN)return new PlainLand(seed);
    else if (biome == RAINFOREST)return new RainForest(seed);
    else if (biome == DESERT)return new DesertLand(seed);
    else if (biome == FOREST)return new ForestLand(seed);
    else if (biome == SNOWFIELD)return new SnowFieldLand(seed);
    else if (biome == OCEAN)return new OceanLand(seed);
    return 0;
}
