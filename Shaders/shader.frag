#version 430 core

in vec2 FragCoord;
out vec4 FragColor;

uniform sampler2D fractalTex;
uniform sampler2D fractalTex2;

void main(){
    vec2 coords = (1.0 + FragCoord) / 2.0;
    FragColor = texture(fractalTex, coords);
}