attribute vec4 position;
attribute vec4 normal;
attribute vec2 uv;

varying vec3 varyingNormal;
varying vec3 varyingPosition;
varying vec2 varyingUv;

uniform mat4 mvm;
uniform mat4 p;
uniform mat4 normalMat; //for normal

void main() {
    varyingNormal = (normalMat*normal).xyz;
    varyingPosition = position.xyz;
    varyingUv = uv;
    vec4 pos = p*mvm*position;
    gl_Position = pos;
}
