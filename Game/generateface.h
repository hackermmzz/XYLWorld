#ifndef GENERATEFACE_H
#define GENERATEFACE_H
#include"chunk.h"
#include"section.h"
using ChunkNearChunks=tuple<Chunk *, Chunk *, Chunk *, Chunk *>;
using FaceBufferType=decltype(Section::face);
void GenerateFace(Section*section,ChunkNearChunks cnc);
ChunkNearChunks GetChunkNearChunks(Chunk*chunk);
void MakeWaterFace(FaceBufferType&buffer,const vec3&pos);
void MakeCubeFace(FaceBufferType&buffer,Block&block,const vec3&pos,Block*NearBlocks);
void MakePlantFace(FaceBufferType&buffer,Block&block,const vec3&pos);
#endif // GENERATEFACE_H
