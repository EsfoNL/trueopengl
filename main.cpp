#include <GL/glew.h>
#include <Gl/freeglut.h>
#include <iostream>

void display() {
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    int win = glutCreateWindow("hello");
    glutDisplayFunc(display);
    glutMainLoop();
}