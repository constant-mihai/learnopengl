#version 330 core
// Ouput data

out vec4 outCol;

// Input data
in vec3 vsCol;
in vec2 vsTex;
in vec3 FragPos;
in vec3 lightPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

uniform float ourColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal_MV = normalize(vsCol);
    vec3 lightDir = normalize(lightPos - FragPos);

    /* Diffuse impact. Dot product */
    float diff = max(dot(normal_MV, lightDir), 0.0);
    /* Multiply by lights color */
    vec3 diffuseColor = diff * lightColor; 

    /* Specular */
    float specularStrength = 0.5;
    // the viewer is always at (0,0,0) in view-space, 
    // in that case the viewDir is (0,0,0) - Position => -Position
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal_MV);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuseColor + specular) * objectColor;
    outCol = vec4(result, 1.0);
    //outCol = mix(texture(ourTexture1, vsTex), texture(ourTexture2, vsTex), 0.2) * ourColor;
    //outCol = texture(texture_diffuse1, vsTex); 
}
