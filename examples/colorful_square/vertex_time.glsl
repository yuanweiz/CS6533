varying vec2 varyingTexCoord;
uniform float time;
attribute vec4 position;
//attribute vec4 position2;
attribute vec2 texCoord;

//attribute mat4 rot ;
void main() {
    varyingTexCoord = texCoord;
    //mat4 rot = mat4( cos(time), sin(time),0,0,
    //                    -sin(time), cos(time),0,0,
    //                    0,0,1,0,
    //                    0,0,0,1);
    mat4 rot = mat4( cos(time), 0,-sin(time),0,
                        0, 1,0,0,
                        sin(time),0,cos(time),0,
                        0,0,0,1);
    //gl_Position = vec4(position.r+time,position.g+time,0.0,1.0);
    gl_Position = rot * position;
}
