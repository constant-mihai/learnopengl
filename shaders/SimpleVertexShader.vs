#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTex;

out vec3 vsCol;
out vec2 vsTex;

void main(){
    gl_Position.xyz = aPos;
    gl_Position.w = 1.0;
    vsCol = aCol;
    vsTex = aTex;
}
