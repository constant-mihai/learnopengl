#version 330 core
// Ouput data

out vec4 outCol;

// Input data
// in vec3 vsCol;
in vec2 vsTex;

uniform float ourColor;
uniform sampler2D ourTexture;

void main()
{
	// Output color = red
	//color = ourColor;
    //outCol = vec4(vsCol, 1.0);
    outCol = texture(ourTexture, vsTex) * ourColor;
}
