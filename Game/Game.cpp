#include"Game.h"
void GameInit()
{
    /////////////////////////////加载所有全局资源
    InitAllGlobalSrc();
    /////////////////////////////输入一个随机种子并且生成世界
    if(!EnrtyTheWorldAccordingTheInputSeed().first)exit(0);

}

void GameUpdate()
{
    //更新人物状态机
    rolecontroller.Run();
    //更新摄像机
    controller->updateCamera();
    //更新人物所在区块坐标
    CameraChunkPos=NormalizeTargetPosToChunkPos(vec2{controller->camera.position.x,controller->camera.position.z});
    //更新区块
    UpdateWorldChunks();
    //更新云噪声纹理图
    UpdateCloudNoiseTexture();
    //更新一天时间
    UpdateDay();
    //更新itemboard
    UpdateItemBoard();

}

void GameRender()
{
    ///////////////////////////////////
    auto&sections=GetNeedRenderSections();
    ///////////////////////////////////
    //渲染游戏信息
    RenderGameInfo();
    //渲染准线
    RenderCrossHair();
    //////////////////////////////////////////渲染至fbo中
    static Fbo*MainFbo=new Fbo(((QOpenGLWidget*)PalettePtr)->defaultFramebufferObject());
    GBuffer->bind();
    auto&GBuffers=GBuffer->GetBuffers();
    GBuffers.front()->fitViewport();
    //渲染气候
    RenderClimate();
    //渲染地形
    RenderTerrain(sections);
    //渲染人物
    RenderRoleShape();
    //渲染海洋
    RenderWater(sections);
    //渲染GBuffer
    MainFbo->bind();
    api->glViewport(0,0,WindowWidth, WindowHeight);
    RenderGBuffer();
    GBuffer->blitbuffer(*MainFbo,GBuffer->GetBufferWidth(), GBuffer->GetBufferHeight(), WindowWidth,WindowHeight,GL_DEPTH_BUFFER_BIT);
    ////////////////////////////////////////
    //渲染选中的block边框
    RenderBlockEdge();
    //渲染天空球
    RenderSkySphere();
    //渲染地图
    RenderBiomeMap();
}

void RenderSkySphere()
{
    static Model model("src/SkySphere/SkySphere.obj");
    static Program*program=0;
    if(!program)
    {
        program=new Program("src/Shader/SkySphere/render.vert","src/Shader/SkySphere/render.frag");
    }
    api->glDepthFunc(GL_LEQUAL);
    program->bind();
    program->setMat4("vp",PersPective*mat4(mat3(View)));
    program->setInt("Cloud",0);
    CloudTexture->bind(0);
    program->setVec3("SunPos",SunPos);
    program->setFloat("Brightness",GlobalBrightness);
    program->setInt("Star",1);
    StarTexture->bind(1);
    model.Render();
    api->glDepthFunc(GL_LESS);
}

void InitAllGlobalSrc()
{

    //初始化主角
    controller=new Controller(vec3{0,0,0});
    //初始化线程池
    threadPool=new ThreadPool(ThreadCount);
    //加载所有block纹理
    auto&&ret=LoadAllImagesToOneTexture("src\\Assets\\minecraft\\textures\\block");
    AllBlocks=get<0>(ret);
    InitBlockTextureCoords(get<1>(ret),get<2>(ret),get<3>(ret));
    //初始化人物的背包
    InitBag(get<4>(ret),get<3>(ret),get<2>(ret));
    //初始化云噪声纹理图片
    CloudTexture=new Texture2DGeneral(CloudTextureSize,CloudTextureSize,GL_R16F,GL_RED,GL_FLOAT,0);
    CloudTexture->filter(GL_LINEAR);
    CloudTexture->wrap(GL_CLAMP_TO_EDGE);
    //初始化GBuffer
    {
        static const uint GBufferSize=2048;
        vector<GLenum>GBufferType{GL_RGBA16F,GL_RGBA32F,GL_RGBA16F };//颜色 位置-深度 法线
        GBuffer = new DeferredRendering(GBufferSize,GBufferSize,GBufferType);
        //////////////////////////////////////////
    }
    //初始化星星的随机噪声图
    {
        static const uint Percent=888;
        static const uint StarTextureSize=2048;
        vector<vec3>data(StarTextureSize*StarTextureSize);
        StarTexture=new Texture2DGeneral(StarTextureSize,StarTextureSize,GL_RGB,GL_RGB,GL_FLOAT,0);
        StarTexture->filter(GL_LINEAR);
        StarTexture->wrap(GL_CLAMP_TO_EDGE);
        for(auto&v:data)
        {
            if(randomInt(0,Percent))continue;
            static const vec3 color0{0.0,0.0,1.0},color1{0.0,0.8,1.0};
            float fac=randomFloat();
            v=glm::mix(color0,color1,vec3{fac,fac,fac});
        }
        api->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, StarTextureSize, StarTextureSize,GL_RGB, GL_FLOAT, data.data());
    }
    //初始化
    const uint SingleLoadTime=10;
    static const uint SingeLoadCnt=100;
    static const int RenderHalf=WorldChunkWidth/2;
    LoadSectionFaceTimer=new Timer(SingleLoadTime,[&](){

        for(int cnt=0;cnt<SingeLoadCnt&&!SectionNeedLoadFace.empty();)
        {
            auto*section=SectionNeedLoadFace.front();
            SectionNeedLoadFace.pop_front();
            //判断section是否需要加载face
            auto&pos=section->Chunk->pos;
            int xi=(pos.x-CameraChunkPos.x)/SectionBlockWidth,zi=(pos.y-CameraChunkPos.y)/SectionBlockWidth;
            xi=abs(xi),zi=abs(zi);
            if(xi<=RenderHalf&&zi<=RenderHalf)
            {
                section->PossDataToGpu();
                section->completePossDataToGpu=1;
                ++cnt;
            }
         }
         LoadSectionFaceTimer->Reset();
   });
}

tuple<Texture2D *, unordered_map<string, glm::vec2>, uint, uint, vector<vector<Pixel> > > LoadAllImagesToOneTexture(const char *path)
{
    unordered_map<string,vec2>mp;
    vector<QImage>images;
    vector<string>rm_suffix;
    QDir dir(path);
    auto fileList=dir.entryList();
    fileList.removeOne(".");
    fileList.removeOne("..");
    for(QString name:fileList)
    {
        QString fileName=QString(path)+"\\"+name;
       images.push_back(QImage(fileName));
       auto rm_suffix_=name.split(".").first();
       rm_suffix.push_back(rm_suffix_.toStdString());
    }
    uint cnt=images.size();
    uint mnW=-1,mnH=-1;
    for(auto&img:images)
    {
       mnW=std::min(mnW,uint(img.width()));
       mnH=std::min(mnH,uint(img.height()));
    }
    /////////////////////////////////不得低于最小分辨率
    const uint minPixel=64;
    mnW=max(mnW,minPixel);
    mnH=max(mnH,minPixel);
    /////////////////////////////////
    for(auto&img:images)
        img=img.scaled(mnW, mnH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGBA8888);
    uint row_col=uint(std::ceil(std::log2(cnt)));
    uint row=row_col/2,col=row_col-row;
    uint cntPerRow=1<<row;
    uint cntPerCol=1<<col;
    uint singleSize=mnW*mnH*4;
    vector<vector<Pixel>>PreData(mnH*cntPerRow,vector<Pixel>(mnW*cntPerCol));
    for(int i=0;i<images.size();++i)
    {
       string name=rm_suffix[i];
       int r=i/cntPerCol,c=i%cntPerCol;
       mp[name]=vec2{c*1.0/cntPerCol,r*1.0/cntPerRow};
       auto&img=images[i];
       int rb=mnH*r,cb=mnW*c,idx=0;
       auto*bits=img.bits();
       for(int ro=0;ro<mnH;++ro)
       {
            for(int co=0;co<mnW;++co)
            {
                auto&pixel=PreData[rb+ro][cb+co];
                pixel.pixel[0]=bits[idx++];
                pixel.pixel[1]=bits[idx++];
                pixel.pixel[2]=bits[idx++];
                pixel.pixel[3]=bits[idx++];
            }
       }
    }
    int DataIdx=0,RowDataSize=PreData.front().size()*sizeof(Pixel);
     vector<unsigned char>data(singleSize*(1<<row_col));
     for(auto&v:PreData)
     {
         memcpy(&data[DataIdx],v.data(),RowDataSize);
         DataIdx+=RowDataSize;
     }
    return tuple<Texture2D*,unordered_map<string,vec2>,uint,uint,vector<vector<Pixel>>>
    {new Texture2D(mnW*cntPerCol,mnH*cntPerRow,data.data()),mp,cntPerCol,cntPerRow,PreData};
}

