#ifndef buildingTEXTURE_CLASS_H
#define buildingTEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>
#include <string>
#include <vector>

#include"Shader.hpp"

class buildingTexture
{
public:
	GLuint ID;
	GLenum type;
	GLuint unit;
	GLuint target;

	buildingTexture();

	buildingTexture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	buildingTexture(std::vector<std::string>  faces, GLuint slot);

	buildingTexture(GLuint existingID, GLenum texType, GLuint slot);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
private:
	void loadCubemap(std::vector<std::string> faces);
};
#endif