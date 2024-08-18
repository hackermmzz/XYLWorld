#include "block.h"

bool BlockIsPlantable(int type)
{
    switch (type)
    {
    case DIRT:
    case SAND:
    case GRASS:
    case SNOW:
        return 1;
    }
    return 0;
}

