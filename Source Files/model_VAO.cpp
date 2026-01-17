#include"model_VAO.h"

// Constructor that generates a VAO ID
modelVAO::modelVAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO Attribute such as a position or color to the VAO
void modelVAO::LinkAttrib(modelVBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void modelVAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void modelVAO::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void modelVAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}