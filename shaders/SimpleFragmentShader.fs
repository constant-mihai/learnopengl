#version 330 core
// Ouput data

out vec4 outCol;

// Input data
in vec3 in_normal;
in vec2 in_tex;
in vec3 in_fragPos;
in vec3 in_lightPos;

uniform vec3 objectColor;
uniform vec3 viewPos;

uniform float ourColor;

struct Material {
    sampler2D ambient;
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D diffuse3;
    sampler2D specular1;
    sampler2D specular2;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

float calculateSpotLightAttenuation(vec3 lightPos, float lightConst, float lightLinear, float lightQuadr, vec3 objPos) {
    float distance = length(lightPos - objPos);
    return (1.0 / (lightConst + lightLinear * distance + 
                            lightQuadr * (distance * distance)));
}

/**
 * ******************************************************
 * Calculate Diffuse
 * TODO DOC THIS
 * ******************************************************
**/
vec3 calculateDiffuse(vec3 normal_MV, vec3 lightDir, vec3 lightDiffuse, sampler2D materialDiffuse) {
    /* Diffuse impact. Dot product */
    float diff = max(dot(normal_MV, lightDir), 0.0);
    return (lightDiffuse * (diff * vec3(texture(materialDiffuse, in_tex))));
}

/**
 * ******************************************************
 * Calculate Specular
 * TODO DOC THIS
 * ******************************************************
**/
vec3 calculateSpecular(vec3 normal_MV, vec3 viewDir, vec3 lightDir, vec3 lightSpecular, sampler2D materialSpecular, float materialShininess) {
    // the viewer is always at (0,0,0) in view-space, 
    // in that case the viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, normal_MV);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    return (lightSpecular * (spec * vec3(texture(materialSpecular, in_tex))));
}

/**
 * ******************************************************
 * Main 
 * ******************************************************
**/
void main()
{
    //light.position = in_lightPos; // TODO where is the light position? Should it be mvp?
    float attenuation = calculateSpotLightAttenuation(
            light.position, light.constant, 
            light.linear, light.quadratic, in_fragPos);

    /* Setting material */
    vec3 ambient = light.ambient * vec3(texture(material.ambient, in_tex));
    //ambient *= attenuation;

    /* Normalization */
    vec3 normal_MV = normalize(in_normal);
    vec3 lightDir = normalize(in_lightPos - in_fragPos);
    vec3 viewDir = normalize(viewPos - in_fragPos);

    /* Diffuse */
    vec3 diffuseColor = calculateDiffuse(normal_MV, lightDir, light.diffuse, material.diffuse1);
    //diffuseColor *= attenuation;

    /* Specular */
    vec3 specular = calculateSpecular(normal_MV, viewDir, lightDir, light.specular, material.specular1, material.shininess);
    //specular *= attenuation;

    vec3 result = (ambient + diffuseColor + specular);// * objectColor;
    outCol = vec4(result, 1.0);
    //outCol = mix(texture(ourTexture1, in_tex), texture(ourTexture2, in_tex), 0.2) * ourColor;
    //outCol = texture(texture_diffuse1, in_tex) * outCol;
}