pair<bool, int> EnrtyTheWorldAccordingTheInputSeed()
{
    //输入一个种子
    bool ok;
    auto WorldSeed=QInputDialog::getInt((QWidget*)PalettePtr,"Please Input The World Seed","Seed",20041118,INT_MIN,INT_MAX,1,&ok);
    if(!ok)return {0,WorldSeed};
    CurrentWorldSeed=WorldSeed;
    /////////////////////////////清空前一个世界
    ClearTheWorld();
    /////////////////////////////生成世界
    GenerateTheWorld(uint(WorldSeed));
    /////////////////////////////初始化人物位置
    {
        controller->camera.position=vec3{0.5,0,0.5};
        Chunk*chunk=WorldChunk[(WorldChunkWidth+2)/2][(WorldChunkWidth+2)/2];
        auto&block=chunk->block;
        for(int h=SectionBlockWidth*ChunkSectionWidth-1;h>=0;--h)
        {
            if(JudgeBlockCanBarry(block[0][h][0])){
                controller->camera.position.y=h+1.0+RoleHeight;
                break;
            }
        }
    }
    /////////////////////////////
    return {1,WorldSeed};
}

void GenerateTheWorld(uint seed)
{
    /////////////初始化噪声生成器
    static const uint SeedBonus=(200411188%1314)*(20050119%520);
    seed+=SeedBonus;
    ContinentalNoise=PerlinNoiseGenerator(seed);
    ErosionNoise=PerlinNoiseGenerator(int(pow(seed+1314,2)));
    PeaksNoise=PerlinNoiseGenerator(int(pow(seed+520,2)));
    HumidityNoise=PerlinNoiseGenerator(int(pow(seed+8888,2)));
    TemperatureNoise=PerlinNoiseGenerator(int(pow(seed+4444,2)));
    /////////////生成世界
    auto check=[&]()->bool{
           static const int RenderHalf=WorldChunkWidth/2;
           static const int LoadHalf=(WorldChunkWidth+2)/2;
           for(int x=-RenderHalf;x<=RenderHalf;++x)
            for(int z=-RenderHalf;z<=RenderHalf;++z){
                Chunk*chunk=WorldChunk[x+LoadHalf][z+LoadHalf];
                if(chunk->GenerateFaceState!=5)return 1;
            }
            return 0;
    };
    do{
        UpdateWorldChunks();
    }while(check());
}

void ClearTheWorld()
{
    //等待上个世界的所有区块加载完毕
    bool flag=1;
    while(flag){
        flag=0;
        for(auto&v:WorldChunk)
            for(Chunk*e:v)
                if(e&&e->beUsing)flag=1;
    }
    //卸载所有的chunk
    for(auto&element:WorldChunkMap)
        AppendToChunkAllocatePool(element.second);
    //清空世界
    WorldChunkMap.clear();
    memset(WorldChunk,0,sizeof(WorldChunk));
    //清空面加载队列
    SectionNeedLoadFace.clear();
    //清空所有更改
    BlockChanged.clear();
}

glm::vec2 GetRoleAvalidPos(const glm::vec2 &initPos)
{

}

void UpdateWorldChunks()
{
    static const int LoadSize=WorldChunkWidth+2;
    static const int LoadHalf=LoadSize/2;
    static const int RenderHalf=WorldChunkWidth/2;
    ////////////////////卸载多余的chunk
    static vector<decltype(WorldChunkMap.end())>NeedRemoveItr;
    NeedRemoveItr.clear();
    for(auto itr=WorldChunkMap.begin();itr!=WorldChunkMap.end();++itr)
    {
        Chunk*chunk=itr->second;
        if(chunk->beUsing)continue;
        vec2&p=chunk->pos;
        int xi=int(p.x-CameraChunkPos.x)/SectionBlockWidth+LoadHalf,zi=int(p.y-CameraChunkPos.y)/SectionBlockWidth+LoadHalf;
        if(xi>=0&&xi<LoadSize&&zi>=0&&zi<LoadSize)continue;
        AppendToChunkAllocatePool(chunk);
        NeedRemoveItr.push_back(itr);
    }
    for(auto itr:NeedRemoveItr)WorldChunkMap.erase(itr);
    /////////////////更新区块并且加载到WorldChunk里面
    for(int x=-LoadHalf;x<=LoadHalf;++x)
    {
        for(int z=-LoadHalf;z<=LoadHalf;++z)
        {
            vec2 tarChunkPos=CameraChunkPos+vec2{x*SectionBlockWidth,z*SectionBlockWidth};
            auto*chunk=WorldChunkMap[tarChunkPos];
            if(!chunk)
            {
                chunk=GetOneChunk();
                chunk->Init(tarChunkPos);
                WorldChunkMap[tarChunkPos]=chunk;
                ++chunk->beUsing;
                threadPool->append([=](){
                    GenerateTerrain(chunk);
                    GenerateBlock(chunk);
                    chunk->completeLoadBlock=1;
                    --chunk->beUsing;
                });
            }
            WorldChunk[x+LoadHalf][z+LoadHalf]=chunk;
        }
    }
    ///////////////加载需要渲染区域的装饰和面
    for(int x=-RenderHalf;x<=RenderHalf;++x)
    {
        for(int z=-RenderHalf;z<=RenderHalf;++z)
        {
            auto*chunk=WorldChunk[x+LoadHalf][z+LoadHalf];
            if(!chunk->completeLoadBlock)continue;
            if(!chunk->GenerateFaceState||chunk->GenerateFaceState==2)
            {
                auto cnc=GetChunkNearChunks(chunk);
                Chunk*front=get<0>(cnc),*back=get<1>(cnc),*left=get<2>(cnc),*right=get<3>(cnc);
                if(front&&back&&left&&right&&front->completeLoadBlock&&back->completeLoadBlock&&left->completeLoadBlock&&right->completeLoadBlock)
                {
                    ++chunk->GenerateFaceState;//1表示正在产生装饰,3表示正在产生面
                    ++chunk->beUsing;
                    ++front->beUsing;
                    ++back->beUsing;
                    ++left->beUsing;
                    ++right->beUsing;
                   threadPool->append([=](){
                        auto state=chunk->GenerateFaceState;
                        if(state==1)
                            GenerateDecoration(chunk);
                        else if(state==3){
                            for(Section*section:chunk->section)
                                GenerateFace(section,cnc);
                        }
                        ++chunk->GenerateFaceState;//2表示装饰产生完成,4表示面产生完成
                        --chunk->beUsing;
                        --front->beUsing;
                        --back->beUsing;
                        --left->beUsing;
                        --right->beUsing;
                   });
                }
             }
            else if(chunk->GenerateFaceState==4)
            {
                  for(auto*section:chunk->section)SectionNeedLoadFace.push_front(section);
                  chunk->GenerateFaceState=5;//3表示已经把section放入面加载队列
            }
        }
    }
}

