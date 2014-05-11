#include "Object.hpp"

#include "shader.hpp"
#include "model3D.hpp"
#include "ResourceManager.hpp"

Object::Object()
{
}


Object::~Object()
{
}


// render the 3D object
void Object::render(glm::mat4 projectionMarix, Camera * camera)
{
	program->useProgram(true);
	model->render(projectionMarix * camera->getViewMatrix());
	//cout << model << endl;
	//cout << projectionMarix << endl;
	//cout << camera->getViewMatrix() << endl;
}


// load Model3D from resourceManager
void Object::loadModel(int modelId, ResourceManager * resourceManager)
{
	model = resourceManager->getModel(modelId);
}


// load Program from resourceManager
void Object::loadProgram(int programId, ResourceManager * resourceManager)
{
	program = resourceManager->getProgram(programId);

	GLuint p = program->getProgram();
	//cout << "Program id when render = " << p << endl;

	Node::uMVPMatrixLoc = glGetUniformLocation(p, "uMVPMatrix");
	Mesh::aPositionLoc = glGetAttribLocation(p, "position");
	Mesh::aTexCoordLoc = glGetAttribLocation(p, "texCoord");
	Mesh::aNormalLoc = glGetAttribLocation(p, "normal");
	Texture::uTextureCountLoc = glGetUniformLocation(p, "texCount");
	Material::uboMaterialLoc = glGetUniformBlockIndex(p, "Material");
	//cout << Node::uMVPMatrixLoc << " " << Mesh::aPositionLoc << " " << Mesh::aTexCoordLoc << " " << Mesh::aNormalLoc << " " << Texture::uTextureCountLoc << " " << Material::uboMaterialLoc << endl;
}


void Object::unload()
{
}

void Object::translate(float x, float y, float z){
	model->translate(glm::vec3(x, y, z));
	position = position + glm::vec3(x, y, z);
} 

void Object::translate(glm::vec3 translateVector){
	model->translate(translateVector);
	position = position + translateVector;
}

void Object::rotate(float angle, float x, float y, float z){
	model->rotate(angle, glm::vec3(x, y, z));
	//TODO handle with position
}

void Object::rotate(float angle, glm::vec3 rotateVector){
	model->rotate(angle, rotateVector);
	//TODO handle with position
}


void Object::scale(float scaleVal){
	model->scale(scaleVal);
	//TODO handle with position
}

void Object::setAnchorPoint(){
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Object::getPosition(){
	return position;
}
