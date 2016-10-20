varying vec4 varyingColor;
void main() {
    gl_FragColor = vec4(1,0,0,1);
    //if (varyingColor.r <.01 && varyingColor.g<0.01 && varyingColor.b<0.01)
    //if (varyingColor.a<0.95)
    //    gl_FragColor = vec4(1,1,1,1);
    //else 
    vec4 color = varyingColor;
    color.a=1.0;
    gl_FragColor = color;
}
