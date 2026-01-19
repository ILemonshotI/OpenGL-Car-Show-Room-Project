#include "building_VAO.h"

buildingVAO::buildingVAO() {
	glGenVertexArrays(1, &ID);
}

void buildingVAO::LinkAttrib(buildingVBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void buildingVAO::Bind() {
	glBindVertexArray(ID);
}

void buildingVAO::Unbind() {
	glBindVertexArray(0);
}

void buildingVAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}