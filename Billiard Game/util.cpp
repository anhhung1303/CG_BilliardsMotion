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

std::string parsingURL(const std::string& path, int urlPasingMode){
	std::string::size_type slashIndex = path.find_last_of("/\\");

	//Checking that dot is separator extension or not
	auto checkExtensionDot = [&](std::string::size_type dotIndex)->bool{
		int first = dotIndex, last = dotIndex;
		while (int(first) >= 0 && path[first] == '.') first--;
		while (int(last) < path.length() && path[last] == '.') last++;

		int flag = 0;
		if ((first < 0) || (path[first] == '\\') || (path[first] == '/')) flag++;
		if ((last >= path.length()) || (path[last] == '\\') || (path[last] == '/')) flag++;
		return flag != 2;
	};

	std::string::size_type dotIndex = path.find_last_of(".");
	if (!checkExtensionDot(dotIndex)) dotIndex = std::string::npos;

	std::string dir = (dotIndex == std::string::npos) ? path :
		((slashIndex == std::string::npos) ? "" : path.substr(0, slashIndex));
	if ((!dir.empty()) && ((dir.front() == '\\') || (dir.front() == '/'))){
		dir.erase(0, 1);
	}

	std::string ext = (dotIndex == std::string::npos) ? "" : path.substr(dotIndex + 1);
	std::string file_name = path.substr(dir.length(), path.length() - ext.length() - dir.length());
	if ((!file_name.empty()) && ((file_name.front() == '\\') || (file_name.front() == '/'))){
		file_name.erase(0, 1);
	}
	if ((!file_name.empty()) && (file_name.back() == '.')){
		file_name.erase(file_name.length() - 1);
	}

	std::string result = "";
	if ((urlPasingMode & PARSING_DIRECTORY) != 0x0){
		result += dir;
	}
	if ((urlPasingMode & PARSING_FILE_NAME) != 0x0){
		if (!result.empty()) result += '/';
		result += file_name;
	}
	if ((urlPasingMode & PARSING_FILE_EXTENSION) != 0x0){
		if (!result.empty()) result += '.'; 
		result += ext;
	}
	return result;
}

void displayAISceneInfo(const aiScene* scene){
	fprintf(stdout, "mNumAnimations: %d\n", scene->mNumAnimations);
	fprintf(stdout, "mNumCameras: %d\n", scene->mNumCameras);
	fprintf(stdout, "mNumLights: %d\n", scene->mNumLights);
	fprintf(stdout, "mNumMaterials: %d\n", scene->mNumMaterials);
	fprintf(stdout, "mNumMeshes: %d\n", scene->mNumMeshes);
	fprintf(stdout, "mNumTextures: %d\n", scene->mNumTextures);

	for (unsigned int i = 0; i < scene->mNumMaterials; i++){
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

std::ostream& operator<<(std::ostream& os, const glm::mat4& matrix){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			os << std::setw(10) << std::setprecision(5) << matrix[i][j];
		}
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const aiMatrix4x4& matrix){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			os << std::setw(10) << std::setprecision(5) << matrix[i][j];
		}
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vector){
	os << vector[0] << ", " << vector[1] << ", " << vector[2];
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vector){
	os << vector[0] << ", " << vector[1] << ", " << vector[2] << ", " << vector[3];
	return os;
}

std::ostream& operator<<(std::ostream& os, const aiVector3D& vector){
	os << vector[0] << ", " << vector[1] << ", " << vector[2];
	return os;
}

std::ostream& operator<<(std::ostream& os, const aiColor4D& vector){
	os << vector[0] << ", " << vector[1] << ", " << vector[2] << ", " << vector[3];
	return os;
}

#include "material.hpp"
std::ostream& operator<<(std::ostream& os, const LightMaterial& met){
	os << "Material:\n";
	os << "   + ambient:\t" << met.ambient << std::endl;
	os << "   + diffuse:\t" << met.diffuse << std::endl;
	os << "   + specular:\t" << met.specular << std::endl;
	os << "   + shininess:\t" << met.shininess << std::endl;
	os << "   + emissive:\t" << met.emissive << std::endl;
	return os;
}

#include "light.hpp"
std::ostream& operator<<(std::ostream& os, const LightSource& ls){
	os << "Light:\n";
	os << "   - ambient:\t" << ls.ambientIntensity << std::endl;
	os << "   - diffuse:\t" << ls.diffuseIntensity << std::endl;
	os << "   - specular:\t" << ls.specularIntensity << std::endl;
	os << "   - position:\t" << ls.position << std::endl;
	os << "   - attenuation:\n";
	os << "      + Constant:\t" << ls.attenuationConstant << std::endl;
	os << "      + Linear:\t" << ls.attenuationLinear << std::endl;
	os << "      + Quadratic:\t" << ls.attenuationQuadratic << std::endl;
	return os;
}