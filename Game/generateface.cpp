#include "generateface.h"

void GenerateFace(Section *section,ChunkNearChunks cnc)
{
    /////////////////////////////
    auto&block=section->Chunk->block;
    auto&buffer0=section->face;
    auto&buffer1=section->waterFace;
    buffer0.clear();
    buffer1.clear();
    //////////////////////////产生block的face
    auto&pos=section->position;
    auto&blocks=section->Chunk->block;
    Chunk*front=get<0>(cnc),*back=get<1>(cnc),*left=get<2>(cnc),*right=get<3>(cnc),*cur=section->Chunk;
    auto GetTarChunkBlock=[&](Chunk*chunk,const vec3&RelativePos)->Block&{
        return chunk->block[int(RelativePos.x)][int(RelativePos.y)][int(RelativePos.z)];
    };
    auto GetTargetPosAroundAllBlock=[&](const vec3&RelativePos)->Block*{
      static const int maxHeight=SectionBlockWidth*ChunkSectionWidth-1;
      thread_local static Block res[6];//分别对应上下前后左右
      //上
      if(RelativePos.y<maxHeight)
          res[0]=GetTarChunkBlock(cur,vec3{RelativePos.x,RelativePos.y+1,RelativePos.z});
      else  res[0].id=0;
      //下
      if(RelativePos.y)
          res[1]=GetTarChunkBlock(cur,vec3{RelativePos.x,RelativePos.y-1,RelativePos.z});
      else res[1].id=0;
      //前
      if(RelativePos.z<(SectionBlockWidth-1))
            res[2]=GetTarChunkBlock(cur,vec3{RelativePos.x,RelativePos.y,RelativePos.z+1});
      else
            res[2]=GetTarChunkBlock(front,vec3{RelativePos.x,RelativePos.y,0});
      //后
      if(RelativePos.z)
            res[3]=GetTarChunkBlock(cur,vec3{RelativePos.x,RelativePos.y,RelativePos.z-1});
      else
            res[3]=GetTarChunkBlock(back,vec3{RelativePos.x,RelativePos.y,(SectionBlockWidth-1)});
      //左
      if(RelativePos.x)
            res[4]=GetTarChunkBlock(cur,vec3{RelativePos.x-1,RelativePos.y,RelativePos.z});
      else
            res[4]=GetTarChunkBlock(left,vec3{(SectionBlockWidth-1),RelativePos.y,RelativePos.z});
      //右
      if(RelativePos.x<(SectionBlockWidth-1))
            res[5]=GetTarChunkBlock(cur,vec3{RelativePos.x+1,RelativePos.y,RelativePos.z});
      else
            res[5]=GetTarChunkBlock(right,vec3{0,RelativePos.y,RelativePos.z});
      return res;
    };
    for(int x=0;x<SectionBlockWidth;++x)
    {
        for(int z=0;z<SectionBlockWidth;++z)
        {
            for(int y=pos.y+SectionBlockWidth-1;y>=pos.y;--y)
            {
                Block&block=blocks[x][y][z];
                if(!block.id)continue;
                vec3 p=vec3{pos.x+x,y,pos.z+z};
                if(block.id==Water){
                    if(y==WaterAltitude)
                    MakeWaterFace(buffer1,p);
                }
                else if(JudgeIsPlant(block))
                    MakePlantFace(buffer0,block,pos);
                else{
                    Block*nearBlock=GetTargetPosAroundAllBlock(vec3{x,y,z});
                    MakeCubeFace(buffer0,block,p,nearBlock);
                }
            }
        }
    }

}

ChunkNearChunks GetChunkNearChunks(Chunk *chunk)
{
    static auto check=[&](int x,int y)->bool{
      static const int loadSize=(WorldChunkWidth+2);
      if(x>=0&&x<loadSize&&y>=0&&y<loadSize)  return 1;
      return 0;
    };
    //结果对应前后左右的临近chunk
    auto&pos=chunk->pos;
    Chunk*front=0,*back=0,*left=0,*right=0;
    vec2 fp=pos+vec2{0,SectionBlockWidth},bp=pos+vec2{0,-SectionBlockWidth},
         lp=pos+vec2{-SectionBlockWidth,0},rp=pos+vec2{SectionBlockWidth,0};
    static const int LoadHalf=(WorldChunkWidth+2)/2;
    int fpxi=(fp.x-CameraChunkPos.x)/SectionBlockWidth+LoadHalf,fpzi=(fp.y-CameraChunkPos.y)/SectionBlockWidth+LoadHalf;
    int bpxi=(bp.x-CameraChunkPos.x)/SectionBlockWidth+LoadHalf,bpzi=(bp.y-CameraChunkPos.y)/SectionBlockWidth+LoadHalf;
    int lpxi=(lp.x-CameraChunkPos.x)/SectionBlockWidth+LoadHalf,lpzi=(lp.y-CameraChunkPos.y)/SectionBlockWidth+LoadHalf;
    int rpxi=(rp.x-CameraChunkPos.x)/SectionBlockWidth+LoadHalf,rpzi=(rp.y-CameraChunkPos.y)/SectionBlockWidth+LoadHalf;
    if(check(fpxi,fpzi)) front=WorldChunk[fpxi][fpzi];
    if(check(bpxi,bpzi)) back=WorldChunk[bpxi][bpzi];
    if(check(lpxi,lpzi)) left=WorldChunk[lpxi][lpzi];
    if(check(rpxi,rpzi)) right=WorldChunk[rpxi][rpzi];
    ////////////////////////////////////
    return ChunkNearChunks{front,back,left,right};
}


