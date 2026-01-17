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

#include <filesystem>
namespace fs = std::filesystem;

const unsigned int width = 800;
const unsigned int height = 800;

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

// Function to get bounding box from a model (adjust values based on your model size)
Camera::BoundingBox getModelBoundingBox(const glm::vec3& position, const glm::vec3& modelSize = glm::vec3(2.0f, 1.0f, 4.0f)) {
    Camera::BoundingBox box;
    box.min = position - modelSize * 0.5f;
    box.max = position + modelSize * 0.5f;
    return box;
}

// Function to get bounding box from your custom Car object
Camera::BoundingBox getCarBoundingBox(Car& car, float extraMargin = 0.3f) {
    glm::vec3 carPos = car.getPosition(); // Assuming Car has getPosition()
    glm::vec3 carSize = glm::vec3(2.0f, 1.5f, 4.5f) + glm::vec3(extraMargin); // Adjust for your car size
    Camera::BoundingBox box;
    box.min = carPos - carSize * 0.5f;
    box.max = carPos + carSize * 0.5f;
    return box;
}

// Function to get bounding box from Cube
Camera::BoundingBox getCubeBoundingBox(Cube& cube, float extraMargin = 0.3f) {
    glm::vec3 cubePos = cube.getPosition(); // Assuming Cube has getPosition()
    glm::vec3 cubeSize = glm::vec3(1.0f, 1.0f, 1.0f) * 5.0f + glm::vec3(extraMargin); // Your cube is scaled 5x
    Camera::BoundingBox box;
    box.min = cubePos - cubeSize * 0.5f;
    box.max = cubePos + cubeSize * 0.5f;
    return box;
}

