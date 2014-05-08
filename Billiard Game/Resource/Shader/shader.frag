#version 330

in vec4 Color;

in vec2 TexCoord0;
in vec3 Normal0;

uniform sampler2D gSampler;

out vec4 FragColor;

void main(){
    FragColor = texture2D(gSampler, TexCoord0.xy);
}