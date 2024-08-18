#ifndef BLOCK_H
#define BLOCK_H
#include"globalsource.h"
////////////////////////////////////////////
enum{
    EMPTY,
    BedRock,
    SAND,
    STONE,
    GRASS,
    BRICK,
    StrippedCherryLog,
    CherryLog,//樱桃
    CherryLogLeaves,
    StrippedJungleLog,//雨林树
    JungleLog,
    JungleLogLeaves,
    StrippedAcaciaLog,
    AcaciaLog,//相思树
    AcaciaLogLeaves,
    StrippedBirchLog,//桦树
    BirchLog,
    BirchLogLeaves,
    SNOW,
    DIRT,
    GLASS,
    CobbleLess,
    CobbleMore,
    WitherRose,
    RoseBush,
    LilyPadFlowerPink,
    LilyPadFlowerYellow,
    TorchFlower,
    RedMushRoom,
    BrownMushRoom,
    Pumpkin,
    Cactus,
    CactusBush,
    TNT,
    WorkSpace,
    TallGrass,
    Water,
};
////////////////////////////////////////////
using BYTE=unsigned char;
struct  Block
{
    BYTE id;
    BYTE meta;
};
bool BlockIsPlantable(int type);
#endif // BLOCK_H
