#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "IOAux.h"

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

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);
    glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, imgTexture);


    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(positionAttribute);
    glDisableVertexAttribArray(texCoordAttribute);

	glutSwapBuffers();
}
void idle(){
    ::glutPostRedisplay();
}
///////////////////////////////////////////////////////////
// Set up the rendering state
void init(void)
{
    glewInit();
    program = glCreateProgram();
    using std::string ;
    string dir = getCurrentDirectory();
    string vs = dir+"/vertex_texture.glsl";
    string fs = dir+"/fragment_texture.glsl";
    string imgPath = dir+"/emoji.png";

    imgTexture = loadGLTexture(imgPath.c_str());
    readAndCompileShader(program, 
            vs.c_str(),
            fs.c_str()
            );

    positionAttribute = glGetAttribLocation(program,"position");
    texCoordAttribute = glGetAttribLocation(program,"texCoord");

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
    init();
	glutMainLoop();
	return 0;
}
