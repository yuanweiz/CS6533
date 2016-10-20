#ifndef __ENTITY_H
#define __ENTITY_H
#include <GL/glew.h>
#include "cvec.h"
#include "quat.h"
#include "Noncopyable.h"
#include <memory>
//value semantic, copyable
struct Transform {
	Quat rotation;
	Cvec3 scale;
	Cvec3 position;
	Transform() : scale(1.0f, 1.0f, 1.0f) {
	}
	Matrix4 createMatrix();
};

struct Geometry {
    Geometry(GLuint vbo,GLuint ibo);
    //Geometry(GLuint vbo,GLuint ibo,Transform t=Transform());
    ~Geometry();
    void onDraw();
    template <typename Func>
    void setDrawCallback(const Func&);
    Transform & getTransform();
    private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

struct Entity :Noncopyable{
    Entity();
    ~Entity();
    Entity * addSubEntity(const Transform&);
    void addGeometry(const Geometry &, const Transform);
    int size();
    void draw();
    Entity& clone();
    private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif// __ENTITY_H

