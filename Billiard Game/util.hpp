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

bool fileIsExist(const std::string& path);

std::string getSource(const std::string& path);

glm::mat4 convertToGLM(const aiMatrix4x4& mat);

std::string parsingDirectory(const std::string& path);

void displayAISceneInfo(const aiScene* scene);

std::ostream& operator<<(std::ostream& os, const glm::mat4 matrix);