varying vec2 varyingTexCoord;
uniform sampler2D texture;
void main() {
    gl_FragColor = texture2D(texture,varyingTexCoord );
}
