#include "model.h"

Model::Model(const char *file)
{
    vao=0;
    vbo=0;
    ebo=0;
    /////////////////////////////////////
    attrib_t attr;
    vector<shape_t> shape;
    vector<material_t> material;
    string err;
    auto ret=LoadObj(&attr,&shape,&material,&err,file);
    if(!ret)
    {
        qDebug()<<"Load "<<file<<" fail!";
        return;
    }
    if(err.size()){
        qDebug()<<err.c_str();
    }
    ////////////////////////////////////
    vao=new Vao;
    vbo=new Vbo(attr.vertices.size()*sizeof(GLfloat),attr.vertices.data());
    vbo->setpoint(0,GL_FLOAT,3,sizeof(vec3),0);
    vector<uint>indices;
    //开始读取索引
    for (size_t s = 0; s < shape.size(); s++) {
      size_t index_offset = 0;
      for (size_t f = 0; f < shape[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = size_t(shape[s].mesh.num_face_vertices[f]);
        for (size_t v = 0; v < fv; v++) {
          auto idx = shape[s].mesh.indices[index_offset + v];
          indices.push_back(idx.vertex_index);
        }
        index_offset += fv;
      }
    }
    ebo=new Ebo(indices.size(),indices.data());
    VertexCnt=indices.size();
    vao->unbind();
}

void Model::Render()
{
    vao->bind();
    api->glDrawElements(GL_TRIANGLES,VertexCnt,GL_UNSIGNED_INT,0);
    vao->unbind();
}

Model::~Model()
{
 if(vao)delete vao;
 if(vbo)delete vbo;
 if(ebo)delete ebo;
}
