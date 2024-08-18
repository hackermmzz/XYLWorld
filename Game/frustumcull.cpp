#include "frustumcull.h"


vector<Section *> &GetNeedRenderSections()
{

    struct Face{
        vec3 position;
        vec3 normal;
    };
    static vector<Section*>visible;
    static const int RenderHalf=WorldChunkWidth/2;
    static const int LoadHalf=(WorldChunkWidth+2)/2;
    static Face CameraFace[6];
    static vec2 camera_chunk;
    static vec3 points[8];
    camera_chunk=NormalizeTargetPosToChunkPos(vec2{controller->camera.position.x,controller->camera.position.z});
    ///////////////////////////////////////////
    static auto UpdateCameraFace=[&]()->void
    {
            //1生成8个点
            static const float tan_half_theta=tan(PersPectiveFov*PI/360),
                half_height_fornt=tan_half_theta*PersPectiveNearDis,
                half_height_back=tan_half_theta*PersPectiveFarDis;
            float  asp=WindowWidth*1.0/WindowHeight,
                half_width_front=asp*half_height_fornt,
                half_width_back=asp*half_height_back;
            vec3 point[8]={
                {-half_width_front,half_height_fornt,-PersPectiveNearDis},
                {half_width_front,half_height_fornt,-PersPectiveNearDis},
                {half_width_front,-half_height_fornt,-PersPectiveNearDis},
                {-half_width_front,-half_height_fornt,-PersPectiveNearDis},
                {-half_width_back,half_height_back,-PersPectiveFarDis},
                {half_width_back,half_height_back,-PersPectiveFarDis},
                {half_width_back,-half_height_back,-PersPectiveFarDis},
                {-half_width_back,-half_height_back,-PersPectiveFarDis}
            };
            auto camera_view_inverse=glm::inverse(View);
            for(int i=0;i<8;++i)
                point[i]=camera_view_inverse*vec4(point[i],1.0);
            vec3 line[]{
                point[5]-point[4],
                point[7]-point[4],
                point[0]-point[4],
                point[6]-point[5],
                point[2]-point[6],
                point[3]-point[2],
                point[1]-point[2]
            };
            ////////////////////////////法线指向视锥体内部
            vec3 normal[]{
                glm::cross(line[5],line[6]),
                glm::cross(line[1],line[0]),
                glm::cross(line[2],line[1]),
                glm::cross(line[4],line[6]),
                glm::cross(line[0],line[2]),
                glm::cross(line[5],line[4])
            };
            //2生成六个面
            auto GenerateCameraFace=[&](int face_index,int i){
                auto&face=CameraFace[face_index];
                face.position=point[i];
                face.normal=glm::normalize(normal[face_index]);
        };
            GenerateCameraFace(0,0);
            GenerateCameraFace(1,4);
            GenerateCameraFace(2,0);
            GenerateCameraFace(3,1);
            GenerateCameraFace(4,0);
            GenerateCameraFace(5,2);
    };
    static  auto JudgeBoxIsInPlane=[&](const vec3 *point,Face&face)->bool
    {
        int cnt=0;
        for(int i=0;i<8;++i)
        {
            float temp=glm::dot(point[i]-face.position,face.normal);
            if(temp<=0)++cnt;
        }
        return cnt!=8;
    };
    static auto JudgeBoxInCameraPlane=[&](const vec3*point)->bool{
      for(int i=0;i<6;++i)
      {
          if(!JudgeBoxIsInPlane(point,CameraFace[i]))return 0;
      }
      return 1;
    };
    /////////////////////////////
    /////////////////////////////////
    UpdateCameraFace();
    visible.clear();
    for(int x=-RenderHalf;x<=RenderHalf;++x)
        for(int z=-RenderHalf;z<=RenderHalf;++z)
        {
            Chunk*chunk=WorldChunk[x+LoadHalf][z+LoadHalf];
            if(chunk&&JudgeBoxInCameraPlane(chunk->BoxPoint)){
                for(Section*section:chunk->section)
                {
                    if(section&&section->completePossDataToGpu&&JudgeBoxInCameraPlane(section->BoxPoint))
                    {
                        visible.push_back(section);
                    }
                }
            }
        }
    return visible;
}
