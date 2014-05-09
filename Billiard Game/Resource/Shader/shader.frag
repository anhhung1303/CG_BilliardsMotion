#version 330

layout (std140) uniform Material {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
};

in vec2 TexCoord;
in vec3 Normal;

uniform int texCount;
uniform sampler2D uSampler;

out vec4 FragColor;

void main(){
	vec4 color;
	vec4 amb;
	float intensity;
	vec3 lightDir;
	vec3 n;
	
	lightDir = normalize(vec3(1.0,1.0,1.0));
	n = normalize(Normal);	
	intensity = max(dot(lightDir,n),0.0);
	
	if (texCount == 0) {
		color = diffuse;
		amb = ambient;
	}
	else {
		color = texture(uSampler, TexCoord);
		amb = color * 0.33;
	}
	FragColor = (color * intensity) + amb;
}