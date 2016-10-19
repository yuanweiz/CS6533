#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "matrix4.h"
#include <memory>

#include "Program.h"
#include "Uniform.h"
#include "BufferObject.h"
#include "LuaConfig.h"

using std::unique_ptr;
using std::shared_ptr;


unique_ptr<Program> program;
unique_ptr<VertexBuffer> vertPosition,vertColor;
//unique_ptr<Uniform> location;
unique_ptr<LuaConfig> config;

void idle(){
    glutPostRedisplay();
}

void display(){
}
void init(void)
{
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glReadBuffer(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.,0.,0.,1.);

    glewInit();
    //program = glCreateProgram();
    program.reset(new Program( CURRENT_DIR "/vertex.glsl",
                        CURRENT_DIR "/fragment.glsl"));
    config.reset( new LuaConfig(CURRENT_DIR "/config.lua"));

    positionAttribute = glGetAttribLocation(program,"position");
    colorAttribute = glGetAttribLocation(program,"color");
    modelviewMatrixUniformLocation = glGetUniformLocation(program,"mvm");
    projectionMatrixUniformLocation = glGetUniformLocation(program, "p");

    auto cubeVerts = config->getFloatArray("cubeVerts");
    vertPosition.reset ( new VertexBuffer(cubeVerts.data() ,cubeVerts.size() ));

    auto cubeColors = config->getFloatArray("cubeColors");
    vertColor.reset ( new VertexBuffer( cubeVerts.data(), cubeVerts.size() ));

}

void reshape(int w,int h){
    glViewport(0,0,w,h);
}


int main(int argc, char* argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("Simple");

    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    //glutKeyboardUpFunc(keyboardup);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    init();
    glutMainLoop();
	return 0;
}


