#include "building_VBO.h"

buildingVBO::buildingVBO() {
	ID = 0;
}

buildingVBO::buildingVBO(GLfloat* vertices, GLsizeiptr size) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void buildingVBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void buildingVBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void buildingVBO::Delete() {
	glDeleteBuffers(1, &ID);
}