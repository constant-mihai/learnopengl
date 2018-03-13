#version 330 core
// Ouput data

out vec4 outCol;

// Input data
// in vec3 vsCol;
in vec2 vsTex;

uniform float ourColor;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	// Output color = red
	//color = ourColor;
    //outCol = vec4(vsCol, 1.0);
    outCol = mix(texture(ourTexture1, vsTex), texture(ourTexture2, vsTex), 0.2) * ourColor;
}
