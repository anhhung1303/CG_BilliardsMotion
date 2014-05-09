#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 uMVPMatrix;

out vec2 TexCoord;
out vec3 Normal;

void main(){
    gl_Position = uMVPMatrix * vec4(position, 1.0);
    TexCoord = texCoord;
    Normal = (uMVPMatrix * vec4(normal, 0.0)).xyz;
}