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

//configuration from lua script
bool use_3d;
double fovy,aspectRatio,zNear,zFar;
double eye_x,eye_y,eye_z;
double rot_x,rot_y,rot_z;

void display(void)
{

    if (use_3d){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glUseProgram(program);

    Matrix4 obj = Matrix4::makeZRotation(rot_z);
    Matrix4 eye = Matrix4::makeTranslation(Cvec3(eye_x,eye_y,eye_z));
    //eye.makeTranslation(Cvec3(-.5,0.,0.));
    Matrix4 mvm = inv(eye)*obj;
    GLfloat colMajorMat[16] ;
    mvm.writeToColumnMajorMatrix(colMajorMat);
    glUniformMatrix4fv(modelviewMatrixUniformLocation,1,false,colMajorMat);
    //Matrix4 p= Matrix4::makeProjection(45.0,1.0,-0.1,-100.0);
    Matrix4 p= Matrix4::makeProjection(fovy,aspectRatio,zNear,zFar);
    p.writeToColumnMajorMatrix(colMajorMat);
    glUniformMatrix4fv(projectionMatrixUniformLocation,1,false,colMajorMat);

    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 108);
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
    glEnable(GL_BLEND);
    if (use_3d){
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glReadBuffer(GL_BACK);
    }
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.,0.,0.,1.);

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
    auto cubeVerts = config.getFloatArray("cubeVerts");
    glBufferData(GL_ARRAY_BUFFER, cubeVerts.size()*sizeof(GLfloat), &cubeVerts[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vertColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
    auto cubeColors = config.getFloatArray("cubeColors");
    glBufferData(GL_ARRAY_BUFFER, cubeColors.size()*sizeof(GLfloat), &cubeColors[0], GL_STATIC_DRAW);

}
void reshape(int w,int h){
    glViewport(0,0,w,h);
}

void readLuaConfig(){
    std::string fname = getCurrentDirectory()+"/config.lua";
    LuaConfig config(fname.c_str());
    use_3d=config.getBool("use_3d");
    auto eye = config.getFloatArray("eye");
    auto rotate = config.getFloatArray("rotate");
    auto projection = config.getFloatArray("projection");
    eye_x = eye[0];
    eye_y = eye[1];
    eye_z = eye[2];
    rot_x = rotate[0];
    rot_y = rotate[1];
    rot_z = rotate[2];
    fovy = projection[0];
    aspectRatio = projection[1];
    zNear = projection[2];
    zFar = projection[3];
}
///////////////////////////////////////////////////////////
// Main program entry point
//
int main(int argc, char* argv[])
{

	glutInit(&argc, argv);

    readLuaConfig();
    if (use_3d){
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);
    }
    else {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    }
    glutInitWindowSize(500,500);
	glutCreateWindow("Simple");

	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    init();
	glutMainLoop();
	return 0;
}
