#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "matrix4.h"

#include "Program.h"
#include "Uniform.h"
#include "BufferObject.h"
#include "LuaConfig.h"
#include "Attribute.h"


//global pointers to on-stack objects
Program* program;
VertexBuffer *vertPosition,*vertColor;
LuaConfig *config;
UniformMatrix4fv *modelView, *projection;
Attribute * color, *position;

void idle(){
    glutPostRedisplay();
}

void display(void)
{
    float colMajorMat[16];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Matrix4 p= Matrix4::makeProjection(fovy,aspectRatio,zNear,zFar);
    modelView->setValue(1,false,colMajorMat);
    //p.writeToColumnMajorMatrix(colMajorMat);
    //glUniformMatrix4fv(projectionMatrixUniformLocation,1,false,colMajorMat);
    projection->setValue(1,false,colMajorMat);

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
    glClearColor(0.,0.,0.,1.);
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
    VertexBuffer vertColor_( cubeVerts.data(), cubeVerts.size() );
    
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

    glutMainLoop();
	return 0;
}


