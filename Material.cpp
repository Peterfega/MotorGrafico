#include "Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void Material::loadImage() {
	int comp = 0;
	stbi_set_flip_vertically_on_load(true);
	this->data = (unsigned char*)stbi_load(this->textureName.c_str(), &this->width, &this->height, &comp, 4);
	if (this->data != nullptr)
	{
		glGenTextures(1, &this->textureId);
		glBindTexture(GL_TEXTURE_2D, this->textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data);
		this->enabled = 1;
	}
}
void Material::materialBind(unsigned int textureUnitId) {
	glActiveTexture(GL_TEXTURE0 + textureUnitId);
	glBindTexture(GL_TEXTURE_2D, this->textureId);
}

Material::Material(std::string textureName) {
	this->textureName = textureName;
	this->textureId = -1;
	this->enabled = 0;
	this->width = 0;
	this->height = 0;
	this->data = nullptr;
	loadImage();

}