void RenderTerrain(vector<Section*>&NeedRenderSections)
{
    static Program*program=0;
    if(!program)
    {
        program=new Program("src/Shader/Terrain/render.vert","src/Shader/Terrain/render.frag");
        program->setUbo("VP",CameraUboBinding);
    }
    api->glEnable(GL_CULL_FACE);
    api->glCullFace(GL_BACK);
    api->glFrontFace(GL_CW);
    program->bind();
    program->setVec3("Eye",controller->camera.position);
    program->setUint("sampler",0);
    AllBlocks->bind(0);
    for(auto*section:NeedRenderSections)
    {
        //绘制方块
        uint IndicesCnt=section->face.size()*6;
        if(IndicesCnt){
            section->vao->bind();
            api->glDrawElements(GL_TRIANGLES,IndicesCnt,GL_UNSIGNED_INT,0);
            section->vao->unbind();
        }
    }
    api->glDisable(GL_CULL_FACE);
}


void InitBlockTextureCoords(unordered_map<string, vec2> &info, uint col, uint row)
{
    float xSize=1.0/col,ySize=1.0/row;
    auto&config=BlockTexCoords;
    auto MakeBlockTexCoord=[&](uint type,const vector<string>&names)->void{
        auto&v=config[type];
        for(auto&name:names){
            if(info.count(name)==0)
            {
                qDebug()<<name.c_str()<<"texture not found!";
                continue;
            }
            auto&beginP=info[name];
            v.emplace_back();
            auto&data=v.back().offset;
            data[0]=beginP;
            data[1]=beginP+vec2{xSize,0};
            data[2]=beginP+vec2{xSize,ySize};
            data[3]=beginP+vec2{0,ySize};
       }
    };
    MakeBlockTexCoord(BedRock,{"bedrock","bedrock","bedrock","bedrock","bedrock","bedrock"});
    MakeBlockTexCoord(SAND,{"sandstone_top","smooth_sandstone_double","smooth_sandstone_double","smooth_sandstone_double","smooth_sandstone_double","smooth_sandstone_double"});
    MakeBlockTexCoord(STONE,{"stone","smooth_stone_slab_side","smooth_stone_slab_side","smooth_stone_slab_side","smooth_stone_slab_side","smooth_stone_slab_side"});
    MakeBlockTexCoord(GRASS,{"grass_block_top1","raw_copper_block","grass_block_side","grass_block_side","grass_block_side","grass_block_side"});
    MakeBlockTexCoord(BRICK,{"nether_bricks1","nether_bricks1","nether_bricks1","nether_bricks1","nether_bricks1","nether_bricks1"});
    MakeBlockTexCoord(StrippedCherryLog,{"stripped_cherry_log_top","stripped_cherry_log","stripped_cherry_log","stripped_cherry_log","stripped_cherry_log","stripped_cherry_log"});
    MakeBlockTexCoord(CherryLog,{"stripped_cherry_log_top","cherry_log1","cherry_log1","cherry_log1","cherry_log1","cherry_log1"});
    MakeBlockTexCoord(CherryLogLeaves,{"cherry_leaves","cherry_leaves","cherry_leaves","cherry_leaves","cherry_leaves","cherry_leaves"});
    MakeBlockTexCoord(StrippedJungleLog,{"jungle_log_top","stripped_jungle_log","stripped_jungle_log","stripped_jungle_log","stripped_jungle_log","stripped_jungle_log"});
    MakeBlockTexCoord(JungleLog,{"jungle_log_top","jungle_log","jungle_log","jungle_log","jungle_log","jungle_log"});
    MakeBlockTexCoord(JungleLogLeaves,{"jungle_leaves","jungle_leaves","jungle_leaves","jungle_leaves","jungle_leaves","jungle_leaves"});
    MakeBlockTexCoord(StrippedAcaciaLog,{"stripped_acacia_log_top","stripped_acacia_log","stripped_acacia_log","stripped_acacia_log","stripped_acacia_log","stripped_acacia_log"});
    MakeBlockTexCoord(AcaciaLog,{"acacia_log_top","acacia_log","acacia_log","acacia_log","acacia_log","acacia_log"});
    MakeBlockTexCoord(AcaciaLogLeaves,{"acacia_leaves","acacia_leaves","acacia_leaves","acacia_leaves","acacia_leaves","acacia_leaves"});
    MakeBlockTexCoord(StrippedBirchLog,{"stripped_birch_log_top","stripped_birch_log","stripped_birch_log","stripped_birch_log","stripped_birch_log","stripped_birch_log"});
    MakeBlockTexCoord(BirchLog,{"birch_log_top","birch_log","birch_log_top","birch_log_top","birch_log_top","birch_log_top"});
    MakeBlockTexCoord(BirchLogLeaves,{"birch_leaves","birch_leaves","birch_leaves","birch_leaves","birch_leaves","birch_leaves"});
    MakeBlockTexCoord(DIRT,{"dirt","dirt","dirt","dirt","dirt","dirt"});
    MakeBlockTexCoord(SNOW,{"snow","grass_block_snow3","grass_block_snow3","grass_block_snow3","grass_block_snow3","grass_block_snow3"});
    MakeBlockTexCoord(GLASS,{"lime_stained_glass_pane_top","white_stained_glass","white_stained_glass","white_stained_glass","white_stained_glass","white_stained_glass"});
    MakeBlockTexCoord(CobbleLess,{"mossy_cobblestone","dirt","dirt","dirt","dirt","dirt"});
    MakeBlockTexCoord(CobbleMore,{"mossy_cobblestone_double","dirt","dirt","dirt","dirt","dirt"});
    MakeBlockTexCoord(WitherRose,{"wither_rose"});
    MakeBlockTexCoord(RoseBush,{"rose_bush_bottom","rose_bush_top"});
    MakeBlockTexCoord(LilyPadFlowerPink,{"lily_pad_flower1"});
    MakeBlockTexCoord(LilyPadFlowerYellow,{"lily_pad_flower2"});
    MakeBlockTexCoord(TorchFlower,{"torchflower"});
    MakeBlockTexCoord(RedMushRoom,{"mushroom_block_inside","mushroom_stem","red_mushroom_block","red_mushroom_block","red_mushroom_block","red_mushroom_block","red_mushroom","red_mushroom1","red_mushroom2","red_mushroom3","red_mushroom4"});
    MakeBlockTexCoord(BrownMushRoom,{"mushroom_block_inside","mushroom_stem","brown_mushroom_block","brown_mushroom_block","brown_mushroom_block","brown_mushroom_block","brown_mushroom","brown_mushroom1","brown_mushroom2","brown_mushroom3","brown_mushroom4"});
    MakeBlockTexCoord(Pumpkin,{"pumpkin_top","pumpkin_bottom","carved_pumpkin","pumpkin_side","pumpkin_side","pumpkin_side","pumpkin_stem"});
    MakeBlockTexCoord(Cactus,{"cactus_top","cactus_bottom","cactus","cactus","cactus","cactus"});
    MakeBlockTexCoord(CactusBush,{"cactus_bush"});
    MakeBlockTexCoord(TNT,{"tnt_top","tnt_bottom","tnt_side","tnt_side","tnt_side","tnt_side"});
    MakeBlockTexCoord(TallGrass,{"TallGrass"});
    MakeBlockTexCoord(WorkSpace,{"WorkSpaceTop","WorkSpaceTop","WorkSpaceSide","WorkSpaceSide","WorkSpaceSide","WorkSpaceSide"});
}


void RenderCrossHair()
{
    static Program*p=0;
    static Vao*vao=0;
    static const float size=30;
    if(!p)
    {
        p=new Program("src/Shader/CrossHair/render.vert","src/Shader/CrossHair/render.frag");
        vao=new Vao;
        vec2 arrays[]{{-size,0},{size,0},{0,size},{0,-size}};
        Vbo*vbo=new Vbo(sizeof(arrays),arrays);
        vbo->setpoint(0,GL_FLOAT,2,sizeof(vec2),0);
        vao->unbind();
    }
    p->bind();
    p->setFloat("width",WindowWidth);
    p->setFloat("height",WindowHeight);
    vao->bind();
    api->glDrawArrays(GL_LINES,0,4);
    vao->unbind();
}

