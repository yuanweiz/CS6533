#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "IOAux.h"
#include "Timer.h"
#include <unistd.h>
GLuint program;

// a handle to vertex buffer object
// it points to an internal variable
// of OpenGL state machine
GLuint vertPositionVBO;
GLuint vertTexCoordVBO;

// a handle(index) to glsl variable
GLuint positionAttribute;
GLuint texCoordAttribute;

//texture
GLuint imgTexture;

//uniform
GLuint timeUniform;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
    glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindTexture(GL_TEXTURE_2D, imgTexture);


    //draw first one
    auto timeInMs = Timer::getTimeOfDay()/1000;
    float t = static_cast<float>(timeInMs%1000);

    glUniform1f(timeUniform,t/1000.f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

	glutSwapBuffers();
}
void idle(){
    ::glutPostRedisplay();
}
///////////////////////////////////////////////////////////
// Set up the rendering state
void init(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5,0.5,0.5,1);


    glewInit();
    program = glCreateProgram();
    using std::string ;
    string dir = getCurrentDirectory();
    string vs = dir+"/vertex_time.glsl";
    string fs = dir+"/fragment_time.glsl";
    string imgPath = dir+"/lena.gif";

    imgTexture = loadGLTexture(imgPath.c_str());
    readAndCompileShader(program, 
            vs.c_str(),
            fs.c_str()
            );
    glUseProgram(program);

    positionAttribute = glGetAttribLocation(program,"position");
    texCoordAttribute = glGetAttribLocation(program,"texCoord");

    glEnableVertexAttribArray(texCoordAttribute);
    glEnableVertexAttribArray(positionAttribute);

    glGenBuffers(1, &vertPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    GLfloat sqVerts[12] = {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f
    };
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), sqVerts, GL_STATIC_DRAW);

    glGenBuffers(1, &vertTexCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
    GLfloat sqTexCoords[12] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), sqTexCoords, GL_STATIC_DRAW);

    timeUniform = glGetUniformLocation(program,"time");
}
void reshape(int w,int h){
    glViewport(0,0,w,h);
}
///////////////////////////////////////////////////////////
// Main program entry point
//
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500,500);
	glutCreateWindow("Simple");

	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    init();
	glutMainLoop();
	return 0;
}
