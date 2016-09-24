varying vec2 varyingTexCoord;
uniform float time;
uniform sampler2D texture2;
void main() {
    //vec2 coord = vec2 (time+varyingTexCoord.x,varyingTexCoord.y);
    vec2 coord = varyingTexCoord;
    gl_FragColor = texture2D(texture2, coord );
}
