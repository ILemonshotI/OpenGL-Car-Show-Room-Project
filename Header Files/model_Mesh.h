#ifndef MODEL_MESH_CLASS_H
#define MODEL_MESH_CLASS_H

#include<string>

#include"model_VAO.h"
#include"model_EBO.h"
#include"Camera.h"
#include"model_Texture.h"

class modelMesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <modelTexture> textures;
	// Store VAO in public so it can be used in the Draw function
	modelVAO VAO;

	// Initializes the mesh
	modelMesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <modelTexture>& textures);

	// Draws the mesh
	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};
#endif