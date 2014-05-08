#include "util.hpp"

bool fileIsExist(const std::string& path){
	struct stat buffer;
	return (stat(path.data(), &buffer) == 0);
}

std::string getSource(const std::string& path){
	if (!fileIsExist(path)){
		fprintf(stderr, "File not found: \"%s\"", path.data());
	}
	std::fstream fin(path);
	if (!fin.is_open()){
		fprintf(stderr, "Error while opening file %s\n", path);
		exit(EXIT_FAILURE);
	}
	std::string shaderSource((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	fin.close();

	return shaderSource;
}

glm::mat4 convertToGLM(const aiMatrix4x4& mat){
	glm::mat4 m;

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			m[i][j] = mat[j][i];
		}
	}

	return m;
}

std::string parsingDirectory(const std::string& path){
	std::string::size_type slashIndex = path.find_last_of("/\\");
	if (slashIndex == std::string::npos){
		return "";
	}
	return path.substr(0, slashIndex);
}

void displayAISceneInfo(const aiScene* scene){
	fprintf(stdout, "mNumAnimations: %d\n", scene->mNumAnimations);
	fprintf(stdout, "mNumCameras: %d\n", scene->mNumCameras);
	fprintf(stdout, "mNumLights: %d\n", scene->mNumLights);
	fprintf(stdout, "mNumMaterials: %d\n", scene->mNumMaterials);
	fprintf(stdout, "mNumMeshes: %d\n", scene->mNumMeshes);
	fprintf(stdout, "mNumTextures: %d\n", scene->mNumTextures);

	for (int i = 0; i < scene->mNumMaterials; i++){
		const aiMaterial* material = scene->mMaterials[i];
		fprintf(stdout, "Material %d\n", i);

		aiColor4D color;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS){
			fprintf(stdout, "Diffuse Color: (%f, %f, %f, %f)\n", color.r, color.g, color.b, color.a);
		}
		else {
			fprintf(stdout, "No Diffuse Color.\n");
		}

		if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS){
			fprintf(stdout, "Ambient Color: (%f, %f, %f, %f)\n", color.r, color.g, color.b, color.a);
		}
		else {
			fprintf(stdout, "No Ambient Color.\n");
		}

		if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS){
			fprintf(stdout, "Specular Color: (%f, %f, %f, %f)\n", color.r, color.g, color.b, color.a);
		}
		else {
			fprintf(stdout, "No Specular Color.\n");
		}

		if (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS){
			fprintf(stdout, "Emissive Color: (%f, %f, %f, %f)\n", color.r, color.g, color.b, color.a);
		}
		else {
			fprintf(stdout, "No Emissive Color.\n");
		}

		fprintf(stdout, "_______________________________________________________\n");

		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS){
			fprintf(stdout, "Diffuse Texture: %s\n", path.C_Str());
		}
		else {
			fprintf(stdout, "No Diffuse Texture.\n");
		}

		if (material->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS){
			fprintf(stdout, "Ambient Texture: %s\n", path.data);
		}
		else {
			fprintf(stdout, "No Ambient Texture.\n");
		}

		if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS){
			fprintf(stdout, "Specular Texture: %s\n", path.data);
		}
		else {
			fprintf(stdout, "No Specular Texture.\n");
		}

		if (material->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS){
			fprintf(stdout, "Emissive Texture: %s\n", path.data);
		}
		else {
			fprintf(stdout, "No Emissive Texture.\n");
		}
		fprintf(stdout, "=========================================================\n");
	}

}

std::ostream& operator<<(std::ostream& os, const glm::mat4 matrix){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			os << std::setw(10) << std::setprecision(5) << matrix[i][j];
		}
		os << std::endl;
	}
	return os;
}