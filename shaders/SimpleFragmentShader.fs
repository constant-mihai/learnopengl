#version 330 core
// Ouput data

out vec4 outCol;

// Input data
// in vec3 vsCol;
in vec2 vsTex;

uniform float ourColor;
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
    //outCol = mix(texture(ourTexture1, vsTex), texture(ourTexture2, vsTex), 0.2) * ourColor;
    outCol = texture(texture_diffuse1, vsTex); 
}
