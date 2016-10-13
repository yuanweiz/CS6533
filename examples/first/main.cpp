#include <GL/gl.h>
#include <GL/freeglut.h>
#include <sys/time.h>
#include <stdio.h>
#include "Timer.h"

void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	// Flush drawing commands
	glFlush();
}
///////////////////////////////////////////////////////////
// Set up the rendering state
void SetupRC(void)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}
///////////////////////////////////////////////////////////
// Main program entry point
//
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
	return 0;
}
