#include <GL/glew.h>
#include "glsupport.h"
#include <stdio.h>
#include "matrix4.h"
#include "quat.h"
#include "geometrymaker.h"

#include "Program.h"
#include "Uniform.h"
#include "BufferObject.h"
#include "LuaConfig.h"
#include "Attribute.h"

#include "Timer.h"
#include "Light.h"
#include <iterator>
#include <math.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;
//using VertexPNBuffer = detail::GlBufferObject<GL_ARRAY_BUFFER,VertexPN>;
using vertex_t = VertexPNT;
using VertexPNTBTGBuffer = detail::GlBufferObject<GL_ARRAY_BUFFER,vertex_t>;

//Some Intialization can only be started after glewInit() and glutInit()
//are called, so I can't put these objects into global static scope.
//One possible workaround is to use
//global pointers to main()'s on-stack objects, which won't be disposed
//until the whole program exits.
VertexPNTBTGBuffer *vbo;
IndexBuffer * ibo;
Program* program;
UniformMatrix4fv *modelView, *projection, *normalMat;
Attribute * normal, *position ,*uv;
LuaConfig * config;
LightList * lights;

GLuint diffuseTex,normalTex,specularTex;

Light * light;
Timer timer;

//Arcball related

int w=500,h=500;
Matrix4 world, rotWorld;
Cvec3 v0,v1;
double eye_x,eye_y,eye_z;
int mouseX,mouseY;
bool mouseDown,mouseUp;//these two have edge trigger semantics
bool pressed; //this has level trigger semantic

void loadObjFile(const char *fileName, std::vector<VertexPNT> &outVertices, std::vector<unsigned short> &outIndices) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName, NULL, true);
    bool hasTex = !attrib.texcoords.empty();
    if(ret) {
        for(unsigned int i=0; i < shapes.size(); i++) {
            for(unsigned int j=0; j < shapes[i].mesh.indices.size(); j++) {
                unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
                unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
                unsigned int texOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
                VertexPNT v;
                v.p[0] = attrib.vertices[vertexOffset];
                v.p[1] = attrib.vertices[vertexOffset+1];
                v.p[2] = attrib.vertices[vertexOffset+2];
                v.n[0] = attrib.normals[normalOffset];
                v.n[1] = attrib.normals[normalOffset+1];
                v.n[2] = attrib.normals[normalOffset+2];
                if (hasTex){
                    v.t[0] = attrib.texcoords[texOffset];
                    v.t[1] = 1.0-attrib.texcoords[texOffset+1];
                }
                outVertices.push_back(v);
                outIndices.push_back(outVertices.size()-1);
            }
        }
    } else {
        std::cout << err << std::endl;
        assert(false);
    }
}


void mouseClick(int /*button*/,int state,int /*x*/,int/* y*/){
    if(state == GLUT_UP){
        mouseUp = true;
        pressed = false;
    }
    else if (state == GLUT_DOWN){
        mouseDown = true;
        pressed = true;
    }
}

void mouseMove (int x,int y){
    if (!pressed){
        return ;
    }
    double xInWorld = 2.0*x/w - 1;
    double yInWorld = 1.0 - 2.0*y/h;
    if (mouseDown){
        mouseDown = false;
        v0 = normalize(Cvec3(xInWorld,yInWorld,1));
        return;
    }
    else if (mouseUp){
        mouseUp =false;
        v0 = normalize(Cvec3(xInWorld,yInWorld,1));
        world=rotWorld*world;
        rotWorld = Matrix4();
        return;
    }
    v1 = normalize(Cvec3(xInWorld,yInWorld,1));
    Cvec3 op = cross(v0,v1);
    double ip = dot(v0,v1);
    Quat q( ip,op[0],op[1],op[2]);
    rotWorld = quatToMatrix(q);
    return ;
}

void idle(){
    glutPostRedisplay();
}

