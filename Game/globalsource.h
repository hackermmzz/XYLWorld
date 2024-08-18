#ifndef GLOBALSOURCE_H
#define GLOBALSOURCE_H
#include"header.h"
#include"Dependency/vao.h"
#include"Dependency/vbo.h"
#include"Dependency/ebo.h"
#include"Dependency/random.h"
#include"Dependency/program.h"
#include"Dependency/texture2d.h"
#include"Dependency/camera.h"
#include"Dependency/model.h"
#include"Dependency/threadpool.h"
#include"Dependency/perlinnoise.h"
#include"Dependency/texture2dgeneral.h"
#include"Dependency/FSM.hpp"
#include"Dependency/aabb.h"
#include"other.h"
#include"Dependency/timer.h"
#include"unordered_map"
#include"Dependency/textur1d.h"
#include"Dependency/particlesystem.h"
#include"qlistview.h"
#include"Dependency/deferredrendering.h"
#include"windows.h"
#include"itemboard.h"
#include"bagbutton.h"
#include"workspaceboard.h"
#include"bag.h"
using namespace std;
////////////////////////////////////////////
struct VEC2CMP{
  bool operator()(const vec2&a,const vec2&b)const;
};
struct VEC3CMP{
  bool operator()(const vec3&a,const vec3&b)const;
};
bool operator<(const vec3&a,const vec3&b);
QDebug operator<<(QDebug dbg,const vec3&v);
////////////////////////////////////////////
struct Chunk;
struct Section;
struct Block;
struct Controller;
struct BLockTexCoordsData{vec2 offset[4];};
struct Pixel{unsigned char pixel[4];};
enum BiomeType{PLAIN,GRASSLAND,RAINFOREST,FOREST,SNOWFIELD,DESERT,OCEAN};
////////////////////////////////////////////
extern const uint GameFps;
extern uint GameInterval;
extern uint RenderTime;
extern uint CurrentWorldSeed;
#define  SectionBlockWidth 16   //一个section含16*16*16个block
#define ChunkSectionWidth 16    //一个chunk 含16个section
#define WorldChunkWidth 31      //每次加载31*31个chunk用于渲染，实际要加载多一层
#define ChunkForceLoad 17       //强制加载17*17个chunk
#define CloudTextureSize 256   //云噪声图大小
extern const float RoleWidth;
extern const float RoleHeight;
extern int CameraMode;
extern float RealCameraDistance;
extern const double PersPectiveFov;
extern const double PersPectiveNearDis;
extern const double PersPectiveFarDis;
extern vec2 CameraChunkPos;
extern  mat4 View;
extern  mat4 PersPective;
extern GLuint BaseBinding;
extern const GLuint CameraUboBinding;
extern const GLfloat RoleMoveSpeed;
extern const float RoleJumpSpeed;
extern const mat4 ControllerModel;
extern Controller*controller;
extern const uint ThreadCount;
extern ThreadPool*threadPool;
extern Texture2D*AllBlocks;
extern unordered_map<uint,vector<BLockTexCoordsData>>BlockTexCoords;
extern PerlinNoiseGenerator ContinentalNoise;
extern PerlinNoiseGenerator ErosionNoise;
extern PerlinNoiseGenerator PeaksNoise;
extern PerlinNoiseGenerator HumidityNoise;
extern PerlinNoiseGenerator TemperatureNoise;
extern map<vec2,Chunk*,VEC2CMP>WorldChunkMap;
extern Chunk* WorldChunk[WorldChunkWidth+2][WorldChunkWidth+2];
extern const uint ChunkAllocatePoolMaxSize;
extern vector<Chunk*>ChunkAllocatePool;//从里面获取chunk
extern const float OceanWeight;
extern const float ContinentalNoiseWeight;
extern const float ErosionNoiseWeight;
extern const float PeakNoiseWeight;
extern const uint ContinentalNoiseFreq;
extern const uint ErosionNoiseFreq;
extern const uint PeakNoiseFreq;
extern const uint HumidityNoiseFreq;
extern const uint TemperatureNoiseFreq;
extern const uint WaterAltitude;
extern const uint BedRockDeep;//基岩生成的深度
extern deque<Section*>SectionNeedLoadFace;
extern Timer*LoadSectionFaceTimer;
extern Timer*GameUpdateTimer;
extern int WindowWidth;
extern int WindowHeight;
extern BiomeType BiomeTypeAround[3][3];
extern void*PalettePtr;
extern QListView*GameInfo;
extern bool ShowGameInfo;
extern map<vec3,int,VEC3CMP>BlockChanged;
extern const float BlockTargetMaxDistance;
extern const int TNTBoomRange;
extern int BagItems[256];
extern Texture2DGeneral*CloudTexture;
extern Texture2DGeneral*StarTexture;
extern float DayPercent;
extern vec3 SunPos;
extern float GlobalBrightness;
extern DeferredRendering*GBuffer;
extern float Exposure;
extern QSlider*ExposureAdjust;
extern bool LimitMouse;
extern map<vector<int>,pair<int,int>>TransitionTable;
extern const uint BagRow;
extern const uint BagColumn;
extern vector<QIcon*>ItemIcons;
extern ItemBoard*itemBoard;
extern int MyBagCurrentSelectedItem;
extern int ItemBoardCurrentSelectedItem;
extern WorkSpaceBoard*workSpaceBoard;
extern const uint ITEM_NUMS;
extern Bag* MyBag;
extern BagButton* ItemBoardAllItems[9];
extern QLabel*ItemBoardAllItemNum[9];
extern BagButton*WorkSpaceBoardSource[9];
////////////////////////////////////////////
Chunk*GetOneChunk();
void AppendToChunkAllocatePool(Chunk*chunk);
vec2 NormalizeTargetPosToChunkPos(const vec2&pos);
Block* GetBlock(const vec3&pos);
Block& GetBlock(int x,int y,int z);
void SetBlock(int x,int y,int z,unsigned char id,unsigned char meta);
Chunk*GetChunk(const vec2&pos);
Section*GetSection(const vec3&pos);
void GenerateBox(vec3*box,const vec3&pos,float w0,float w1,float h);
bool JudgeIsTransParent(Block&block);
bool JudgeIsPlant(Block&block);
bool JudgeIsCover(Block&block);
bool JudgeBlockCanBarry(Block&block);
bool JudgeBlockCanDestroy(Block&block);
bool JudgeBlockIsFunctionBlock(Block&block);
bool JudgeIsLeaves(Block&block);
bool equal(const vec3&a,const vec3&b);
void ReleaseMouse();
void ForceMouse();
//////////////////
#endif // GLOBALSOURCE_H
