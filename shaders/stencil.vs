#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTex;

uniform mat4 model; 
uniform mat4 view;
uniform mat4 mov;
uniform mat4 mvp;
uniform mat4 vp;
 // we now define the uniform in the vertex shader and pass the 'view space' 
 // lightpos to the fragment shader. in_lightPos is currently in world space.
uniform vec3 u_lightPos;
uniform mat4 transposedInversedModel;

out vec3 in_normal;
out vec2 in_tex;
out vec3 in_fragPos;

void main(){
    in_fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = (vp * vec4(in_fragPos, 1.0f));
    in_normal = vec3(transposedInversedModel* vec4(aCol, 0));
    in_tex = aTex;
}
