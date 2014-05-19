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
#include <vector>
#include <assert.h>
#include <sys/stat.h>

#define PRINT_LOADING
//#define PRINT_LIGHT_SOURCE
//#define PRINT_LIGHT_MATERIAL
//#define PRINT_CAMERA_POSITION
//#define PRINT_UNIFORM

//Return variable name of var
#define VAR_NAME(var) #var
#define EPSILON 1e-5

#define PARSING_DIRECTORY		0x1
#define PARSING_FILE_NAME		0x2
#define PARSING_FILE_EXTENSION	0x4

enum Coordinates{
	MODEL_COORINATES,
	WORLD_COORDINATES,
	VIEW_COORDINATES,
};

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

glm::vec3 extractScaling(const glm::mat4& matrix);
glm::vec3 extractTranslation(const glm::mat4& matrix);
void extractAngleAndAxis(const glm::mat4& matrix, float& angle, glm::vec3& axis);

/**
* A yaw is a counterclockwise rotation of $alpha about the z-axis	Rz(yaw)
* A pitch is a counterclockwise rotation of $beta about the y-axis	Ry(pitch)
* A roll is a counterclockwise rotation of $gamma about the x-axis	Rx(roll)
* The yaw, pitch, and roll rotations can be used to place a 3D body in any orientation.
* R = Rz(yaw) * Ry(pitch) * Rx(roll)
*/
void extractYawPitchRoll(const glm::mat4& matrix, float& yaw, float& pitch, float& roll);

/**
* Decomposes matrix M such that T * R * S = M, where:
*	T is translation matrix,
*	R is rotation matrix and
*	S is scaling matrix.
* http://code.google.com/p/assimp-net/source/browse/trunk/AssimpNet/Matrix4x4.cs
* (this method is exact to at least 0.0001f)
*
* | 1  0  0  Tx | | R11 R12 R13 0 | | Sx 0  0  0 |   | SxR11 SyR12 SzR13 Tx |
* | 0  1  0  Ty |.| R21 R22 R23 0 |.| 0  Sy 0  0 | = | SxR21 SyR22 SzR23 Ty |
* | 0  0  0  Tz | | R31 R32 R33 0 | | 0  0  Sz 0 |   | SxR31 SyR32 SzR33 Tz |
* | 0  0  0   1 | |  0   0   0  1 | | 0  0  0  1 |   |   0     0     0    1 |
*
* @param matrix (in) matrix to decompose
* @param scaling (out) scaling vector
* @param rotation (out) rotation matrix
* @param translation (out) translation vector
*/
void decomposeTRS(const glm::mat4& matrix, glm::vec3& scaling,
	glm::mat4& rotation, glm::vec3& translation);