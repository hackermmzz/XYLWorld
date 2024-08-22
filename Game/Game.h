#ifndef GAME_H
#define GAME_H
#include"globalsource.h"
#include"controller.h"
#include"tuple"
#include"unordered_map"
#include"qdialog.h"
#include"qinputdialog.h"
#include"generateterrain.h"
#include"generateblock.h"
#include"generateface.h"
#include"frustumcull.h"
#include"generatedecoration.h"
#include"qstringlistmodel.h"
#include"rolecontroller.h"
using namespace std;
//////////////////////////////////
void GameInit();
void GameUpdate();
void GameRender();
void RenderSkySphere();
void RenderTerrain(vector<Section*>&section);
void RenderWater(vector<Section*>&section);
void RenderBiomeMap();
void RenderGameInfo();
void RenderRoleShape();
void RenderBlockEdge();
void RenderClimate();
void RenderSnow();
void RenderRain();
void RenderGBuffer();
void InitAllGlobalSrc();
pair<bool,int> EnrtyTheWorldAccordingTheInputSeed();
void GenerateTheWorld(uint seed);
void ClearTheWorld();
void RenderCrossHair();
vec2 GetRoleAvalidPos(const vec2&initPos);//为了防止人物出生在海洋上，提前判断该位置可不可行
void UpdateWorldChunks();
void UpdateDay();
void UpdateCloudNoiseTexture();
void UpdateItemBoard();
void InitBlockTextureCoords(unordered_map<string,vec2>&info,uint col,uint row);
void InitBag(vector<vector<Pixel>>&BlockImage,uint rowCnt,uint colCnt);
void InitItemBoard();
void InitMyBag();
void WorkSpaceBoardInit();
void UpdateWorldChunksForce();
tuple<Texture2D*,unordered_map<string,vec2>,uint,uint,vector<vector<Pixel>>>LoadAllImagesToOneTexture(const char*path);
//////////////////////////////////
#endif