void RenderBiomeMap()
{
    static Program*program;
    static Vao*vao;
    static Vbo*vbo;
    static Ebo*ebo;
    static Texture2DGeneral* Biome_Map=0;
    static const uint BIOMEMAP_SIZE=(WorldChunkWidth+2)*4;
    if(!Biome_Map)
    {
        program=new Program("Src/Shader/BiomeMap/render.vert","Src/Shader/BiomeMap/render.frag");;
        Biome_Map=new Texture2DGeneral(BIOMEMAP_SIZE,BIOMEMAP_SIZE,GL_RGB,GL_RGB,GL_FLOAT,0);
        Biome_Map->filter(GL_LINEAR);
        Biome_Map->wrap(GL_CLAMP_TO_EDGE);
        vao=new Vao;
        vec4 pos_and_uv[4];
        float map_size=0.3;
        pos_and_uv[0]={-1,1,0,1};//左上
        pos_and_uv[1]=pos_and_uv[0]+vec4(map_size,0,1,0);//右上
        pos_and_uv[2]=pos_and_uv[1]+vec4(0,-map_size,0,-1);//右下
        pos_and_uv[3]=pos_and_uv[0]+vec4(0,-map_size,0,-1);//左下
        vbo=new Vbo(sizeof(pos_and_uv),pos_and_uv);
        vbo->setpoint(0,GL_FLOAT,2,sizeof(GLfloat)*4,0);
        vbo->setpoint(1,GL_FLOAT,2,sizeof(GLfloat)*4,sizeof(GLfloat)*2);
        GLuint indices[]{
            0,1,2,0,2,3
        };
        ebo=new Ebo(sizeof(indices)/sizeof(GLuint),indices);
        vao->unbind();
    }
    static vec2 prePos =vec2(INT_MAX/2,INT_MIN/2);
    static float eps=std::numeric_limits<float>::epsilon();
    if (abs(prePos.x-CameraChunkPos.x)>eps||abs(prePos.y-CameraChunkPos.y)>eps)
    {
        prePos=CameraChunkPos;
        ////////////////////////更新地图
        {
            Biome_Map->bind();
            static int HALF_BIOMEMAP_SIZE=BIOMEMAP_SIZE/2;
            static vec3 data[BIOMEMAP_SIZE][BIOMEMAP_SIZE];
            for(int x=-HALF_BIOMEMAP_SIZE;x<HALF_BIOMEMAP_SIZE;++x)
            {
                for(int z=-HALF_BIOMEMAP_SIZE;z<HALF_BIOMEMAP_SIZE;++z)
                {
                    ////////////////////////////////
                    vec2 position=vec2(x*SectionBlockWidth,z*SectionBlockWidth)+CameraChunkPos;
                    BiomeType biome=GenerateBiomeMap(position).first;
                    auto& pixel = data[x + HALF_BIOMEMAP_SIZE][z + HALF_BIOMEMAP_SIZE];
                    if (biome ==DESERT)pixel = vec3(0.5, 0.6, 0.0);
                    else if (biome == SNOWFIELD)pixel = vec3(1.0, 1.0, 1.0);
                    else if (biome == PLAIN)pixel = vec3(0.5, 0.5, 0);
                    else if (biome == GRASSLAND)pixel = vec3(0, 0.5, 0);
                    else if (biome == FOREST)pixel = vec3(0, 0.8, 0);
                    else if (biome == RAINFOREST)pixel = vec3(0, 1, 0);
                    else if (biome == OCEAN)pixel = vec3(0, 0, 1);
                    ////////////////////////////////获取人物周围的生物群落
                    if (abs(x) <= 1 && abs(z) <= 1)
                        BiomeTypeAround[x + 1][z + 1] = biome;
                }
            }
            /////////////////对人物所在位置进行特殊着色
            static int weight = 3;
            for (int x = -weight; x <= weight; ++x)
            {
                for (int z = -weight; z <= weight; ++z) {
                    data[HALF_BIOMEMAP_SIZE+x][HALF_BIOMEMAP_SIZE+z]= vec3(1, 0, 0);
                }
            }
            ////////////////
            api->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, BIOMEMAP_SIZE, BIOMEMAP_SIZE, GL_RGB, GL_FLOAT, data);
            Biome_Map->unbind();
        }

    }
    api->glDisable(GL_DEPTH_TEST);
    api->glEnable(GL_BLEND);
    api->glBlendEquation(GL_FUNC_ADD);
    api->glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    vao->bind();
    program->bind();
    Biome_Map->bind(0);
    program->setUint("BiomeMap",0);
    api->glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    api->glEnable(GL_DEPTH_TEST);
    api->glDisable(GL_BLEND);
}


void RenderGameInfo()
{
    if(!GameInfo)return;
    if(ShowGameInfo){
        //////////////////////////////////////////
        QStringList list;
        //
        list.append(QString("GameFps: ")+QString::number(1000/RenderTime));
        list.append(QString("RolePos:")+QString("   X:")+QString::number(controller->camera.position.x,'f',1)+QString("   Y:")+QString::number(controller->camera.position.y,'f',1)+QString("   Z:")+QString::number(controller->camera.position.z,'f',1));
        list.append(QString("GameCurrentTime: ")+QString::number(DayPercent*24,'f',1)+QString("H"));
        list.append(QString("CurrentTime: ")+QDateTime::currentDateTime().toString());
        //
        QStringListModel*model=new QStringListModel(list);
        GameInfo->setModel(model);
        //////////////////////////////////////////
        GameInfo->show();
    }
    else
        GameInfo->hide();
}

void RenderWater(vector<Section *> &sections)
{
    static Program*program=0;
    static Texture2D*wave0,*wave1,*water;
    if(!program)
    {
        program=new Program("src/Shader/Water/render.vert","src/Shader/Water/render.frag");
        program->setUbo("VP",CameraUboBinding);
        wave0=new Texture2D("src/Water/wave0.jpg");
        wave1=new Texture2D("src/Water/wave1.jpg");
        water=new Texture2D("src/Water/water.png");
    }
    //////////////////////////////////////
    static int Time = 2000;
    static auto beginTime=QDateTime::currentMSecsSinceEpoch();
    auto currentTime=QDateTime::currentMSecsSinceEpoch()-beginTime;
    api->glEnable(GL_BLEND);
    api->glBlendEquation(GL_FUNC_ADD);
    api->glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    program->bind();
  //  program->setVec3("camera", controller->camera.position);
 //   program->setFloat("time", currentTime*1.0/Time);
    wave0->bind(0);
    wave1->bind(1);
    water->bind(2);
 //   program->setInt("wave0",0);
 //   program->setInt("wave1",1);
    program->setInt("water",2);
    /////////////////////////////////////
    for (auto& section : sections)
    {
        //绘制水面
        auto IndicesCnt=section->waterFace.size()*6;
        if(IndicesCnt){
            section->waterVao->bind();
            api->glDrawElements(GL_TRIANGLES,IndicesCnt,GL_UNSIGNED_INT,0);
            section->waterVao->unbind();
       }
    }
    api->glDisable(GL_BLEND);
}

void RenderRoleShape()
{
    if(CameraMode==0)return;
    static Model*model;
    static Program*program=0;
    static const float Eps=std::numeric_limits<float>::epsilon();
    if(!program)
    {
        program=new Program("src/Shader/Role/render.vert","src/Shader/Role/render.frag");
        program->setUbo("VP",CameraUboBinding);
        program->setVec3("Model",vec3{RoleWidth,0.8,RoleWidth});
        model=new Model("src/Role/role.obj");
    }
    program->bind();
    program->setVec3("RolePos",controller->camera.position);
    model->Render();
}

