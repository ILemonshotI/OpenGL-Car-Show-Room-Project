#ifndef buildingVAO_CLASS_H
#define buildingVAO_CLASS_H

#include <glad/glad.h>
#include "building_VBO.h"

class buildingVAO {
public:
	GLuint ID;
	buildingVAO();

	void LinkAttrib(buildingVBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif