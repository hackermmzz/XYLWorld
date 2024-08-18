#include "palette.h"

Palette::Palette(QWidget *parent) : QOpenGLWidget(parent)
{
    /////允许接受键盘事件
    setFocusPolicy(Qt::StrongFocus);
    //////鼠标移动事件
    setCursor(Qt::BlankCursor);
    setMouseTracking(true);
}

void Palette::initializeGL()
{
    initializeOpenGLFunctions();
    SetOpenGLApi(this);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    /////////////////////////////////////初始化
    PalettePtr=this;
    GameInit();
}

void Palette::resizeGL(int w, int h)
{
    api->glViewport(0, 0, w, h);
    WindowWidth=w;
    WindowHeight=h;
    //////////////////////////////////////////
    PersPective=glm::perspective(glm::radians(PersPectiveFov),w*1.0/h,PersPectiveNearDis,PersPectiveFarDis);
    //////////////////////////////////////////
    {
        static float relativePosx = 0.05, relativePosy = 0.9, relativeWidth = 0.1*9, relativeHeight =0.1;
        int ItemBoardWidth = relativeWidth * WindowWidth, ItemBoardHeight = relativeHeight * WindowHeight;
        int ItemBasePosX = relativePosx * WindowWidth, ItemBasePosyY = relativePosy * WindowHeight;
        itemBoard->move(ItemBasePosX, ItemBasePosyY);
        itemBoard->resize(ItemBoardWidth, ItemBoardHeight);
    }
    ///////////////////////////////////////////
    {
        //更新工作台大小
        static float relativePosxy = 0.1, relativeSize = 0.8;
        float widgetWidth = relativeSize / BagColumn * WindowWidth;
        float widgetHeight = relativeSize / BagRow * WindowWidth;
        workSpaceBoard->move(relativePosxy * WindowWidth, relativePosxy * WindowHeight);
        workSpaceBoard->resize(relativeSize * WindowWidth, relativeSize * WindowHeight);
        auto& Bag = workSpaceBoard->ui->Bag;
        for (int r = 0; r < Bag->rowCount(); ++r)
        {
            Bag->verticalHeader()->setSectionResizeMode(r, QHeaderView::Fixed);
            Bag->verticalHeader()->resizeSection(r, widgetHeight);  // 设置行高为50像素
            for (int c = 0; c < Bag->columnCount(); ++c)
            {
                Bag->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Fixed);
                Bag->horizontalHeader()->resizeSection(c, widgetWidth);  // 设置行高为50像素
            }
        }
        //更新背包大小
        MyBag->move(relativePosxy * WindowWidth, relativePosxy * WindowHeight);
        MyBag->resize(relativeSize * WindowWidth, relativeSize * WindowHeight);
        MyBag->ui->bag->move({0,0});
        MyBag->ui->bag->resize(MyBag->size());
        auto& bag = MyBag->ui->bag;
        for (int r = 0; r < bag->rowCount(); ++r)
        {
            bag->verticalHeader()->setSectionResizeMode(r, QHeaderView::Fixed);
            bag->verticalHeader()->resizeSection(r, widgetHeight);  // 设置行高为50像素
            for (int c = 0; c < bag->columnCount(); ++c)
            {
                bag->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Fixed);
                bag->horizontalHeader()->resizeSection(c, widgetWidth);  // 设置行高为50像素
            }
        }
    }
}

void Palette::paintGL()
{
    auto RenderBegin=QDateTime::currentMSecsSinceEpoch();
   ///////////////////////////////////////////
    api->glEnable(GL_DEPTH_TEST);
    api->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    ///////////////////////////////////////////更新所有定时器
    UpdateAllTimers();
    //////////////////////////////////////////
    GameRender();
    glFinish();
    update();
    ///////////////////////////////////////////
    auto RenderEnd=QDateTime::currentMSecsSinceEpoch();
    RenderTime=RenderEnd-RenderBegin;
    RenderTime=RenderTime?RenderTime:1;
}

void Palette::keyPressEvent(QKeyEvent *event)
{
    auto key=event->key();
    auto&camera=((Controller*)controller)->camera;
    if(key==Qt::Key_W){
        controller->camera.processKeyBoard(FORWARD,1.0);;
        RoleMoveDirection|=FORWARD;
        rolecontroller.mvfsm.SwitchState(MOVE);
    }
    else if(key==Qt::Key_S){
        RoleMoveDirection|=BACKWARD;
        rolecontroller.mvfsm.SwitchState(MOVE);
    }
    else if(key==Qt::Key_A){
        RoleMoveDirection|=LEFT;
        rolecontroller.mvfsm.SwitchState(MOVE);
    }
    else if(key==Qt::Key_D){
        RoleMoveDirection|=RIGHT;
        rolecontroller.mvfsm.SwitchState(MOVE);
    }
    else if(key==Qt::Key_Space)
    {
        rolecontroller.jdfsm.SwitchState(JUMP);
    }
    else if(key==Qt::Key_F2)(++CameraMode)%=2;
    else if(key==Qt::Key_F3)
    {
        if(LimitMouse){
            ExposureAdjust->show();
            ReleaseMouse();
        }
        else{
            ExposureAdjust->hide();
            ForceMouse();
        }
    }
    else if(key>=Qt::Key_1&&key<=Qt::Key_9)
    {
        auto& items = ItemBoardAllItems;
        int id=key-Qt::Key_1;
        /////////先将之前的框去掉
        items[ItemBoardCurrentSelectedItem]->setStyleSheet("");
        items[id]->setStyleSheet("QPushButton { border: 2px solid black; }");
        ItemBoardCurrentSelectedItem = id;
    }
    else if(key==Qt::Key_E){
        static bool is_show = 1;
        /////////将之前选中的item设置为0
        MyBagCurrentSelectedItem = 0;
        DrawBag(is_show);
        is_show ^=1;
    }
}