void RenderBlockEdge()
{
    static Program* program=0;
    static Vao* vao;
    static Vbo* vbo;
    static Ebo* ebo;
    if (!program)
    {
        program = new Program("src/Shader/BlockEdge/render.vert","src/Shader/BlockEdge/render.frag");
        program->setUbo("VP", CameraUboBinding);
        vao = new Vao;
        vec3 pos[]{
            {0,1,0},{1,1,0},{1,1,1},{0,1,1},
            {0,0,0},{1,0,0},{1,0,1},{0,0,1}
        };
        GLuint indices[]{
        0,1,1,2,2,3,3,0,
        4,5,5,6,6,7,7,4,
        3,7,2,6,
        0,4,1,5,
        };
        vbo = new Vbo(sizeof(pos),pos);
        vbo->setpoint(0, GL_FLOAT, 3, sizeof(GLfloat)*3, 0);
        ebo = new Ebo(sizeof(indices) / sizeof(GLuint), indices);
        vao->unbind();
    }
    //////////////////////////////////////////根据摄像机得到目标
    static vec3 blockOffset[]{
        {1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
    };
    vec3 pos;
    bool flag = 0;
    auto&camera=controller->camera;
    for (RayMatch ray(camera.position, camera.front); ray.Distance() < BlockTargetMaxDistance; ray.Step(0.1))
    {
        pos = ray.GetRay();
        pos =vec3{floor(pos.x),floor(pos.y),floor(pos.z)};
        //////////////////////判断是否比最高高度还高
        static int MAX_HEIGHT = ChunkSectionWidth * SectionBlockWidth-1;
        if (pos.y > MAX_HEIGHT)continue;
        ////////////////////
        Block& block =GetBlock(pos.x,pos.y,pos.z);
        /////////////////////////////////判断是否是水
        if (block.id&&block.id!=Water)
            flag = 1;
        if (flag)break;
    }
    //////////////////////////////////////////
    if (!flag)return;
    api->glLineWidth(8.0);
    vao->bind();
    program->bind();
    program->setVec3("offset", pos);
    api->glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    api->glLineWidth(2.0);
}

void RenderClimate()
{
    RenderSnow();
    RenderRain();
}



void RenderSnow()
{
    struct SnowParticle {
        float type;
        float life;
        float age;
        vec3 pos;
        vec3 dir;
    };
    static Program* TransformProgram;
    static Program* SnowRenderProgram;
    static Texture1D* RandomTexture;
    static Texture2D* snowTexture;
    static ParticleSystem* snowSystem = 0;
    static const int SnowLauncherNums = 5000;
    static const int SnowMaxNums = 100000;
    static const int SnowBufferSize = SnowMaxNums * sizeof(SnowParticle);
    static const int SnowRenderRadius = 20;
    static  Random rand;
    static Vao* vao[2];
    static Vbo* vbo[2];
    static bool IsSnow =1;
    static long long TotalTime = 3 * 60 * 1000;
    static long long Time = 0;
    if (!snowSystem)
    {
        snowTexture = new Texture2D("src/Snow/snow.png");
        //////////////////////////////////////
        vector<SnowParticle>SnowLauncher(SnowLauncherNums);
        float MulFac = 100.0;
        rand.SetSeed(QDateTime::currentMSecsSinceEpoch());
        for (auto& launcher : SnowLauncher)
        {
            launcher.age = 0;
            launcher.type = 0.0;
            launcher.life = rand.rand(100, 1000);
            launcher.pos = vec3(
                rand.rand(-SnowRenderRadius * MulFac, SnowRenderRadius * MulFac) / MulFac,
                -10000.0,
                rand.rand(-SnowRenderRadius * MulFac, SnowRenderRadius * MulFac) / MulFac
            );
        }
        /////////////////////////////////////
        const GLuint randTextureSize = 256;
        vector<vec3>dir(randTextureSize);
        for (auto&dir_:dir)
        {
         dir_.x = rand.rand(-80, 80);
         dir_.z = rand.rand(-80, 80);
         dir_.y = rand.rand(-200, -100);
         dir_=glm::normalize(dir_);
        }
        RandomTexture = new Texture1D(randTextureSize, GL_RGB16F, GL_RGB, GL_FLOAT, dir.data());
       //////////////////////////////////////////
        for (int i = 0; i < 2; ++i)
        {
            vao[i] = new Vao;
            vbo[i] = new Vbo(SnowBufferSize, 0);
            if (i==0)
                vbo[i]->setdata(0,SnowLauncherNums * sizeof(SnowParticle), SnowLauncher.data());
            vbo[i]->setpoint(0, GL_FLOAT, 3, sizeof(SnowParticle), 0);//type life age
            vbo[i]->setpoint(1, GL_FLOAT, 3, sizeof(SnowParticle),offsetof(SnowParticle,SnowParticle::pos));
            vbo[i]->setpoint(2, GL_FLOAT, 3, sizeof(SnowParticle),offsetof(SnowParticle, SnowParticle::dir));
            vao[i]->unbind();
        }
        /////////////////////////////////////////////
        snowSystem = new ParticleSystem(vao, vbo, SnowLauncherNums);
        /////////////////////////////////////////////
        TransformProgram = new Program("src/Shader/Snow/Transform.vert", "src/Shader/Snow/Transform.geom","", vector<string>{ "type","life","age","pos","dir"});
        SnowRenderProgram = new Program("src/Shader/Snow/render.vert", "src/Shader/Snow/render.frag");
        SnowRenderProgram->setUbo("VP", CameraUboBinding);
    }
    /////////////////////////////////////////判断今天是否应该下雪
    Time += RenderTime;
    if (Time / TotalTime)
    {
        Time = 0;
        IsSnow = rand.rand(0, 2) ? 1 : 0;
    }
    if (IsSnow == 0)return;
    ////////////////////////////////////////////判断人物所处的生物群落是否应该绘制雪
    static float blur[3][3] = {
        {0.0625,0.125,0.0625},
        {0.125,0.25,0.125},
        {0.0625,0.125,0.0625}
    };
    float val = 0.0;
    for (int x = 0; x < 3; ++x)
    {
        for (int z = 0; z < 3; ++z)
        {
            val += blur[x][z] * (BiomeTypeAround[x][z] == SNOWFIELD);
        }
    }
    if (val < 0.25)return;
    ///////////////////////////////////////////
    TransformProgram->bind();
    TransformProgram->setInt("random", 0);
    RandomTexture->bind(0);
    TransformProgram->setVec3("rolePos", controller->camera.position);
    TransformProgram->setFloat("deltaTime", RenderTime);
    TransformProgram->setFloat("randomFac", randomFloat());
    snowSystem->Update(GL_POINTS);
    /////////////////////////////////////////
    api->glEnable(GL_DEPTH_TEST);
    api->glEnable(GL_POINT_SPRITE);
    api->glEnable(GL_POINT_SMOOTH);
    api->glEnable(GL_PROGRAM_POINT_SIZE);
    api->glBindVertexArray(0);
    auto* RenderBuffer = snowSystem->GetCurrentRenderBuffer();
    RenderBuffer->bind();
    RenderBuffer->setpoint(0, GL_FLOAT, 3, sizeof(SnowParticle), offsetof(SnowParticle, SnowParticle::pos));
    SnowRenderProgram->bind();
    SnowRenderProgram->setInt("snow", 0);
    SnowRenderProgram->setVec3("eye",controller->camera.position);
    snowTexture->bind(0);
    snowSystem->Draw(GL_POINTS);
}

void RenderRain()
{
    struct RainParticle
    {
        float type;
        float life;
        float age;
        vec3 pos;
    };
    static Program* TransformProgram;
    static Program* RainRenderProgram;
    static Texture1D* RandomTexture;
    static Texture2D* rainTexture;
    static ParticleSystem* rainSystem = 0;
    static const int rainLauncherNums = 2000;
    static const int rainDropMaxNums = 100000;
    static const int RainBufferSize = rainDropMaxNums * sizeof(RainParticle);
    static const int RainRenderRadius = 20;
    static Random rand;
    static Vao* vao[2];
    static Vbo* vbo[2];
    static bool IsRain = 0;
    static long long TotalTime = 3 * 60 * 1000;
    static long long Time = 0;
    if (!rainSystem)
    {
        rainTexture = new Texture2D("src/Rain/rain.png");
        //////////////////////////////////////
        vector<RainParticle>RainLauncher(rainLauncherNums);
        float MulFac = 100.0;
        rand.SetSeed(QDateTime::currentMSecsSinceEpoch());
        for (auto& launcher : RainLauncher)
        {
            launcher.age = 0;
            launcher.type = 0.0;
            launcher.life = rand.rand(100, 1000);
            launcher.pos = vec3(
                rand.rand(-RainRenderRadius * MulFac, RainRenderRadius * MulFac) / MulFac,
                -10000.0,
                rand.rand(-RainRenderRadius * MulFac, RainRenderRadius * MulFac) / MulFac
            );
        }
        /////////////////////////////////////
        const GLuint randTextureSize = 256;
        const int MulFac2 = 10000;
        vector<float>randNum(randTextureSize);
        for (auto& num : randNum)
        {
            num = rand.rand(0, MulFac2) * 1.0 / MulFac2;
        }
        RandomTexture = new Texture1D(randTextureSize, GL_R16F, GL_RED, GL_FLOAT, randNum.data());
        //////////////////////////////////////////
        for (int i = 0; i < 2; ++i)
        {
            vao[i] = new Vao;
            vbo[i] = new Vbo(RainBufferSize, 0);
            if (i == 0)
                vbo[i]->setdata(0,rainLauncherNums * sizeof(RainParticle), RainLauncher.data());
            vbo[i]->setpoint(0, GL_FLOAT, 3, sizeof(RainParticle), 0);//type life age
            vbo[i]->setpoint(1, GL_FLOAT, 3, sizeof(RainParticle), offsetof(RainParticle, RainParticle::pos));
            vao[i]->unbind();
        }
        /////////////////////////////////////////////
        rainSystem = new ParticleSystem(vao, vbo, rainLauncherNums);
        /////////////////////////////////////////////
        TransformProgram = new Program("src/Shader/Rain/Transform.vert", "src/Shader/Rain/Transform.geom","",vector<string>{ "type", "life", "age", "pos"});
        RainRenderProgram = new Program("src/Shader/Rain/render.vert", "src/Shader/Rain/render.frag");
        RainRenderProgram->setUbo("VP", CameraUboBinding);
    }
    /////////////////////////////////////////////
    if (Time / TotalTime)
    {
        Time = 0;
        IsRain=rand.rand(0, 5) ? 0 : 1;
    }
    if (IsRain == 0)return;
    ////////////////////////////////////////////判断人物所处的生物群落是否应该绘制雨
    static float blur[3][3] = {
        {0.0625,0.125,0.0625},
        {0.125,0.25,0.125},
        {0.0625,0.125,0.0625}
    };
    float val = 0.0;
    for (int x = 0; x < 3; ++x)
    {
        for (int z = 0; z < 3; ++z)
        {
            val += blur[x][z] * (BiomeTypeAround[x][z] == FOREST||BiomeTypeAround[x][z]==RAINFOREST);
        }
    }
    if (val < 0.25)return;
    ///////////////////////////////////////////
    TransformProgram->bind();
    TransformProgram->setInt("random", 0);
    RandomTexture->bind(0);
    TransformProgram->setVec3("rolePos", controller->camera.position);
    TransformProgram->setFloat("deltaTime", RenderTime);
    TransformProgram->setFloat("randomFac", randomFloat());
    rainSystem->Update(GL_POINTS);
    /////////////////////////////////////////
    api->glEnable(GL_DEPTH_TEST);
    api->glEnable(GL_POINT_SPRITE);
    api->glEnable(GL_POINT_SMOOTH);
    api->glEnable(GL_PROGRAM_POINT_SIZE);
    auto* RenderBuffer = rainSystem->GetCurrentRenderBuffer();
    api->glBindVertexArray(0);
    RenderBuffer->bind();
    RenderBuffer->setpoint(0, GL_FLOAT, 3, sizeof(RainParticle), offsetof(RainParticle, RainParticle::pos));
    RainRenderProgram->bind();
    RainRenderProgram->setInt("rain", 0);
    RainRenderProgram->setVec3("eye", controller->camera.position);
    rainTexture->bind(0);
    rainSystem->Draw(GL_POINTS);
}

void UpdateCloudNoiseTexture()
{
    static float data[CloudTextureSize][CloudTextureSize];
    static PerlinNoiseGenerator generator(QDateTime::currentMSecsSinceEpoch());
    static const float CloudFreq=SectionBlockWidth*2;
    static const float TimeFreq=10*1000;
    static const uint fbm=4;
    static int Complete=0;
    static auto Fun=[&]{
        static const auto TimeBegin=QDateTime::currentMSecsSinceEpoch();
        auto time=QDateTime::currentMSecsSinceEpoch();
        double z=(time-TimeBegin)/TimeFreq;
        for(int i=0;i<CloudTextureSize;++i)
        {
            float x0=i/CloudFreq+z,x1=i/CloudFreq-z;
            for(int j=0;j<CloudTextureSize;++j)
            {
                data[i][j]=Lerp(generator.generate01(x0,j/CloudFreq+z,fbm),generator.generate01(x1,j/CloudFreq+z,fbm),0.3);
            }
        }
        Complete=2;
    };
    if(Complete==0)
    {
        Complete=1;
        threadPool->append(Fun);
    }
    else if(Complete==2){
        Complete=0;
        CloudTexture->bind();
        api->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CloudTextureSize, CloudTextureSize,GL_RED, GL_FLOAT, data);
        CloudTexture->unbind();
    }
}

