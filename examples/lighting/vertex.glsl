attribute vec4 position;
attribute vec4 normal;
varying vec4 varyingNormal;
uniform mat4 mvm;
uniform mat4 p;
uniform mat4 normalMat; //for normal

void main() {
    varyingNormal = normalMat*normal;
    gl_Position = p * mvm* position;
}
