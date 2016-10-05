attribute vec4 position;
attribute vec4 color;
varying vec4 varyingColor;
uniform mat4 mvm;
uniform mat4 p;

void main() {
    varyingColor = color;
    //gl_Position = position;
    gl_Position = p * mvm* position;
}
