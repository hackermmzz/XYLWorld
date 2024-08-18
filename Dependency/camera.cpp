#include "camera.h"
void Camera::update()
{

    //////////////////////////////
    double pitch_r=pitch*PI/180;
    double yaw_r=yaw*PI/180;
    front.x=cos(pitch_r)*cos(yaw_r);
    front.y=sin(pitch_r);
    front.z=cos(pitch_r)*sin(yaw_r);
    front=glm::normalize(front);
    //////////////////////////////
    right=glm::normalize(glm::cross(front,worldup));
    up=glm::normalize(glm::cross(right,front));
}

Camera::Camera(const vec3 &position_, const vec3 &worldup_,double pitch_,double yaw_,double movespeed_)
{
    position=position_;
    worldup=glm::normalize(worldup_);
    pitch=pitch_;
    yaw=yaw_;
    movespeed=movespeed_;
    update();
}

void Camera::processMouseMovement(double xoffset, double yoffset, double sensibility)
{
    pitch+=yoffset*sensibility;
    if(pitch<-90.0)pitch=-90.0;
    else if(pitch>90.0)pitch=90.0;
    yaw+=xoffset*sensibility;
    update();
}

void Camera::processKeyBoard(CAMERA_MOVEMENT movement, double deltatime)
{
    auto temp=vec3(deltatime*movespeed);
    switch(movement)
    {
    case FORWARD:
        position+=front*temp;
        break;
    case BACKWARD:
        position-=front*temp;
        break;
    case LEFT:
        position-=right*temp;
        break;
    case RIGHT:
        position+=right*temp;
        break;
    }
}

mat4 Camera::getMatrix()
{
    return glm::lookAt(position,position+front,up);
}

Camera::~Camera()
{

}
