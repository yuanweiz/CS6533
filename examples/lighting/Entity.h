#ifndef __ENTITY_H
#define __ENTITY_H
#include <GL/glew.h>
#include "cvec.h"
#include "quat.h"
struct Transform {
	Quat rotation;
	Cvec3 scale;
	Cvec3 position;
	Transform() : scale(1.0f, 1.0f, 1.0f) {
	}
	Matrix4 createMatrix();
};
struct Geometry {
	GLuint vertexBO;
	GLuint indexBO;
	int numIndices;
	void Draw(GLuint positionAttribute, GLuint normalAttribute) {
		// bind buffer objects and draw
	}
};


#endif// __ENTITY_H

