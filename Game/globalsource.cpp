#include "globalsource.h"
const float RoleWidth=0.5;
const float RoleHeight=1.8;
int CameraMode;
float RealCameraDistance=5.0;
uint CurrentWorldSeed;
const uint GameFps=90;
uint RenderTime=0;
uint GameInterval=0;
vec2 CameraChunkPos;
const double PersPectiveFov=40.0;
const double PersPectiveNearDis=0.1;
const double PersPectiveFarDis=1000.0;
mat4 View;
mat4 PersPective;
GLuint BaseBinding=0;
const GLuint CameraUboBinding=BaseBinding++;
const GLfloat RoleMoveSpeed=5.0;
const float RoleJumpSpeed = 6;
const mat4 ControllerModel=mat4(1.0);
Controller*controller;
const uint ThreadCount=20;
ThreadPool*threadPool;
Texture2D*AllBlocks;
unordered_map<uint,vector<BLockTexCoordsData>>BlockTexCoords;
PerlinNoiseGenerator ContinentalNoise(0);
PerlinNoiseGenerator ErosionNoise(0);
PerlinNoiseGenerator PeaksNoise(0);
PerlinNoiseGenerator HumidityNoise(0);
PerlinNoiseGenerator TemperatureNoise(0);
map<vec2,Chunk*,VEC2CMP>WorldChunkMap;
Chunk* WorldChunk[WorldChunkWidth+2][WorldChunkWidth+2];
const uint ChunkAllocatePoolMaxSize=(WorldChunkWidth+2)*8;
vector<Chunk*>ChunkAllocatePool;//从里面获取chunk
const float OceanWeight=0.1;
const float ContinentalNoiseWeight=0.7;
const float ErosionNoiseWeight=0.2;
const float PeakNoiseWeight=0.3;
const uint ContinentalNoiseFreq=(SectionBlockWidth*WorldChunkWidth);
const uint ErosionNoiseFreq=SectionBlockWidth*4;
const uint PeakNoiseFreq=SectionBlockWidth*8;
const uint HumidityNoiseFreq=SectionBlockWidth*32;
const uint TemperatureNoiseFreq=SectionBlockWidth*32;
const uint WaterAltitude=SectionBlockWidth*ChunkSectionWidth/4;
const uint BedRockDeep=4;
deque<Section*>SectionNeedLoadFace;
Timer*LoadSectionFaceTimer;
Timer*GameUpdateTimer=new Timer(1000.0/GameFps*(5.0/6.0),[&](){
    GameInterval=QDateTime::currentMSecsSinceEpoch()-GameUpdateTimer->BeginTime;
    extern void GameUpdate();
    GameUpdate();
    GameUpdateTimer->Reset();
});
int WindowWidth;
int WindowHeight;
BiomeType BiomeTypeAround[3][3];
void*PalettePtr;
QListView*GameInfo;
bool ShowGameInfo;
map<vec3,int,VEC3CMP>BlockChanged;
const float BlockTargetMaxDistance=5.0;
const int TNTBoomRange=3;
int BagItems[256];
Texture2DGeneral*CloudTexture;
Texture2DGeneral*StarTexture;
float DayPercent=0.5;//中午
vec3 SunPos;
float GlobalBrightness=1.0;
DeferredRendering*GBuffer;
float Exposure=0.2;
QSlider*ExposureAdjust;
bool LimitMouse=1;
map<vector<int>,pair<int,int>>TransitionTable;
const uint BagRow=16;
const uint BagColumn=16;
vector<QIcon*>ItemIcons;
ItemBoard*itemBoard;
int MyBagCurrentSelectedItem=0;
int ItemBoardCurrentSelectedItem = 0;
WorkSpaceBoard*workSpaceBoard;
const uint ITEM_NUMS=37;
Bag* MyBag;
BagButton* ItemBoardAllItems[9];
QLabel*ItemBoardAllItemNum[9];
BagButton*WorkSpaceBoardSource[9];
///////////////////////////////////////////////////////
bool VEC2CMP::operator()(const glm::vec2 &a, const glm::vec2 &b)const
{

    if(a.x==b.x)return a.y<b.y;
    return a.x<b.x;
}



glm::vec2 NormalizeTargetPosToChunkPos(const glm::vec2 &pos)
{
    static const uint shift=Get_2_N_POW(SectionBlockWidth);
    int x=int(pos.x),z=int(pos.y);
    x=x>>shift<<shift,z=z>>shift<<shift;
    return vec2{x,z};
}

QDebug operator<<(QDebug dbg, const glm::vec3 &v)
{
    dbg<<v.x<<" "<<v.y<<" "<<v.z;
    return dbg;
}

void GenerateBox(glm::vec3 *box, const glm::vec3 &pos, float w0, float w1, float h)
{
    box[0]=pos;
    box[1]=vec3{pos.x+w0,pos.y,pos.z};
    box[2]=vec3{pos.x+w0,pos.y,pos.z+w1};
    box[3]=vec3{pos.x,pos.y,pos.z+w1};
    box[4]=vec3{pos.x,pos.y+h,pos.z};
    box[5]=vec3{pos.x+w0,pos.y+h,pos.z};
    box[6]=vec3{pos.x+w0,pos.y+h,pos.z+w1};
    box[7]=vec3{pos.x,pos.y+h,pos.z+w1};
}



bool VEC3CMP::operator()(const glm::vec3 &a, const glm::vec3 &b) const
{
    return a<b;
}

bool equal(const glm::vec3 &a, const glm::vec3 &b)
{
    return a.x==b.x&&a.y==b.y&&a.z==b.z;
}

bool operator<(const glm::vec3 &a, const glm::vec3 &b)
{
    if(a.x<b.x)return 1;
    if(a.x>b.x)return 0;
    if(a.y<b.y)return 1;
    if(a.y>b.y)return 0;
    if(a.z<b.z)return 1;
    if(a.z>b.z)return 0;
    return 1;
}

void ReleaseMouse()
{
    LimitMouse=0;
    QOpenGLWidget*widget=(QOpenGLWidget*)PalettePtr;
    widget->setCursor(Qt::ArrowCursor);
    widget->setMouseTracking(false);
    ClipCursor(0);
}

void ForceMouse()
{
    LimitMouse=1;
    QOpenGLWidget*widget=(QOpenGLWidget*)PalettePtr;
    widget->setCursor(Qt::BlankCursor);
    widget->setMouseTracking(true);
    QPoint topleft = widget->mapToGlobal(widget->rect().topLeft());
    RECT rect;
    rect.bottom=topleft.y()+WindowHeight;
    rect.top=topleft.y();
    rect.left=topleft.x();
    rect.right=topleft.x()+WindowWidth;
    ClipCursor(&rect);
}
