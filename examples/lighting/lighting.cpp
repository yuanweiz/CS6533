#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "matrix4.h"

#include "Program.h"
#include "Uniform.h"
#include "BufferObject.h"
#include "LuaConfig.h"
#include "Attribute.h"

#include "Timer.h"

//global pointers to on-stack objects
Program* program;
VertexBuffer *vertPosition,*vertColor;
LuaConfig *config;
UniformMatrix4fv *modelView, *projection;
Attribute * color, *position;

Timer timer;

class EntityHierarchy {
    
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

    Matrix4 p= Matrix4::makeProjection(fovy,aspectRatio,zNear,zFar);
    p.writeToColumnMajorMatrix(colMajorMat);
    projection->setValue(1,false,colMajorMat);

    Matrix4 obj = Matrix4::makeZRotation(10.0/100000*timer.runningTime());
    Matrix4 eye=Matrix4::makeTranslation(Cvec3(eye_x,eye_y,eye_z));
    Matrix4 mvm = inv(eye)*obj;
    mvm.writeToColumnMajorMatrix(colMajorMat);
    modelView->setValue(1,false,colMajorMat);

    vertPosition->bind();
    glVertexAttribPointer(position->get(), 3, GL_FLOAT, GL_FALSE, 0, 0);
    position->enable();

    vertColor->bind();
    glVertexAttribPointer(color->get(), 4, GL_FLOAT, GL_FALSE, 0, 0);
    color->enable();

    glDrawArrays(GL_TRIANGLES, 0, vertPosition->size());

    position->disable();
    color->disable();

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
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glReadBuffer(GL_BACK);
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
    Attribute color_( &program_,"color");
    UniformMatrix4fv modelView_(&program_, "mvm"),
        projection_(&program_,"p");
    auto cubeVerts = config_.getFloatArray("cubeVerts");
    VertexBuffer vertPosition_(cubeVerts.data() ,cubeVerts.size() );
    auto cubeColors = config_.getFloatArray("cubeColors");
    VertexBuffer vertColor_( cubeColors.data(), cubeColors.size() );
    
    //set global pointers, they won't dangle
    program = &program_;
    config = &config_;
    position = &position_;
    color = &color_;
    modelView = &modelView_;
    projection = &projection_;
    vertPosition = &vertPosition_;
    vertColor = &vertColor_;

    program->useThis();
    color->enable();
    position->enable();
    timer.start();

    glutMainLoop();
	return 0;
}


