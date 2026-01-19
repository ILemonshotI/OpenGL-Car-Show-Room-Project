#ifndef buildingEBO_CLASS_H
#define buildingEBO_CLASS_H

#include <glad/glad.h>

class buildingEBO {
public:
	GLuint ID;

	buildingEBO();
	buildingEBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif
