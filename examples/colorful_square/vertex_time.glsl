varying vec2 varyingTexCoord;
uniform float time;
attribute vec4 position;
//attribute vec4 position2;
attribute vec2 texCoord;
void main() {
    varyingTexCoord = texCoord;
    gl_Position = vec4(position.r+time,position.g+time,0.0,1.0);
}
