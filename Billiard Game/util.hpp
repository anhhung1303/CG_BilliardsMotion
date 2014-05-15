#pragma once

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <sys/stat.h>

#define PRINT_LOADING
//#define PRINT_LIGHT_SOURCE
//#define PRINT_LIGHT_MATERIAL
//#define PRINT_CAMERA_POSITION

//Return variable name of var
#define VAR_NAME(var) #var
#define PARSING_DIRECTORY		0x1
#define PARSING_FILE_NAME		0x2
#define PARSING_FILE_EXTENSION	0x4

const static glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
const static glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
const static glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
const static glm::vec2 zeroVec2(0.0f);
const static glm::vec3 zeroVec3(0.0f);
const static glm::vec4 zeroVec4(0.0f);
const static glm::mat3 idMat3(1.0f);
const static glm::mat4 idMat4(1.0f);

bool fileIsExist(const std::string& path);

std::string getSource(const std::string& path);

glm::mat4 convertToGLM(const aiMatrix4x4& mat);

std::string parsingURL(const std::string& path, int urlPasingMode);

void displayAISceneInfo(const aiScene* scene);

std::ostream& operator<<(std::ostream& os, const glm::mat4& matrix);
std::ostream& operator<<(std::ostream& os, const aiMatrix4x4& matrix);
std::ostream& operator<<(std::ostream& os, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& os, const glm::vec4& vector);
std::ostream& operator<<(std::ostream& os, const aiVector3D& vector);
std::ostream& operator<<(std::ostream& os, const aiColor4D& vector);

extern struct LightSource;
extern struct LightMaterial;
std::ostream& operator<<(std::ostream& os, const LightSource& ls);
std::ostream& operator<<(std::ostream& os, const LightMaterial& met);