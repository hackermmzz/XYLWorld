#include "controller.h"

Controller::Controller(const vec3 &InitPos):camera(InitPos,vec3{0,1,0},0,-90.0,RoleMoveSpeed),role(InitPos,ControllerModel)
{
    ubo=new Ubo(sizeof(mat4)*3,0);
    ubo->bindBase(CameraUboBinding);
    ubo->unbind();
}

void Controller::updateCamera()
{
   static float data[1024];
   static const uint MatSize=sizeof(mat4);
   static const float EPS=std::numeric_limits<float>::epsilon();
   //////////////////////////////////////
   auto camera_pos =camera.position;
   if(CameraMode==1){
       auto finalPos = camera_pos;
       auto temp = RealCameraDistance;
       for (;temp >= 0;temp-=1)
       {
           finalPos= camera_pos-camera.front * vec3(temp,temp,temp);
           if (!JudgeBlockCanBarry(GetBlock(floor(finalPos.x), floor(finalPos.y), floor(finalPos.z))))
               break;
       }
       if (temp>0)camera.position = finalPos;
   }
   View=camera.getMatrix();
   auto vp=PersPective*View;
   memcpy(data,value_ptr(View),MatSize);
   memcpy(&data[16],value_ptr(PersPective),MatSize);
   memcpy(&data[32],value_ptr(vp),MatSize);
   ubo->bind();
   ubo->setdata(0,MatSize*3,data);
   ubo->unbind();
   camera.position=camera_pos;
}

void Controller::setPos(const glm::vec3 &pos)
{
        camera.position=pos;
}

