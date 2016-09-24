varying vec2 varyingTexCoord;
uniform float time;
attribute vec4 position;
attribute vec2 texCoord;
void main() {
    varyingTexCoord = texCoord;
    //gl_Position = vec4(position.r+mouseLoc.x,position.g+mouseLoc.y,0.0,1.0);
    mat4 rot = mat4 ( cos(time),-sin(time),0,0,
                  sin(time),cos(time), 0,0,
                  0,0,1,0,
                  0,0,0,1);
    gl_Position =rot* position;
}
