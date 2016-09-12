#ifndef __WRAPPER_H
#define __WRAPPER_H
#include <GL/gl.h>
struct GLDrawingGaurd{
	explicit GLDrawingGaurd(::GLenum mode){
		::glBegin(mode);
	}
	~GLDrawingGaurd(){
		::glEnd();
	}
};
struct GLMatrixStackGaurd{
	GLMatrixStackGaurd(){
		::glPushMatrix();
	}
	~GLMatrixStackGaurd(){
		::glPopMatrix();
	}
};
#endif// __WRAPPER_H