void drawCube(const Matrix4& modelMatrix_ ){
    Matrix4 modelMatrix = Matrix4( rotWorld*world*modelMatrix_);
    float colMajorMat[16];
    static LuaTable luaProjection = config->getLuaTable("projection"),
                    luaEyePosition = config->getLuaTable("eye");

    double fovy=luaProjection.get<double>(0);
    double aspectRatio=luaProjection.get<double>(1);
    double zNear=luaProjection.get<double>(2);
    double zFar=luaProjection.get<double>(3);
    eye_x = luaEyePosition.get<double>(0);
    eye_y = luaEyePosition.get<double>(1);
    eye_z = luaEyePosition.get<double>(2);
    Matrix4 p= Matrix4::makeProjection(fovy,aspectRatio,zNear,zFar);
    p.writeToColumnMajorMatrix(colMajorMat);
    projection->setValue(1,false,colMajorMat);

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
    glVertexAttribPointer(position->get(), 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), offset(vertex_t,p));
    glVertexAttribPointer(normal->get(), 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), offset(vertex_t,n));
    glVertexAttribPointer(uv->get(), 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), offset(vertex_t,t));
#undef offset
    ibo->bind();
    glDrawElements(GL_TRIANGLES,ibo->size(),GL_UNSIGNED_SHORT,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    double rotSpeed = config->getDouble("rotSpeed");
    double angle=rotSpeed*timer.runningTime()/1000000;
    Matrix4 mv = Matrix4::makeZRotation(angle) ;
    drawCube(mv);
	glutSwapBuffers();
}

void reshape(int _w,int _h){
    w=_w;
    h=_h;
    glViewport(0,0,w,h);
}

void init(int *argc, char* argv[])
{
    
    glutInit(argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutCreateWindow("Simple");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
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
    //#define ptr_to_stack(ptr,ctor) auto ptr##_ =(ctor);//ptr=& ptr##_ ;
    Program program_( CURRENT_DIR "/vertex.glsl",CURRENT_DIR "/fragment.glsl");
    LuaConfig config_(CURRENT_DIR "/config.lua");
    Attribute position_( &program_,"position");
    Attribute normal_( &program_,"normal");
    Attribute uv_( &program_,"uv");
    UniformMatrix4fv modelView_(&program_, "mvm"),
        projection_(&program_,"p"),
        normalMat_(&program_,"normalMat");
    LightList lightList_(&program_,"lights",10);


    std::vector<vertex_t>verts;
    std::vector<unsigned short>indices;
    loadObjFile( 
            "/data/code/interactive_computer_graphics/3d_models/Monk_Giveaway/Monk_Giveaway.obj",
            verts,indices);
    
    
    VertexPNTBTGBuffer vbo_(verts.data(),verts.size());
    IndexBuffer ibo_(indices.data(),indices.size());
    
    program = &program_;
    position = &position_;
    normal = &normal_;
    uv =&uv_;
    modelView = &modelView_;
    projection = &projection_;
    normalMat = & normalMat_;
    vbo = &vbo_;
    ibo = &ibo_;
    config = &config_;
    lights = & lightList_;

    program->useThis();

    lightList_[0].setPosition(-5.f,-5.f,-5.f);
    lightList_[0].setDiffuseColor(1.0f,1.0f,0.f);
    lightList_[0].setSpecularColor(1.0f,1.0f,0.f);
    lightList_[1].setPosition(4.f,4.f,-4.f);
    lightList_[1].setSpecularColor(1.0f,.0f,1.f);
    lightList_[1].setSpecularColor(1.0f,.0f,1.f);

    Uniform1i diffuseu (program,"diffuseTex");
    glActiveTexture(GL_TEXTURE0);
    diffuseTex = loadGLTexture("/data/code/interactive_computer_graphics/3d_models/Monk_Giveaway/Monk_D.tga");
    diffuseu.setValue(0);
    Uniform1i specularu (program, "specularTex");
    glActiveTexture(GL_TEXTURE1);
    specularTex = loadGLTexture("/data/code/interactive_computer_graphics/3d_models/Monk_Giveaway/Monk_S.tga");
    specularu.setValue(1);


    normal->enable();
    position->enable();
    uv->enable();
    timer.start();
    glutMainLoop();
	return 0;
}
