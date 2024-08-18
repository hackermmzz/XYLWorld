#ifndef HEADER_H
#define HEADER_H
#include<qopenglwidget.h>
#include<qopenglfunctions_4_5_core.h>
#include"QDateTime"
#include"QDebug"
#include"glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
////////////////////////////////
using glm::mat4;
using glm::mat3;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using ULL=unsigned long long;
using LL=long long;
////////////////////////////////
#define OPENGLAPI QOpenGLFunctions_4_5_Core
extern OPENGLAPI*api;
void SetOpenGLApi(OPENGLAPI* api_);
////////////////////////////////
const double PI=acos(-1.0);
////////////////////////////////
#endif // HEADER_H
