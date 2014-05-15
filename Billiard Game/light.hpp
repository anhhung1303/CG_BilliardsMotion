#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

struct LightSource{
	glm::vec4 ambientIntensity;
	glm::vec4 diffuseIntensity;
	glm::vec4 specularIntensity;

	glm::vec3 position;	//in world coordiantes

	glm::vec3 spotDirection;
	float spotExponent;
	float spotCutoff;

	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;

	LightSource();
};

class Light{
public:
//private:
	LightSource* uLight;

	//Light's Uniform Buffer Object
	GLuint uboLight;
public:
	static GLuint uboLightLoc;

	Light();
	virtual ~Light();

	void clear();
	void setLightSource(const LightSource* lightSource);
	GLuint getUBOLight() const;
};
