#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "matrix4.h"
#include "geometrymaker.h"

#include "Program.h"
#include "Uniform.h"
#include "BufferObject.h"
#include "LuaConfig.h"
#include "Attribute.h"

#include "Timer.h"
#include <iterator>
#include <math.h>
using namespace std;
using VertexPNBuffer = detail::GlBufferObject<GL_ARRAY_BUFFER,VertexPN>;

//global pointers to on-stack objects
VertexPNBuffer *vbo;
IndexBuffer * ibo;
Program* program;
UniformMatrix4fv *modelView, *projection, *normalMat;
Uniform3f *color;
Attribute * normal, *position;
LuaConfig * config;


Timer timer;

class EntityHierarchy {
    void onDraw();
};
void idle(){
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float colMajorMat[16];
    static LuaTable luaProjection = config->getLuaTable("projection"),
                    luaEyePosition = config->getLuaTable("eye");

    double fovy=luaProjection.get<double>(0);
    double aspectRatio=luaProjection.get<double>(1);
    double zNear=luaProjection.get<double>(2);
    double zFar=luaProjection.get<double>(3);
    double eye_x = luaEyePosition.get<double>(0);
    double eye_y = luaEyePosition.get<double>(1);
    double eye_z = luaEyePosition.get<double>(2);

#define offset(T,e) ((void*)&(((T*)0)->e))
    //main object
    Matrix4 p= Matrix4::makeProjection(fovy,aspectRatio,zNear,zFar);
    p.writeToColumnMajorMatrix(colMajorMat);
    projection->setValue(1,false,colMajorMat);

    Matrix4 obj = Matrix4::makeZRotation(100.0/1000000*timer.runningTime());
    Matrix4 eye=Matrix4::makeTranslation(Cvec3(eye_x,eye_y,eye_z));
    Matrix4 mvm = inv(eye)*obj;

    mvm.writeToColumnMajorMatrix(colMajorMat);
    modelView->setValue(1,false,colMajorMat);

    Matrix4 n = normalMatrix(mvm);
    n.writeToColumnMajorMatrix(colMajorMat);
    normalMat->setValue(1,false,colMajorMat);
    vbo->bind();
    glVertexAttribPointer(position->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,p));
    glVertexAttribPointer(normal->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,n));
    ibo->bind();
    color->setValue(1.0,1.0,.0);//yellow
    glDrawElements(GL_TRIANGLES,ibo->size(),GL_UNSIGNED_SHORT,0);
    
    //subobject1
    Matrix4 relativeTransform = Matrix4::makeTranslation(Cvec3(1.0,1.0,-2.0));
    auto mvm1 = inv(eye)*obj*relativeTransform;
    mvm1.writeToColumnMajorMatrix(colMajorMat);
    modelView->setValue(1,false,colMajorMat);
    n = normalMatrix(mvm1);
    n.writeToColumnMajorMatrix(colMajorMat);
    normalMat->setValue(1,false,colMajorMat);
    vbo->bind();
    glVertexAttribPointer(position->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,p));
    glVertexAttribPointer(normal->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,n));
    ibo->bind();
    color->setValue(1.0,.0,.0);//red
    glDrawElements(GL_TRIANGLES,ibo->size(),GL_UNSIGNED_SHORT,0);

    //subobject2
    Matrix4 relativeTransform2 = Matrix4::makeTranslation(Cvec3(.0,2.0*sin(10.*timer.runningTime()/1000000),-7.0));
    mvm = inv(eye)*obj*relativeTransform2;
    mvm.writeToColumnMajorMatrix(colMajorMat);
    modelView->setValue(1,false,colMajorMat);
    n = normalMatrix(mvm);
    n.writeToColumnMajorMatrix(colMajorMat);
    normalMat->setValue(1,false,colMajorMat);
    vbo->bind();
    glVertexAttribPointer(position->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,p));
    glVertexAttribPointer(normal->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,n));
    ibo->bind();
    color->setValue(.0,1.0,.0);//green
    glDrawElements(GL_TRIANGLES,ibo->size(),GL_UNSIGNED_SHORT,0);

#undef offset
	glutSwapBuffers();
}

void reshape(int w,int h){
    glViewport(0,0,w,h);
}

void init(int *argc, char* argv[])
{
    
    glutInit(argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("Simple");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glReadBuffer(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClearDepth( -100.0);
    glClearColor(0.3,0.3,0.3,1.);
    glewInit();
}

int main(int argc, char* argv[])
{
    init(&argc,argv);
    //on-stack objects, won't be disposed until program ends
    Program program_( CURRENT_DIR "/vertex.glsl",CURRENT_DIR "/fragment.glsl");
    LuaConfig config_(CURRENT_DIR "/config.lua");
    Attribute position_( &program_,"position");
    Attribute normal_( &program_,"normal");
    UniformMatrix4fv modelView_(&program_, "mvm"),
        projection_(&program_,"p"),
        normalMat_(&program_,"normalMat");
    Uniform3f color_(&program_,"uColor");

    int nvtx,nidx;
    getCubeVbIbLen(nvtx,nidx);
    std::vector<VertexPN>verts(nvtx);
    std::vector<unsigned short>indices(nidx);
    makeCube(2.0,verts.begin(),indices.begin());
    
    VertexPNBuffer vbo_(verts.data(),verts.size());
    IndexBuffer ibo_(indices.data(),indices.size());
    

    program = &program_;
    position = &position_;
    normal = &normal_;
    modelView = &modelView_;
    projection = &projection_;
    normalMat = & normalMat_;
    vbo = &vbo_;
    ibo = &ibo_;
    config = &config_;
    color = &color_;

    program->useThis();
    normal->enable();
    position->enable();
    timer.start();
    glutMainLoop();
	return 0;
}