void UpdateDay()
{
    static auto GameBeginDayTime=DayPercent;
    static unsigned long long MSecPerHour=30*1000;
    static auto MSecPerDay=MSecPerHour*24;
    static auto TimeBegin=QDateTime::currentMSecsSinceEpoch();
    auto CurTime=QDateTime::currentMSecsSinceEpoch()-TimeBegin;
    float fac=CurTime*1.0/MSecPerDay;
    DayPercent=GameBeginDayTime+fac;
    DayPercent=fmod(DayPercent,1.0);
    ////////////////////////////////////////////////计算太阳位置
    static float MaxY=sqrt(3.0)/2.0;
    static float SinMaxTheta=MaxY;
    static float CosMaxTheta=sqrt(1.0-SinMaxTheta*SinMaxTheta);
    static float SunBeginDegree=-PI/2;
    float SunRotateDegree=DayPercent*2*PI;
    float SunCurDegree=SunBeginDegree-SunRotateDegree;
    float x=cos(SunCurDegree);
    float z=sin(SunCurDegree);
    float y=z*SinMaxTheta;
    z=z*CosMaxTheta;
    SunPos=vec3(x,y,z);
    //////////////////////////////////////////////////计算全局光照
    GlobalBrightness=-cos(2*PI*DayPercent)+0.2;
}

