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
 // lightpos to the fragment shader. in_lightPos is currently in world space.
uniform vec3 u_lightPos;
uniform mat4 transposedInversedMov;

out vec3 in_normal;
out vec2 in_tex;
out vec3 in_fragPos;
out vec3 in_lightPos;

void main(){
    gl_Position = (mvp * vec4(aPos, 1.0f));
    in_fragPos = vec3(mov * vec4(aPos, 1.0));
    in_lightPos = vec3(mov* vec4(u_lightPos, 1.0));
    // Transform world-space light position to view-space light position
    in_normal = vec3(transposedInversedMov * vec4(aCol, 0));
    in_tex = aTex;
}
