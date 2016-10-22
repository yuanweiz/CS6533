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

//Some Intialization can only be started after glewInit() and glutInit()
//are called, so I can't put these objects into global static scope.
//One possible workaround is to use
//global pointers to main()'s on-stack objects, which won't be disposed
//until the whole program exits.
VertexPNBuffer *vbo;
IndexBuffer * ibo;
Program* program;
UniformMatrix4fv *modelView, *projection, *normalMat;
Uniform3f *color;
Attribute * normal, *position;
LuaConfig * config;
Timer timer;

void idle(){
    glutPostRedisplay();
}

void drawCube(const Matrix4& modelMatrix ){
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
    Matrix4 p= Matrix4::makeProjection(fovy,aspectRatio,zNear,zFar);
    p.writeToColumnMajorMatrix(colMajorMat);
    projection->setValue(1,false,colMajorMat);

    //set eye frame
    //Matrix4 eye=Matrix4::makeTranslation(Cvec3(eye_x,eye_y,eye_z));
    Matrix4 eye=lookFrom(eye_x,eye_y,eye_z,0,1,0);
    Matrix4 modelViewMatrix = inv(eye)* modelMatrix;
    modelViewMatrix.writeToColumnMajorMatrix(colMajorMat);
    modelView->setValue(1,false,colMajorMat);

    //set normalMatrix
    Matrix4 n = normalMatrix(modelViewMatrix);
    n.writeToColumnMajorMatrix(colMajorMat);
    normalMat->setValue(1,false,colMajorMat);

    //draw
    vbo->bind();
#define offset(T,e) ((void*)&(((T*)0)->e))
    glVertexAttribPointer(position->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,p));
    glVertexAttribPointer(normal->get(), 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offset(VertexPN,n));
#undef offset
    ibo->bind();
    glDrawElements(GL_TRIANGLES,ibo->size(),GL_UNSIGNED_SHORT,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //main object modelView
    Matrix4 mv = Matrix4::makeZRotation(100.0/1000000*timer.runningTime());
    color->setValue(1.0,1.0,.0);//yellow
    drawCube(mv);

    //subobject1
    Matrix4 t1 = Matrix4::makeTranslation(Cvec3(.0,.0,-3.0));
    auto mv1 = t1 * mv;
    color->setValue(1.0,.0,.0);//red
    drawCube(mv1);

    //subobject2
    double angle1=10.*timer.runningTime()/1000000;
    Matrix4 t2 = Matrix4::makeTranslation(Cvec3(.0,2.0*sin(angle1),-3.0));
    auto mv2 = t2 * mv1;
    color->setValue(.0,1.0,.0);//green
    drawCube(mv2);

    //subobject3
    Matrix4 t3 = Matrix4::makeTranslation(Cvec3(2.0*cos(angle1),0,-3.0));
    auto mv3 = t3 * mv2;
    color->setValue(.0,.0,1.0);//blue
    drawCube(mv3);
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
    glClearDepth( -100.0);
    //glDepthFunc(GL_LESS);
    glReadBuffer(GL_FRONT);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
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
