#include "light.hpp"

GLuint Light::uboLightLoc = 2;

LightSource::LightSource(){
	ambientIntensity = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	diffuseIntensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	specularIntensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	spotDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	spotExponent = 0.0f;
	spotCutoff = 180.0f;
	attenuationConstant = 1.0f;
	attenuationLinear = 0.0f;
	attenuationQuadratic = 0.0f;
}

Light::Light(){
	uLight = new LightSource();
}

Light::~Light(){
	clear();
}

void Light::clear(){
	glDeleteBuffers(1, &uboLight);
	if (uLight != NULL){
		delete uLight;
		uLight = NULL;
	}
}

void Light::setLightSource(const LightSource* lightSource){
	clear();
	this->uLight = new LightSource(*lightSource); //clone object

	glGenBuffers(1, &uboLight);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLight);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightSource), (void*) uLight, GL_STATIC_DRAW);
	//glBindBufferRange(GL_UNIFORM_BUFFER, uboLightLoc, uboLight, 0, sizeof(LightSource));
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}

GLuint Light::getUBOLight() const{
	return this->uboLight;
}