int main() {
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

    // Create camera
    Camera camera(width, height, glm::vec3(0.0f, 3.0f, 5.0f));

    // **SET UP WORLD BOUNDARIES** - Define your showroom limits
    camera.setWorldBounds(Camera::BoundingBox(
        glm::vec3(-15.0f, 0.0f, -15.0f),   // min: x, y, z
        glm::vec3(15.0f, 10.0f, 15.0f)     // max: x, y, z
    ));

    // **ADJUST CAMERA SIZE** - Make collision box match player/camera size
    camera.cameraSize = glm::vec3(0.8f, 1.8f, 0.8f); // Width, Height, Depth

    // Generate shaders
    Shader modelShader(
        "Resource Files/shaders/model_shader.vert",
        "Resource Files/shaders/model_shader.frag"
    );
    Shader carShader(
        "Resource Files/shaders/car_shader.vert",
        "Resource Files/shaders/car_shader.frag"
    );

    // Init textures
    Texture messi("Resource Files/textures/painted_metal_shutter.png");
    Texture wheelTex("Resource Files/textures/wheel2.png");

    // Init models
    modelModel carModel("Resource Files/3d_models/audi_rs5/scene.gltf");
    modelModel car1Model("Resource Files/3d_models/lamborghini_centenario/scene.gltf");
    modelModel car2Model("Resource Files/3d_models/mclaren_sienna/scene.gltf");

    // Init custom objects
    Car myCar(&carShader, &messi, &wheelTex, glm::vec3(6, 0, 0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    myCar.useTexture = true;

    Cube cube(&carShader, &messi, glm::vec3(-6, 0, 0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    cube.useTexture = true;

    // **ADD OBSTACLES (CARS AND CUBE) TO CAMERA'S COLLISION SYSTEM**

    // 1. Add center car (from model) as obstacle
    glm::vec3 centerCarPos = glm::vec3(0, 0, 0);
    camera.addObstacle(getModelBoundingBox(centerCarPos, glm::vec3(2.0f, 1.2f, 4.5f)));

    // 2. Add right car (from model) as obstacle
    glm::vec3 rightCarPos = glm::vec3(3, 0, 0);
    camera.addObstacle(getModelBoundingBox(rightCarPos, glm::vec3(2.2f, 1.3f, 4.7f)));

    // 3. Add left car (from model) as obstacle
    glm::vec3 leftCarPos = glm::vec3(-3, 0, 0);
    camera.addObstacle(getModelBoundingBox(leftCarPos, glm::vec3(2.1f, 1.1f, 4.6f)));

    // 4. Add custom Car as obstacle
    camera.addObstacle(getCarBoundingBox(myCar, 0.5f));

    // 5. Add cube as obstacle (your cube is scaled 5x)
    camera.addObstacle(getCubeBoundingBox(cube, 0.3f));

    // 6. Add walls/barriers around the showroom (optional)
    // Left wall
    camera.addObstacle(Camera::BoundingBox(
        glm::vec3(-14.5f, 0.0f, -14.5f),
        glm::vec3(-13.5f, 5.0f, 14.5f)
    ));

    // Right wall
    camera.addObstacle(Camera::BoundingBox(
        glm::vec3(13.5f, 0.0f, -14.5f),
        glm::vec3(14.5f, 5.0f, 14.5f)
    ));

    // Back wall
    camera.addObstacle(Camera::BoundingBox(
        glm::vec3(-14.5f, 0.0f, 13.5f),
        glm::vec3(14.5f, 5.0f, 14.5f)
    ));

    // Front wall (with opening for entrance maybe)
    camera.addObstacle(Camera::BoundingBox(
        glm::vec3(-14.5f, 0.0f, -14.5f),
        glm::vec3(-5.0f, 5.0f, -13.5f)
    ));
    camera.addObstacle(Camera::BoundingBox(
        glm::vec3(5.0f, 0.0f, -14.5f),
        glm::vec3(14.5f, 5.0f, -13.5f)
    ));

    // Lighting setup
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    glEnable(GL_DEPTH_TEST);

    float lastTime = glfwGetTime();

    // Debug: Visualize collision boxes (optional - wireframe mode)
    bool showCollisionBoxes = false;

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Handle inputs
        CarDoorInput(window, camera, myCar);

        // Toggle collision visualization with 'C' key
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

        // Update camera (includes collision detection)
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Clear screen
        glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get view/projection matrices
        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        // Draw custom car
        carShader.Activate();
        glUniform4f(glGetUniformLocation(carShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(carShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        myCar.update(deltaTime);
        myCar.draw(view, projection);

        cube.update(deltaTime);
        cube.draw(view, projection);

        // Draw models
        modelShader.Activate();
        glUniform4f(glGetUniformLocation(modelShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(modelShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        // Reset transformation uniforms
        glm::mat4 identity = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(identity));

        // Draw all models
        carModel.Draw(modelShader, camera, glm::translate(glm::mat4(1.0f), centerCarPos));
        car1Model.Draw(modelShader, camera, glm::translate(glm::mat4(1.0f), rightCarPos));
        car2Model.Draw(modelShader, camera, glm::translate(glm::mat4(1.0f), leftCarPos));

        // Draw collision boxes for debugging (optional)
        if (showCollisionBoxes) {
            // You would need to implement wireframe rendering here
            // This is just a placeholder
            // drawWireframeBox(getModelBoundingBox(centerCarPos), glm::vec3(1,0,0));
            // drawWireframeBox(getModelBoundingBox(rightCarPos), glm::vec3(0,1,0));
            // drawWireframeBox(getModelBoundingBox(leftCarPos), glm::vec3(0,0,1));
        }

        // Debug output (optional - shows camera position)
        static float debugTimer = 0.0f;
        debugTimer += deltaTime;
        if (debugTimer > 1.0f) {
            std::cout << "Camera Position: ("
                << camera.Position.x << ", "
                << camera.Position.y << ", "
                << camera.Position.z << ")" << std::endl;
            debugTimer = 0.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    modelShader.Delete();
    carShader.Delete();
    messi.Delete();
    wheelTex.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}