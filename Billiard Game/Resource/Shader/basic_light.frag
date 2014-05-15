#version 330

layout (std140) uniform Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    vec4 emissive;
} material;

layout (std140) uniform Light{
    vec4 ambientIntensity;
    vec4 diffuseIntensity;
    vec4 specularIntensity;
    vec3 position;		//in world coordinates
	
	vec3 spotDirection;
    float spotExponent;
    float spotCutoff;

    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
} light;

uniform vec3 viewPos; //camera position in world coordinates
uniform int texCount;
uniform sampler2D uSampler;

in vec3 Position; //in world coordinates
in vec2 TexCoord;
in vec3 Normal; //in world coordinates

void main(){
    vec3 lightDir = normalize(light.position - Position);
    vec3 viewDir = normalize(viewPos - Position);
    vec3 normalDir = normalize(Normal);

	//Ambient
    vec4 La = light.ambientIntensity * material.ambient;

    //Diffuse reflection - Lambertian illumination model
	float diffuseCoef = max(dot(normalDir, lightDir), 0.0);
	vec4 Ld = light.diffuseIntensity * material.diffuse * diffuseCoef;

    //Specular reflection. - Phong illumination model
	vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
	float specularCoef = max(dot(reflectDir, viewDir), 0.0);
	specularCoef = pow(specularCoef, material.shininess);
    vec4 Ls = light.specularIntensity * material.specular * specularCoef;

	//attenuation
	float distanceToLight = length(light.position - Position);
	float attenuation = light.attenuationConstant;
	attenuation += light.attenuationLinear * distanceToLight;
	attenuation += light.attenuationQuadratic * pow(distanceToLight, 2);
	attenuation = 1.0f / attenuation;

	//vec4 linearColor = material.emissive + attenuation * (La + Ld + Ls);
	vec4 linearColor = La + Ld + Ls;
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);
	if (texCount != 0){
		texColor = texture(uSampler, TexCoord);
	}

	//after gamma correction
	vec4 gamma = vec4(1.0f / 2.2f);
	gl_FragColor = pow(linearColor, gamma) * texColor;
}
