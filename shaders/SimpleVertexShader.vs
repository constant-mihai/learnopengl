#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTex;

uniform mat4 model; 
uniform mat4 view;
uniform mat4 mov;
uniform mat4 mvp;
 // we now define the uniform in the vertex shader and pass the 'view space' 
 // lightpos to the fragment shader. lightPos is currently in world space.
uniform vec3 u_lightPos;
uniform mat4 transposedInversedMov;

out vec3 vsCol;
out vec2 vsTex;
out vec3 FragPos;
out vec3 lightPos;

void main(){
    gl_Position = (mvp * vec4(aPos, 1.0f));
    FragPos = vec3(mov * vec4(aPos, 1.0));
    lightPos = vec3(view * vec4(u_lightPos, 1.0));
    // Transform world-space light position to view-space light position
    vsCol = vec3(transposedInversedMov * vec4(aCol, 0));
    vsTex = aTex;
}