void RenderGBuffer()
{
     static Vao* vao = 0;
     static Fbo MainWindow(((QOpenGLWidget*)PalettePtr)->defaultFramebufferObject());
     static Texture2DGeneral* screen[2];
     static Texture2DGeneral* NormalTexture;
     static Texture2DGeneral* PositionTexture;
     static Fbo* fbo[2];
     if (!vao)
        {
         vao = new Vao();
         GLfloat vertex[]{
         -1,1,0,1,
         1,1,1,1,
         -1,-1,0,0,
         1,-1,1,0
         };
         GLuint index[]{ 0,1,2,1,2,3 };
         auto*vbo = new Vbo(sizeof(vertex), vertex);
         vbo->setpoint(0, GL_FLOAT, 2, sizeof(float) * 4, 0);
         vbo->setpoint(1, GL_FLOAT, 2, sizeof(float) * 4, sizeof(float) * 2);
         auto*ebo = new Ebo(6, index);
         vao->unbind();
         ///////////////////////////////
         fbo[0] = new Fbo();
         screen[0] = GBuffer->GetBuffers()[0];
         fbo[0]->attachTexture(*screen[0], GL_COLOR_ATTACHMENT0);
         fbo[0]->setDrawBuffer(GL_COLOR_ATTACHMENT0);
         ///////////////////////////////
         fbo[1] = new Fbo();
         screen[1] = new Texture2DGeneral(GBuffer->GetBufferWidth(), GBuffer->GetBufferHeight(), GL_RGB16F, GL_RGB, GL_FLOAT, 0);
         fbo[1]->attachTexture(*screen[1], GL_COLOR_ATTACHMENT0);
         fbo[1]->setDrawBuffer(GL_COLOR_ATTACHMENT0);
         /////////////////////////////////////
         NormalTexture = GBuffer->GetBuffers()[2];
         PositionTexture = GBuffer->GetBuffers()[1];
        }
     /////////////////////////////////////
     screen[0]->fitViewport();
     api->glDisable(GL_DEPTH_TEST);
     vao->bind();
     /////////////////////////////////////开始后期处理
     ////////////////////////炫光
     static Program* BloomProgram = 0;
     static Fbo* BloomFbo[2];
     static Texture2DGeneral* BloomTexture[2];
     if (!BloomProgram)
     {
         BloomProgram = new Program("src/Shader/Bloom/Bloom.vert", "Src/shader/Bloom/Bloom.frag");

         BloomFbo[0] = fbo[1];
         BloomTexture[0] = screen[1];
         BloomFbo[1] = new Fbo();
         BloomTexture[1] = new Texture2DGeneral(GBuffer->GetBufferWidth(), GBuffer->GetBufferHeight(), GL_RGB16F, GL_RGB, GL_FLOAT, 0);
         BloomFbo[1]->attachTexture(*BloomTexture[1], GL_COLOR_ATTACHMENT0);
         BloomFbo[1]->setDrawBuffer(GL_COLOR_ATTACHMENT0);
     }
     fbo[1]->bind();
     bool isHorizon = 1;
     BloomProgram->bind();
     BloomProgram->setInt("screen", 0);
     BloomProgram->setInt("horizon", isHorizon);
     screen[0]->bind(0);
     api->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
     static const int BloomTime = 5;//必须为单数
     for (int i = 0; i < BloomTime; ++i)
     {
         BloomFbo[(i+1) % 2]->bind();
         isHorizon = !isHorizon;
         BloomProgram->setInt("horizon", isHorizon);
         BloomTexture[i % 2]->bind(0);
         api->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
     }
    //////////////混合+HDR
     static Program* HDR = 0;
     if (!HDR)
     {
         HDR = new Program("src/Shader/HDR/HDR.vert", "src/Shader/HDR/HDR.frag");
     }
     fbo[1]->bind();
     HDR->bind();
     HDR->setFloat("exposure", Exposure);
     HDR->setInt("screen", 0);
     screen[0]->bind(0);
     HDR->setInt("bloom", 1);
     BloomTexture[1]->bind(1);
     api->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
     ////////////////ssao 由于Gbuffer未保存TBN矩阵,故不进行该处理
     ////////////////延时光照
     //////////////////////////////////////
     MainWindow.bind();
     api->glViewport(0, 0, WindowWidth, WindowHeight);
     api->glEnable(GL_DEPTH_TEST);
     Texture2D::DrawTexture2D(screen[1]->getid(),glm::clamp(GlobalBrightness,0.2f,1.0f));
}

void InitBag(vector<vector<Pixel> > &BlockImage, uint rowCnt, uint colCnt)
{
    ///////////////初始化所有物品数量
    int ItemInitNum = 1;
    for (int i = 1; i <256; ++i)
        BagItems[i] = ItemInitNum;
    /////////////////////////////////////////////获得转换表
    decltype(TransitionTable) table;
    table.insert({ vector<int>{TallGrass,DIRT},{GRASS,1000} });
    //对转换表所有的元素重新排序
    for (auto itr = table.begin(); itr != table.end(); ++itr)
    {
        auto transition = itr->first;
        sort(transition.begin(), transition.end());
        TransitionTable.insert({ transition,itr->second });
    }
    /////////////////////////////////////////////获得物品的图标
    vector<BYTE>ImageData(BlockImage.size()*BlockImage.front().size()*sizeof(Pixel));
    for(int i=0,idx=0;i<BlockImage.size();++i,idx+=BlockImage.front().size()*sizeof(Pixel))
    {
        memcpy(&ImageData[idx],BlockImage[i].data(),BlockImage.front().size()*sizeof(Pixel));
    }
    QImage blocks((uchar*)ImageData.data(),BlockImage.front().size(),BlockImage.size(),QImage::Format_RGBA8888);
    ItemIcons=decltype(ItemIcons)(BagRow * BagColumn, 0);
    int image_width = blocks.width(), image_height = blocks.height();
    int itemWidth = image_width / colCnt;
    int itemHeight = image_height / rowCnt;
    for (auto&ele:BlockTexCoords)
    {
        uint type=ele.first;
        vec2 offset=ele.second.front().offset[0];
        uint x=offset.x*image_width,y=offset.y*image_height;
        auto itemPicture = blocks.copy(x, y, itemWidth, itemHeight);
        ItemIcons[type] = new QIcon(QPixmap::fromImage(itemPicture));
    }
    ////////////////初始化工作台
    WorkSpaceBoardInit();
    ////////////////初始化物品框
    InitItemBoard();
    ///////////////初始化人物背包
    InitMyBag();
}

