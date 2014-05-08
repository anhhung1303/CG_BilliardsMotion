#include "model3D.hpp"

Model3D::Model3D(){
	clear();
}

Model3D::~Model3D(){
	clear();
}

void Model3D::clear(){
	for (std::vector<Mesh*>::iterator it = meshes.begin();
		it != meshes.end(); it++){
		delete (*it);
	}
	for (std::vector<Material*>::iterator it = materials.begin();
		it != materials.end(); it++){
		delete (*it);
	}
	for (std::map<std::string, Texture*>::iterator it = textures.begin();
		it != textures.end(); it++){
		delete (it->second);
	}
	meshes.clear();
	materials.clear();
	textures.clear();
}

bool Model3D::loadModel(const std::string& path){
	clear();

	if (!fileIsExist(path)){
		printf("File not fount.\n");
		exit(EXIT_FAILURE);
	}

	static Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(path, aiProcessPreset_TargetRealtime_Quality);
	if (scene == NULL){
		fprintf(stderr, "Error parsing '%s': '%s'\n", path, imp.GetErrorString());
		exit(EXIT_FAILURE);
	} else {
		fprintf(stdout, "Loading model \"%s\" done.\n", path.data());
	}

	importMaterials(scene);
	importTextures(scene, path);
	importMeshes(scene);
	importNodes(scene);

	printf("Mesh %d\n", meshes.size());
	printf("Material %d\n", materials.size());
	printf("Texture %d\n", textures.size());

	return true;
}

void Model3D::importMeshes(const aiScene* scene){
	if (!scene->HasMeshes()){
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++){
		Mesh* mes = new Mesh();
		mes->setMesh(scene->mMeshes[i]);

		int matIndex = scene->mMeshes[i]->mMaterialIndex;
		mes->setMaterial(materials[matIndex]);

		aiString file;
		if (scene->mMaterials[matIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == aiReturn_SUCCESS){
			mes->setTexture(textures[file.data]);
		}

		meshes.push_back(mes);
	}
}

void Model3D::importMaterials(const aiScene* scene){
	if (!scene->HasMaterials()){
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMaterials; i++){
		Material* mat = new Material();
		mat->setMaterial(scene->mMaterials[i]);

		materials.push_back(mat);
	}
}

void Model3D::importTextures(const aiScene* scene, const std::string& path){
	std::string dir = parsingDirectory(path);
	aiString file;

	for (unsigned int i = 0; i < scene->mNumMaterials; i++){
		aiMaterial* aiMat = scene->mMaterials[i];
		
		if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &file) == aiReturn_SUCCESS){
			Texture* tex = new Texture();
			tex->loadTexture(dir + "/" + file.data);

			textures[file.data] = tex;
		}
	}
}

void Model3D::importNodes(const aiScene* scene){
	root = builtNode(scene->mRootNode);
}

Node* Model3D::builtNode(const aiNode* node){
	Node* not = new Node();

	not->modelMatrix = convertToGLM(node->mTransformation);
	for (unsigned int i = 0; i < node->mNumMeshes; i++){
		int mesIndex = node->mMeshes[i];
		not->content.push_back(meshes[mesIndex]);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++){
		Node* child = builtNode(node->mChildren[i]);
		not->children.push_back(child);
	}
	return not;
}

void Model3D::render(const glm::mat4& preMatrix){
	draw(root, preMatrix);
}

void Model3D::draw(Node* node, const glm::mat4& preMatrix){
	glm::mat4 curMatrix = preMatrix * node->modelMatrix;
	std::cout << preMatrix;
	glUniformMatrix4fv(Node::uMVPMatrixLoc, 1, GL_FALSE, glm::value_ptr(curMatrix));

	for (Mesh* mes : node->content){
		const Material* mat = mes->getMaterial();
		const Texture* tex = mes->getTexture();

		if (mat != NULL){
			glBindBufferRange(GL_UNIFORM_BUFFER, Material::uMaterialBlockLoc,
				mat->getMaterialBlockBuffer(), 0, sizeof(UniformMaterialBlock));
		}
		if (tex != NULL){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex->getTextureUnit());
		}
		glBindVertexArray(mes->getVertexBufferObject());

		glDrawElements(GL_TRIANGLES, mes->getNumFaces() * 3, GL_UNSIGNED_INT, 0);

		if (mat != NULL){
		}
		if (tex != NULL){
			glBindTexture(GL_TEXTURE_2D, NULL);
		}
	}

	for (Node* child : node->children){
		draw(child, curMatrix);
	}
}