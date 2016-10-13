#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "IOAux.h"
#include "config.h"
#include "matrix4.h"
#include "Timer.h"

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

class TimeVariable {
    public:
        TimeVariable(char c,double initVal,double changeRate):
            initVal_(initVal),changeRate_(changeRate),
            trigger_(c),timer_()
    {
    }
        void changeStat(){
            timer_.changeStat();
        }
        void start(){
            timer_.start();
        }
        void stop(){
            timer_.stop();
        }
        double value(){
            return initVal_ + changeRate_* timer_.runningTime()/1000000;
        }
        char trigger(){ return trigger_;}
    private:
        const double initVal_;
        const double changeRate_;
        char trigger_;
        Timer timer_;
};

//configuration from lua script
std::string currentDirectory(getCurrentDirectory());
std::string configFileName (currentDirectory +"/config.lua");
LuaConfig config(configFileName.c_str());
bool lua_test;
bool use_3d;
double fovy,aspectRatio,zNear,zFar;
double eye_x,eye_y,eye_z;
double rot_x,rot_y,rot_z;
LuaTable keyBoardConfig;
std::vector<TimeVariable> timeVariables;
std::vector<float> cubeVerts,cubeColors;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    rot_z = timeVariables[0].value();
    eye_x = timeVariables[1].value()+timeVariables[2].value();
    eye_y = timeVariables[3].value()+timeVariables[4].value();
    eye_z = timeVariables[5].value()+timeVariables[6].value();

    Matrix4 obj = Matrix4::makeZRotation(rot_z);

    Matrix4 eye = Matrix4::makeTranslation(Cvec3(eye_x,eye_y,eye_z));
    Matrix4 mvm = inv(eye)*obj;
    GLfloat colMajorMat[16] ;
    mvm.writeToColumnMajorMatrix(colMajorMat);
    glUniformMatrix4fv(modelviewMatrixUniformLocation,1,false,colMajorMat);
    Matrix4 p= Matrix4::makeProjection(fovy,aspectRatio,zNear,zFar);
    p.writeToColumnMajorMatrix(colMajorMat);
    glUniformMatrix4fv(projectionMatrixUniformLocation,1,false,colMajorMat);

    glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorAttribute);

    glDrawArrays(GL_TRIANGLES, 0, cubeVerts.size());
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
    cubeVerts = config.getFloatArray("cubeVerts");
    glBufferData(GL_ARRAY_BUFFER, cubeVerts.size()*sizeof(GLfloat), &cubeVerts[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vertColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertColorVBO);
    cubeColors = config.getFloatArray("cubeColors");
    glBufferData(GL_ARRAY_BUFFER, cubeColors.size()*sizeof(GLfloat), &cubeColors[0], GL_STATIC_DRAW);

}
void reshape(int w,int h){
    glViewport(0,0,w,h);
}
void readLuaConfig(){
    use_3d=config.getBool("use_3d");
    auto eye = config.getFloatArray("eye");
    auto rotate = config.getFloatArray("rotate");
    auto projection = config.getFloatArray("projection");
    keyBoardConfig=config.getLuaTable("keyboard");
    //decode it into TimeVariable
    auto timeConfig = config.getLuaTable("timeVariable");
    timeVariables.clear();
    for (size_t i =0;i<timeConfig.size();i+=3){
        auto & str = timeConfig.get<std::string>(i);
        auto initVal = timeConfig.get<double>(i+1);
        auto changeRate = timeConfig.get<double>(i+2);
        //move semantics
        timeVariables.push_back( TimeVariable{str[0],initVal,changeRate});
    }
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

static bool char_equal (unsigned char a,unsigned char b){
    return (0x20 |a)==(0x20|b);
}
void keyboard (unsigned char c,int ,int ){
    if (c=='1'){
        readLuaConfig();
        return;
    }
    if (false)
    for (size_t i=0;i<keyBoardConfig.size();i+=3){
        auto & key = keyBoardConfig.get<std::string>(i);
        double & val = keyBoardConfig.get<double>(i+1);
        double step= keyBoardConfig.get<double>(i+2);
        if (char_equal(key[0],c)){ //ignore case
            val+=step;
            break;
        }
        else if (key.size()>1&&char_equal(key[1],c)){
            val-=step;
            break;
        }
    }
    //handle timeVariables
    for (auto & val : timeVariables){
        if (char_equal(val.trigger(), c)){
            val.start();
            break;
        }
    }
}
void keyboardup(unsigned char c,int ,int){
    for (auto & val : timeVariables){
        if (char_equal(val.trigger(), c)){
            val.stop();
            break;
        }
    }
}
///////////////////////////////////////////////////////////
// Main program entry point
//
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    readLuaConfig();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("Simple");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    init();
    glutMainLoop();
	return 0;
}