void Palette::keyReleaseEvent(QKeyEvent *event)
{
    auto key=event->key();
    if(key==Qt::Key_Escape){
       auto ret=QMessageBox::warning(this,"Warning","Do you want to exit the game?",QMessageBox::Yes|QMessageBox::No);
       if(ret==QMessageBox::Yes)exit(0);
    }
    else if(key==Qt::Key_Tab)
        ExChangeTheWorld();
    else if(key==Qt::Key_F1)
       ShowGameInfo^=1;
    else if(key==Qt::Key_W||key==Qt::Key_A||key==Qt::Key_S||key==Qt::Key_D)
    {
        RoleMoveDirection&=0;
        rolecontroller.mvfsm.SwitchState(STOP);
    }
}

void Palette::mouseMoveEvent(QMouseEvent *event)
{
    if(!LimitMouse)return;
    static QPoint oldPos=event->globalPos();
    QPoint curPos=event->globalPos();
    QPoint offset=curPos-oldPos;
    //限制鼠标必须在屏幕内部
    auto globalPos=this->mapToGlobal(this->geometry().topLeft());
    auto centerGlobalPos=this->mapToGlobal(this->geometry().center());
    QCursor::setPos(centerGlobalPos.x(),centerGlobalPos.y());
    oldPos=centerGlobalPos;
    RECT mainRect; //windef.h中被定义
    mainRect.left = (LONG)globalPos.x();
    mainRect.right = (LONG)globalPos.x()+width();
    mainRect.top = (LONG)globalPos.y();
    mainRect.bottom = (LONG)globalPos.y()+height();
    ClipCursor(&mainRect);
    //////////////////////////
    controller->camera.processMouseMovement(offset.x(),-offset.y(),1.0);
}

void Palette::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        rolecontroller.buildfsm.SwitchState(DESTROY_BLOCK);
    else if(event->button()==Qt::RightButton)
         rolecontroller.buildfsm.SwitchState(FUNCTION_BLOCK);
}

void Palette::wheelEvent(QWheelEvent* event)
{
    if(CameraMode==0)return;
    static const float div = 360.0;
    static const float RealCameraMaxDistance=20.0;
    auto delta = event->angleDelta().y()/div;
    auto temp = RealCameraDistance;
    RealCameraDistance -= delta;
    if(RealCameraDistance<0.0)RealCameraDistance=0.0;
    else if(RealCameraDistance>RealCameraMaxDistance)RealCameraDistance=RealCameraMaxDistance;
}

void UpdateAllTimers()
{
    LoadSectionFaceTimer->start();
    GameUpdateTimer->start();
}

void ExChangeTheWorld()
{
    static vector<QString>AllStarNames;
    if(AllStarNames.empty())
    {
        QFile file("src/StarNames/StarNames.txt");
        if(!file.open(QIODevice::ReadOnly))
            exit(0);//没有这个文件谁也别想玩这个游戏!
        char data[1024*1024];
        while(file.readLine(data,1024*1024)!=-1)
        {
            AllStarNames.push_back(QString(data));
        }
        file.close();
    }
    auto&&ret=EnrtyTheWorldAccordingTheInputSeed();
   if(ret.first) QMessageBox::warning(0,"Tips",QString("Welcome to the star named ")+AllStarNames[uint(ret.second)%AllStarNames.size()]+QString(" !"),QMessageBox::Ok);
}


void DrawBag(bool isdraw)
{
    if (isdraw)
    {
        ////////////////////////////设置表格里面的元素
        for (int i = 1; i < ITEM_NUMS; ++i)
        {
            int r = i / BagColumn, c = i % BagColumn;
            auto* widget = MyBag->ui->bag->cellWidget(r, c);
            auto* NumLabel = widget->findChild<QLabel*>("num");
            NumLabel->setText(to_string(BagItems[i]).c_str());
        }
        ///////////////////////////
        ReleaseMouse();
        MyBag->show();
    }
    else
    {
        ForceMouse();
        MyBag->hide();
    }
}
