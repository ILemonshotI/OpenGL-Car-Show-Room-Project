#ifndef MODEL_VAO_CLASS_H
#define MODEL_VAO_CLASS_H

#include<glad/glad.h>
#include"model_VBO.h"

class modelVAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	modelVAO();

	// Links a VBO Attribute such as a position or color to the VAO
	void LinkAttrib(modelVBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};

#endif