void MakeCubeFace(FaceBufferType &buffer, Block &block, const glm::vec3 &pos, Block *NearBlocks)
{
    static const vec3 vertexRelativePos[]={
        vec3{0,1,0},vec3{1,1,0},vec3{1,1,1},vec3{0,1,1},
        vec3{0,0,0},vec3{1,0,0},vec3{1,0,1},vec3{0,0,1},
    };
    auto MakeFace=[&](uint idx0,uint idx1,uint idx2,uint idx3,uint faceDir)->void
    {
           buffer.emplace_back();
           Face&face=buffer.back();
           BLockTexCoordsData&texcoord=BlockTexCoords[block.id][faceDir];
           face.vertex[0].position=vertexRelativePos[idx0]+pos;
           face.vertex[1].position=vertexRelativePos[idx1]+pos;
           face.vertex[2].position=vertexRelativePos[idx2]+pos;
           face.vertex[3].position=vertexRelativePos[idx3]+pos;
           for(int i=0;i<4;++i)face.vertex[i].texcoord=texcoord.offset[i];
    };
    //此处有bug，不应该对叶子进行单独判断，而是只应该判断该方块周围是否有其他的cover方块，以后再改
    if(!JudgeIsCover(NearBlocks[0])||JudgeIsLeaves(block))MakeFace(0,1,2,3,0);
    if(!JudgeIsCover(NearBlocks[1])||JudgeIsLeaves(block))MakeFace(7,6,5,4,1);
    if(!JudgeIsCover(NearBlocks[2])||JudgeIsLeaves(block))MakeFace(3,2,6,7,2+block.meta%4);
    if(!JudgeIsCover(NearBlocks[3])||JudgeIsLeaves(block))MakeFace(1,0,4,5,2+((block.meta+1)&3));
    if(!JudgeIsCover(NearBlocks[4])||JudgeIsLeaves(block))MakeFace(0,3,7,4,2+((block.meta+2)&3));
    if(!JudgeIsCover(NearBlocks[5])||JudgeIsLeaves(block))MakeFace(2,1,5,6,2+((block.meta+3)&3));
}


void MakeWaterFace(FaceBufferType&buffer, const glm::vec3 &pos)
{
    static const float esp=0.1;
    static const vec3 offset[]={vec3{0,1-esp,0},vec3{1,1-esp,0},vec3{1,1-esp,1},vec3{0,1-esp,1}};
    static const vec2 texcoord[]={vec2(0.0,0.0),vec2(1.0,0.0),vec2(1.0,1.0),vec2(0.0,1.0)};
    buffer.emplace_back();
    Face&face=buffer.back();
    for(int i=0;i<4;++i)
        face.vertex[i].position=pos+offset[i];
    for(int i=0;i<4;++i)
        face.vertex[i].texcoord=texcoord[i];
}

bool JudgeIsTransParent(Block &block)
{
    if(block.id==Water)return 1;
    return block.id==0;
}

void MakePlantFace(FaceBufferType &buffer, Block &block, const glm::vec3 &pos)
{
    static const vec3 vertexRelativePos[]={
        vec3{0,1,0},vec3{1,1,0},vec3{1,1,1},vec3{0,1,1},
        vec3{0,0,0},vec3{1,0,0},vec3{1,0,1},vec3{0,0,1},
    };
    auto MakeFace=[&](const vec3&pos,uint idx0,uint idx1,uint idx2,uint idx3,uint idx)->void{
        buffer.emplace_back();
        Face&face0=buffer.back();
        buffer.emplace_back();
        Face&face1=buffer.back();
        BLockTexCoordsData&texcoord=BlockTexCoords[block.id][idx];
        face0.vertex[0].position=vertexRelativePos[idx0]+pos;
        face0.vertex[1].position=vertexRelativePos[idx1]+pos;
        face0.vertex[2].position=vertexRelativePos[idx2]+pos;
        face0.vertex[3].position=vertexRelativePos[idx3]+pos;
        face1.vertex[0].position=vertexRelativePos[idx3]+pos;
        face1.vertex[1].position=vertexRelativePos[idx2]+pos;
        face1.vertex[2].position=vertexRelativePos[idx1]+pos;
        face1.vertex[3].position=vertexRelativePos[idx0]+pos;
        for(int i=0;i<4;++i){
            face0.vertex[i].texcoord=texcoord.offset[i];
            face1.vertex[i].texcoord=texcoord.offset[3-i];
        }

    };
    switch (block.id) {
    case RoseBush:
        MakeFace(pos,0,2,6,4,0);
        MakeFace(pos,3,1,5,7,0);
        MakeFace(vec3{pos.x,pos.y+1,pos.z},0,2,6,4,1);
        MakeFace(vec3{pos.x,pos.y+1,pos.z},3,1,5,7,1);
        break;
    default:
        MakeFace(pos,0,2,6,4,0);
        MakeFace(pos,3,1,5,7,0);
        break;
    }
}

bool JudgeIsPlant(Block &block)
{
    switch (block.id) {
    case WitherRose:case RoseBush:
    case LilyPadFlowerPink:case LilyPadFlowerYellow:
    case TorchFlower:case RedMushRoom:
    case BrownMushRoom:case CactusBush:case TallGrass:
        return 1;
    }
    return 0;
}

bool JudgeIsCover(Block &block)
{
    if(JudgeIsPlant(block))return 0;
    if(JudgeIsTransParent(block))return 0;
    return block.id;
}

bool JudgeBlockCanBarry(Block&block)
{
    if(JudgeIsPlant(block))return 0;
    switch (block.id) {
    case Water:
        return 0;
    }
    return block.id;
}
