//varying vec3 varyingNormal;
varying vec3 varyingPosition;
varying vec2 varyingUv;
varying mat3 varyingTBNMatrix;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

struct Light {
    vec3 position;
    vec3 diffuseColor;
    vec3 specularColor;
};

uniform Light lights[10];

float attenuate(float dist, float a, float b) {
    return 1.0 / (1.0 + a*dist + b*dist*dist);
}

void main() {
    vec2 uv = varyingUv;
    vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
    vec3 specularColor = vec3(0.0, 0.0, 0.0);

    vec3 textureNormal = normalize((texture2D(normalTex, varyingUv).xyz * 2.0) -1.0);
    textureNormal = normalize(varyingTBNMatrix * textureNormal);

    for(int i=0; i< 2; i++) {
        vec3 lightDirection = -normalize(varyingPosition-lights[i].position);
        //float diffuse = max(0.2, dot(varyingNormal, lightDirection));
        float diffuse = max(0.2, dot(textureNormal, lightDirection));
        float attenuation = attenuate(distance(varyingPosition, lights[i].position) / 5.0, 2.7, 5.0);
        attenuation = 1.0;
        diffuseColor += (lights[i].diffuseColor * diffuse) * attenuation;
        vec3 v = normalize(-varyingPosition);
        vec3 h = normalize(v + lightDirection);
        float specular = pow(max(0.0, dot(h, textureNormal)), 6.0);
        specularColor += lights[i].specularColor * specular * attenuation;
    }
    float texSpecular = texture2D(specularTex,uv).x;
    vec3 texColor = texture2D(diffuseTex,uv).xyz;
    vec3 intensity =  texColor* diffuseColor + 20.0 *texSpecular* specularColor;
    gl_FragColor = vec4(intensity.xyz*2.0, 1.0);
}

float norm (vec3 v){
    return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}
