attribute vec4 position;
attribute vec4 color;
varying vec4 varyingColor;
uniform mat4 mvm;
uniform mat4 p;

void main() {
    varyingColor = color;
    vec4 tmp = position;
    //tmp.a=1.0;
    gl_Position = tmp;
}
