attribute vec4 position;
attribute vec2 texCoord;
varying vec2 varyingTexCoord;
void main() {
    varyingTexCoord = texCoord;
    gl_Position = position;
}
