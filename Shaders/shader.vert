#version 430 core

layout(location = 0) in vec3 aPos;
out vec2 FragCoord;

void main(){
    FragCoord = aPos.xy;
    gl_Position = vec4(aPos, 1.0f);
}