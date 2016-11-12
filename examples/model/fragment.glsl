varying vec3 varyingNormal;
uniform vec3 uColor;
varying vec3 varyingPosition;

vec3 yellow = vec3(1.0,1.0,0);

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
    vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
    vec3 specularColor = vec3(0.0, 0.0, 0.0);
    for(int i=0; i< 2; i++) {
        vec3 lightDirection = -normalize(varyingPosition-lights[i].position);
        float diffuse = max(0.2, dot(varyingNormal, lightDirection));
        float attenuation = attenuate(distance(varyingPosition, lights[i].position) / 5.0, 2.7, 5.0);
        // debug part
        attenuation = 1.0;
        //diffuse = 1.0;
        //
        diffuseColor += (lights[i].diffuseColor * diffuse) * attenuation;
        vec3 v = normalize(-varyingPosition);
        vec3 h = normalize(v + lightDirection);
        float specular = pow(max(0.0, dot(h, varyingNormal)), 6.0);
        //specular = 1.0;
        //specular = 0.0;
        specularColor += lights[i].specularColor * specular * attenuation;
    }
    vec3 intensity = (uColor * diffuseColor) + specularColor;
    gl_FragColor = vec4(intensity.xyz, 1.0);
}

