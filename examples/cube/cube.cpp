#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "IOAux.h"
#include "config.h"
#include "matrix4.h"

extern "C"{
#include <lauxlib.h>
}

GLuint program;

// a handle to vertex buffer object
// it points to an internal variable
// of OpenGL state machine
GLuint vertPositionVBO;
GLuint vertColorVBO;

// a handle(index) to glsl variable
GLuint positionAttribute;
GLuint colorAttribute;

//Uniforms
GLuint modelviewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    Matrix4 obj;
    obj = obj.makeZRotation(45);
    Matrix4 eye;
    eye.makeTranslation(Cvec3(-.5,.0,.0));
    Matrix4 mvm = inv(eye)*obj;
    GLfloat colMajorMat[16] ;
    mvm.writeToColumnMajorMatrix(colMajorMat);
    glUniformMatrix4fv(modelviewMatrixUniformLocation,1,false,colMajorMat);
    Matrix4 p;
    p=p.makeProjection(45.0,1.0,-0.1,-100.0);
    p.writeToColumnMajorMatrix(colMajorMat);
    glUniformMatrix4fv(projectionMatrixUniformLocation,1,false,colMajorMat);



    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 24);
    glDisableVertexAttribArray(positionAttribute);
    glDisableVertexAttribArray(colorAttribute);

	glutSwapBuffers();
}
void idle(){
    glutPostRedisplay();
}
///////////////////////////////////////////////////////////
// Set up the rendering state
void init(void)
{

    //glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);

    //glDepthFunc(GL_LESS);
    //glReadBuffer(GL_BACK);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5,0.5,0.5,1);

    std::string fname = getCurrentDirectory()+"/config.lua";
    LuaConfig config(fname.c_str());

    glewInit();
    program = glCreateProgram();
    using std::string ;
    string dir = getCurrentDirectory();
    string vs = dir+"/vertex.glsl";
    string fs = dir+"/fragment.glsl";
    readAndCompileShader(program, 
            vs.c_str(),
            fs.c_str()
            );

    positionAttribute = glGetAttribLocation(program,"position");
    colorAttribute = glGetAttribLocation(program,"color");
    modelviewMatrixUniformLocation = glGetUniformLocation(program,"mvm");
    projectionMatrixUniformLocation = glGetUniformLocation(program, "p");

    glGenBuffers(1, &vertPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    std::vector<float> cubeVerts = config.getFloatArray("cubeVerts");
    glBufferData(GL_ARRAY_BUFFER, cubeVerts.size()*sizeof(GLfloat), &cubeVerts[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vertColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
    std::vector<float> cubeColors = config.getFloatArray("cubeColors");
    glBufferData(GL_ARRAY_BUFFER, cubeColors.size()*sizeof(GLfloat), &cubeColors[0], GL_STATIC_DRAW);

}
void reshape(int w,int h){
    glViewport(0,0,w,h);
}
///////////////////////////////////////////////////////////
// Main program entry point
//
void func(void)
{
}
int main(int argc, char* argv[])
{

    if(false){
	glutInit(&argc, argv);
	glutInitDisplayMode(
            GLUT_DOUBLE | GLUT_RGBA 
            //|GLUT_DEPTH
            );
    glutInitWindowSize(500,500);
	glutCreateWindow("Simple");

	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    init();
	glutMainLoop();
    }
    func();
    std::string fname = getCurrentDirectory()+"/config.lua";
    LuaConfig config(fname.c_str());
    std::vector<float> cubeVerts = config.getFloatArray("cubeVerts");
    std::vector<float> cubeColors = config.getFloatArray("cubeColors");
    //std::string fname=getCurrentDirectory()+"/config.lua";
    //LuaConfig config(fname.c_str());
    //auto vf = config.getFloatArray("cubeVerts");
	return 0;
}
