#include "texture.hpp"

GLuint Texture::uTextureCountLoc = 0;

Texture::Texture(){
	clear();
}

Texture::~Texture(){
	clear();
}

void Texture::clear(){
	if (texUnit != NULL){
		glDeleteTextures(1, &texUnit);
	}
	texUnit = NULL;
}

bool Texture::loadTexture(const std::string& path){
#ifdef PRINT_LOADING
	fprintf(stdout, "Loading texture : \"%s\" ...\n", path.data());
#endif
	if (!fileIsExist(path)){
		fprintf(stderr, "File not Found!\n");
		return false;
	}

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	unsigned int imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	if (!ilLoadImage(path.data())){
		ilDeleteImages(1, &imageID);
		fprintf(stderr, "Error while read texture.\n");
		exit(EXIT_FAILURE);
	}

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)){
		ilDeleteImages(1, &imageID);
		fprintf(stderr, "Error while convert texture");
		exit(EXIT_FAILURE);
	}

	glGenTextures(1, &texUnit);
	glBindTexture(GL_TEXTURE_2D, texUnit);
	glTexImage2D(GL_TEXTURE_2D, 0,
		ilGetInteger(IL_IMAGE_FORMAT),
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		ilGetData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, NULL);

	ilDeleteImage(imageID);

#ifdef PRINT_LOADING
	fprintf(stdout, "Loading texture DONE.\n");
	fprintf(stdout, "____________________________________________________________\n");
#endif
	return true;
}

GLuint Texture::getTextureUnit() const{
	return this->texUnit;
}