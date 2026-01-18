// main.cpp
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Cube.hpp"
#include "Cylinder.hpp"
#include "Car.hpp"
#include "CarDoor.hpp"
#include "CarFront.hpp"
#include "Material.hpp"
#include "Camera.h"
#include "model_Model.h"
#include "model_Transform.h"

#include <filesystem>
namespace fs = std::filesystem;

// ---------------------------------------------------------
// 1. CONSTANTS AND CONFIGURATION
// ---------------------------------------------------------
const unsigned int width = 800;
const unsigned int height = 800;

// ---------------------------------------------------------
// 2. CALLBACKS AND HELPER FUNCTIONS
// ---------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void CarDoorInput(GLFWwindow* window, Camera& camera, Car& mycar) {
    static bool gWasPressed = false;
    bool gPressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;

    if (gPressed && !gWasPressed) {
        mycar.ToggleDriverDoor(camera.Position);
    }
    gWasPressed = gPressed;
}

int main() {
    // ---------------------------------------------------------
    // 3. GLFW & GLAD INITIALIZATION
    // ---------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Car Show Room", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, width, height);

    // ---------------------------------------------------------
    // 4. CAMERA AND WORLD BOUNDARIES
    // ---------------------------------------------------------
    Camera camera(width, height, glm::vec3(0.0f, 3.0f, 5.0f));

    camera.setWorldBounds(Camera::BoundingBox(
        glm::vec3(-15.0f, 0.0f, -15.0f),
        glm::vec3(15.0f, 10.0f, 15.0f)
    ));

    camera.cameraSize = glm::vec3(0.5f, 1.6f, 0.5f);

    // ---------------------------------------------------------
    // 5. SHADER SETUP
    // ---------------------------------------------------------
    Shader modelShader(
        "Resource Files/shaders/model_shader.vert",
        "Resource Files/shaders/model_shader.frag"
    );
    Shader carShader(
        "Resource Files/shaders/car_shader.vert",
        "Resource Files/shaders/car_shader.frag"
    );

    // ---------------------------------------------------------
    // 6. TEXTURE SETUP
    // ---------------------------------------------------------
    Texture messi("Resource Files/textures/painted_metal_shutter.png");
    Texture wheelTex("Resource Files/textures/wheel2.png");

    // ---------------------------------------------------------
    // 7. 3D MODEL & CUSTOM OBJECT LOADING
    // ---------------------------------------------------------
    modelModel carModel("Resource Files/3d_models/mercedes_c63/scene.gltf");
    modelModel car1Model("Resource Files/3d_models/porsche_911/scene.gltf");
    modelModel car2Model("Resource Files/3d_models/mclaren_sienna/scene.gltf");

    Car myCar(&carShader, &messi, &wheelTex, glm::vec3(6, 0, 0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    myCar.useTexture = true;

    Cube cube(&carShader, &messi, glm::vec3(-6, 0, 0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    cube.useTexture = true;

    // ---------------------------------------------------------
    // 8. TRANSFORMS AND BASE SCALING
    // ---------------------------------------------------------
    Transform centerCarTransform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.5f));
    Transform rightCarTransform(glm::vec3(3, 0, 2), glm::vec3(0, 45.0f, 0), glm::vec3(0.5f));
    Transform leftCarTransform(glm::vec3(-3, 0, -2), glm::vec3(0, -30.0f, 0), glm::vec3(0.5f));

    myCar.baseSize = glm::vec3(2.0f, 1.0f, 1.5f);
    cube.baseSize = glm::vec3(1.0f, 1.0f, 1.0f);
    cube.setScale(glm::vec3(2.0f, 2.0f, 2.0f));

    // ---------------------------------------------------------
    // 9. LIGHTING AND GLOBAL RENDER STATE
    // ---------------------------------------------------------
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    glEnable(GL_DEPTH_TEST);
    float lastTime = glfwGetTime();

    // ---------------------------------------------------------
    // 10. COLLISION & OBSTACLE REGISTRATION
    // ---------------------------------------------------------
    camera.addObstacle(myCar.getBoundingBox());
    camera.addObstacle(cube.getBoundingBox());
    camera.addObstacle(carModel.getBoundingBox(centerCarTransform.getModelMatrix()));
    camera.addObstacle(car1Model.getBoundingBox(rightCarTransform.getModelMatrix()));
    camera.addObstacle(car2Model.getBoundingBox(leftCarTransform.getModelMatrix()));

    // Showroom walls
    camera.addObstacle({ glm::vec3(-14.5f,0,-14.5f), glm::vec3(-13.5f,5,14.5f) });
    camera.addObstacle({ glm::vec3(13.5f,0,-14.5f), glm::vec3(14.5f,5,14.5f) });
    camera.addObstacle({ glm::vec3(-14.5f,0,13.5f), glm::vec3(14.5f,5,14.5f) });
    camera.addObstacle({ glm::vec3(-14.5f,0,-14.5f), glm::vec3(-5.0f,5,-13.5f) });
    camera.addObstacle({ glm::vec3(5.0f,0,-14.5f), glm::vec3(14.5f,5,-13.5f) });

    bool showCollisionBoxes = false;

    // ---------------------------------------------------------
    // 11. MAIN RENDER LOOP
    // ---------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        // --- A. Time and Logic Update ---
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // --- B. Input Handling ---
        CarDoorInput(window, camera, myCar);

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            static bool cWasPressed = false;
            if (!cWasPressed) {
                showCollisionBoxes = !showCollisionBoxes;
                std::cout << "Collision boxes: " << (showCollisionBoxes ? "ON" : "OFF") << std::endl;
            }
            cWasPressed = true;
        }
        else {
            static bool cWasPressed = false;
            cWasPressed = false;
        }

        // --- C. Camera & Physics Update ---
        camera.obstacles[0] = myCar.getBoundingBox();
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // --- D. Clear Buffers ---
        glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        // --- E. Draw 3D Models ---
        modelShader.Activate();
        glUniform4f(glGetUniformLocation(modelShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(modelShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        glm::mat4 identity = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(identity));

        carModel.Draw(modelShader, camera, centerCarTransform.getModelMatrix());
        car1Model.Draw(modelShader, camera, rightCarTransform.getModelMatrix());
        car2Model.Draw(modelShader, camera, leftCarTransform.getModelMatrix());

        // --- F. Draw Custom Objects (Last) ---
        carShader.Activate();
        glUniform4f(glGetUniformLocation(carShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(carShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        cube.update(deltaTime);
        cube.draw(view, projection);

        myCar.update(deltaTime);
        myCar.draw(view, projection); // Drawing myCar last

        // --- G. Debug Visuals ---
        if (showCollisionBoxes) {
            // Implement wireframe rendering here if needed
        }

        static float debugTimer = 0.0f;
        debugTimer += deltaTime;
        if (debugTimer > 1.0f) {
            std::cout << "Camera Position: (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ")" << std::endl;
            debugTimer = 0.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---------------------------------------------------------
    // 12. CLEANUP / MEMORY MANAGEMENT
    // ---------------------------------------------------------
    modelShader.Delete();
    carShader.Delete();
    messi.Delete();
    wheelTex.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}