void WorkSpaceBoardInit()
{
    workSpaceBoard = new WorkSpaceBoard((QWidget*)PalettePtr);
    workSpaceBoard->setStyleSheet("background-color: rgba(255,255,255,120);");
    workSpaceBoard->focusWidget();
    workSpaceBoard->hide();
    WorkSpaceBoardSource[0]=workSpaceBoard->ui->source0;
    WorkSpaceBoardSource[1]=workSpaceBoard->ui->source1;
    WorkSpaceBoardSource[2]=workSpaceBoard->ui->source2;
    WorkSpaceBoardSource[3]=workSpaceBoard->ui->source3;
    WorkSpaceBoardSource[4]=workSpaceBoard->ui->source4;
    WorkSpaceBoardSource[5]=workSpaceBoard->ui->source5;
    WorkSpaceBoardSource[6]=workSpaceBoard->ui->source6;
    WorkSpaceBoardSource[7]=workSpaceBoard->ui->source7;
    WorkSpaceBoardSource[8]=workSpaceBoard->ui->source8;
    ////////////////////////////////////
    auto& ui = *workSpaceBoard->ui;
    ////////////////绑定退出事件
    auto& exitRadioButton = ui.exit;
    QApplication::connect(ui.exit, &QRadioButton::clicked, [&]() {
        exitRadioButton->setChecked(0);
        workSpaceBoard->hide();
        ForceMouse();
     });
    ///////////////////绑定target按钮
    auto&targetButton = ui.target;
    targetButton->setIcon(ItemIcons[0], 0);
    QApplication::connect(targetButton, &BagButton::clicked, [&]() {
        auto* numberLabel = ui.number;
        int id = targetButton->id;
        if (id== 0)return;
        int r = id / BagColumn, c = id % BagColumn;
        auto* widget = ui.Bag->cellWidget(r, c);
        auto* child = widget->findChild<QLabel*>(QString("number"));
        int nums = child->text().toInt();
        int bonus = numberLabel->text().toInt();
        //////////////////////////////
        BagItems[id] += bonus;
        //////////////////////////////
        child->setText(to_string(nums + bonus).c_str());
        targetButton->setIcon(ItemIcons[0], 0);
        numberLabel->setText(to_string(0).c_str());
        ////////////////////////////////
        ////////////////清空source
        for (auto& source :WorkSpaceBoardSource)
        {
            int id = source->id;
            if (id == 0)continue;
            BagItems[id]--;
            source->setIcon(ItemIcons[0], 0);
        }
        });
    //////////////////设置target的图标和数量
    static auto FindExchangeTable=[&](vector<int>& id)->pair<int,int>
    {
        pair<int,int> res = { 0,0 };
        /////////////////////////////////////
        //排序所有的id
        sort(id.begin(), id.end());
        //从转换表里面查找对于的转换
        auto itr = TransitionTable.find(id);
        if (itr != TransitionTable.end())res = itr->second;
        ////////////////////////////////////
        return res;
    };
    static auto setTaregtInfo = [&]() {
        static auto& icons = ItemIcons;
        vector<int>id;
        for (auto& source : WorkSpaceBoardSource)
        {
            if (source->id == 0)break;
            id.push_back(source->id);
        }
        auto data = FindExchangeTable(id);
        ui.target->setIcon(icons[data.first], data.first);
        ui.number->setText(to_string(data.second).c_str());
        };
    //////////////////设置source按钮
    auto& source = WorkSpaceBoardSource;
    int index = 0;
    for (auto& ele : source)
    {
        ele->setIcon(ItemIcons[0], 0);
        QApplication::connect(ele, &BagButton::clicked, [=]() {
            static auto* icons = &ItemIcons;
            if (ele->id == 0)return;
            int r = ele->id / BagRow, c = ele->id % BagColumn;
            auto* widget = ui.Bag->cellWidget(r, c);
            auto* child = widget->findChild<QLabel*>(QString("number"));
            int nums = child->text().toInt();
            child->setText(to_string(nums + 1).c_str());
            ele->setIcon((*icons)[0], 0);
            //////////////////设置target
            setTaregtInfo();
            });

    }
    ////////////////设置物品的图片
    auto& Bag = ui.Bag;
    Bag->setSelectionMode(QAbstractItemView::SingleSelection);
    Bag->verticalHeader()->hide();
    Bag->horizontalHeader()->hide();
    Bag->setRowCount(BagRow);
    Bag->setColumnCount(BagColumn);
    for (int row = 0; row < BagRow; ++row)
    {
        for (int column = 0; column < BagColumn; ++column)
        {
            QWidget* widget = new QWidget(Bag);
            QVBoxLayout* layout = new QVBoxLayout(widget);
            widget->setLayout(layout);
            /////////////////////////
            int index = row * BagColumn + column;
            BagButton* image = new BagButton();
            QLabel* NumbersLabel = new QLabel(to_string(BagItems[index]).c_str());
            image->setObjectName(QString("icon"));
            NumbersLabel->setObjectName(QString("number"));
            image->setIcon(ItemIcons[0], 0);
            if (index < ITEM_NUMS)image->setIcon(ItemIcons[index], index);
            layout->addWidget(image);
            layout->addWidget(NumbersLabel);
            Bag->setCellWidget(row, column, widget);
            auto item = Bag->item(row, column);
            QApplication::connect(image, &BagButton::clicked, [&,image, NumbersLabel]() {
                int id = image->id;
                int nums = NumbersLabel->text().toInt();
                if (nums == 0)return;
                for (auto& source : WorkSpaceBoardSource)
                {
                    if (source->id == 0)
                    {
                        source->setIcon(ItemIcons[id],id);
                        NumbersLabel->setText(to_string(nums - 1).c_str());
                        break;
                    }
                }
                ////////////////////检测是否有对应的替换
                setTaregtInfo();
                });
        }
    }
}

void InitItemBoard()
{
    itemBoard = new ItemBoard((QWidget*)PalettePtr);
    itemBoard->setStyleSheet("background-color: rgba(255,255,255,120);");
    itemBoard->setFocusPolicy(Qt::NoFocus);
    ItemBoardAllItems[0]=itemBoard->ui->item0;
    ItemBoardAllItems[1]=itemBoard->ui->item1;
    ItemBoardAllItems[2]=itemBoard->ui->item2;
    ItemBoardAllItems[3]=itemBoard->ui->item3;
    ItemBoardAllItems[4]=itemBoard->ui->item4;
    ItemBoardAllItems[5]=itemBoard->ui->item5;
    ItemBoardAllItems[6]=itemBoard->ui->item6;
    ItemBoardAllItems[7]=itemBoard->ui->item7;
    ItemBoardAllItems[8]=itemBoard->ui->item8;
    ItemBoardAllItemNum[0]=itemBoard->ui->num0;
    ItemBoardAllItemNum[1]=itemBoard->ui->num1;
    ItemBoardAllItemNum[2]=itemBoard->ui->num2;
    ItemBoardAllItemNum[3]=itemBoard->ui->num3;
    ItemBoardAllItemNum[4]=itemBoard->ui->num4;
    ItemBoardAllItemNum[5]=itemBoard->ui->num5;
    ItemBoardAllItemNum[6]=itemBoard->ui->num6;
    ItemBoardAllItemNum[7]=itemBoard->ui->num7;
    ItemBoardAllItemNum[8]=itemBoard->ui->num8;
    ///////////////////////////
    auto& items =ItemBoardAllItems;
    auto& nums = ItemBoardAllItemNum;
    for (int i=0;i<9;++i)
    {
        auto& item = items[i];
        auto& num = nums[i];
        item->setIcon(ItemIcons[0], 0);
        num->setText("0");
        QApplication::connect(item, &BagButton::clicked,[&](){
        item->setIcon(ItemIcons[MyBagCurrentSelectedItem], MyBagCurrentSelectedItem);
        num->setText(to_string(BagItems[MyBagCurrentSelectedItem]).c_str());
        });
    }
    //第一个物品为工作台
    items[0]->setIcon(ItemIcons[WorkSpace],WorkSpace);
    items[0]->setStyleSheet("QPushButton { border: 2px solid black; }");
    nums[0]->setText(to_string(BagItems[WorkSpace]).c_str());
}

void UpdateItemBoard()
{
    static BagButton**bagbutton=ItemBoardAllItems;
    static QLabel**num=ItemBoardAllItemNum;
    for(int i=0;i<9;++i)
    {
        int id=bagbutton[i]->id;
        int cnt=BagItems[id];
        num[i]->setText(QString::number(cnt));
        if(cnt==0)bagbutton[i]->setIcon(0,0);
    }
}

void InitMyBag()
{
    MyBag = new Bag((QWidget*)PalettePtr);
    MyBag->setStyleSheet("background-color: rgba(255,255,255,120);");
    MyBag->setFocusPolicy(Qt::NoFocus);
    MyBag->hide();
    /////////////////////////////
    auto& bag = MyBag->ui->bag;
    bag->setRowCount(BagRow);
    bag->setColumnCount(BagColumn);
    ////////////////////////////
    for (int r = 0; r < BagRow; ++r)
    {
        for (int c = 0; c < BagColumn; ++c)
        {
            QWidget* widget = new QWidget();
            QVBoxLayout* layout = new QVBoxLayout();
            BagButton* bagbutton = new BagButton;
            QLabel* label = new QLabel;
            widget->setLayout(layout);
            layout->addWidget(bagbutton);
            layout->addWidget(label);
            bag->setCellWidget(r,c,widget);
            bagbutton->setObjectName(QString("icon"));
            label->setObjectName(QString("num"));
            int id = r * BagColumn + c;
            id = id >= ITEM_NUMS?0:id;
            label->setText(to_string(BagItems[id]).c_str());
            bagbutton->setIcon(ItemIcons[id], id);
            ////////////////绑定点击事件
            QApplication::connect(bagbutton, &BagButton::clicked, [&,bagbutton](){
                MyBagCurrentSelectedItem = bagbutton->id;
            });
        }
    }
}
