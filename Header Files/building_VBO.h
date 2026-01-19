#ifndef buildingVBO_CLASS_H
#define buildingVBO_CLASS_H

#include <glad/glad.h>

class buildingVBO {
public:
	GLuint ID;

	buildingVBO();
	buildingVBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif