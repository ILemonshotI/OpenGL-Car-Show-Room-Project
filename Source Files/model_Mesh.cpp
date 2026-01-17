#include "model_Mesh.h"

modelMesh::modelMesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <modelTexture>& textures)
{
	modelMesh::vertices = vertices;
	modelMesh::indices = indices;
	modelMesh::textures = textures;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	modelVBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	modelEBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void modelMesh::Draw(
    Shader& shader,
    Camera& camera,
    glm::mat4 matrix,
    glm::vec3 translation,
    glm::quat rotation,
    glm::vec3 scale
)
{
    shader.Activate();
    VAO.Bind();

    // --- Determine if mesh is transparent ---
    bool transparent = false;

    // Check diffuse textures for alpha channel (RGBA)
    for (auto& tex : textures)
    {
        if (tex.type == "diffuse" && tex.hasAlpha) // you need to add hasAlpha in modelTexture
        {
            transparent = true;
            break;
        }
    }

    if(transparent)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE); // don't write to depth buffer
    }

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;
        if(type == "diffuse")
            num = std::to_string(numDiffuse++);
        else if(type == "specular")
            num = std::to_string(numSpecular++);

        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 rot   = glm::mat4_cast(rotation);
    glm::mat4 sca   = glm::scale(glm::mat4(1.0f), scale);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));


    if (transparent)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE); // don't write to depth buffer while blending
    }


    // Draw mesh
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


    if (transparent)
    {
        glDepthMask(GL_TRUE); // restore depth writing
        glDisable(GL_BLEND);  // disable blending for next opaque meshes
    }


    // --- Restore state ---
    if(transparent)
    {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
}
