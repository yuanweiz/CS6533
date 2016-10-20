#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
//#include "IOAux.h"
#include "Timer.h"
#include <unistd.h>
#include "Uniform.h"
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
GLuint offsetUniform;

//window size
int g_w = 500,g_h = 500;

bool down=false;
int64_t t,oldt=0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
    glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindTexture(GL_TEXTURE_2D, imgTexture);

    static float theta=0.0f;
    static float dtheta=0.f;
    static float edge =down;
    if (down){ 
        t= Timer::getTimeOfDay();
        if (!edge){
            oldt=t;
            puts("edge=down=true");
            edge = true;
            printf("theta=%f\n",theta);
        }

        if (oldt==0) {
            oldt=t; //hit here only once
            puts("hit here only once");
        }
        dtheta=(t-oldt)/1000000.f;
    }
    else {
        //edge-trigger semantics
        if (edge){
            oldt= t;
            puts("now edge=down=false");
            theta = theta + dtheta;
            dtheta = 0.f;
            edge=false;
            printf("theta=%f\n",theta);
        }
    }

    glUniform1f(timeUniform,theta+dtheta);
    glUniform2f(offsetUniform,-0.25,-0.25);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUniform1f(timeUniform,-theta-dtheta);
    glUniform2f(offsetUniform,0.25,0.25);
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
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);
    glReadBuffer(GL_BACK);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5,0.5,0.5,1);

    glewInit();
    program = glCreateProgram();
    using std::string ;
    string dir = CURRENT_DIR;//getCurrentDirectory();
    string vs = dir+"/vertex_rotate.glsl";
    string fs = dir+"/fragment_rotate.glsl";
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
    offsetUniform = glGetUniformLocation(program,"offset");
    glUniform1f( timeUniform,0.f);

}

void reshape(int w,int h){
    g_w=w;
    g_h=h;
    glViewport(0,0,w,h);
}

void mouse(int button,int state,int x,int y){
    (void)(x*y); //suppress warning
    if (button == GLUT_LEFT_BUTTON ){
        if (state == GLUT_DOWN){
            down = true;
            puts("down=true");
        }
        else if (state == GLUT_UP){
            down = false;
            puts("down=false");
        }
    }
    else {
    }
}

void mouseMove(int x,int y){
    (void)(x+y);
}
///////////////////////////////////////////////////////////
// Main program entry point
//
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(g_w,g_h);
	glutCreateWindow("Simple");

	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);
    init();
	glutMainLoop();
	return 0;
}
