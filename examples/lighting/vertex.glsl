attribute vec4 position;
attribute vec4 normal;
varying vec4 varyingNormal;
uniform mat4 mvm;
uniform mat4 p;
uniform mat4 normalMat; //for normal

void main() {
    varyingNormal = normalMat*normal;
    vec4 pos = p*mvm*position;
    gl_Position = pos;
}
