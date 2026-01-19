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
#include "Chair.hpp"
#include "Couch.hpp"
#include "Table.hpp"
#include "SlidingDoor.hpp"
#include "Podium.hpp"
#include "Frame.hpp"

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

void EnterExitCarInput(GLFWwindow* window, Camera& camera, Car& car)
{
    static bool eWasPressed = false;
    bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    if (ePressed && !eWasPressed)
    {
        // ---- ENTER CAR ----
        if (!car.isDriven)
        {
            if (camera.distanceTo(car.position) < 3.0f &&
                car.isDriverDoorOpen())
            {
                car.setDriven(true);
            }
        }
        // ---- EXIT CAR ----
        else
        {
            car.setDriven(false);

            // ?? MOVE CAMERA SLIGHTLY OUTSIDE THE CAR
            camera.Position += glm::vec3(0.0f, 0.0f, 1.8f);
        }
    }

    eWasPressed = ePressed;
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
    Texture leather("Resource Files/textures/79_leather texture-seamless.jpg");
    Texture leather1("Resource Files/textures/Leather.jpg");
    Texture merc("Resource Files/textures/mercedes-logo-mercedes-benz-logos-vector-eps-cdr-svg-download-15.png");
    Texture CLS(
        "Resource Files/textures/CLS.png");
   


    // ---------------------------------------------------------
    // 7. 3D MODEL & CUSTOM OBJECT LOADING
    // ---------------------------------------------------------
    modelModel carModel("Resource Files/3d_models/office_table/scene.gltf");
    modelModel car1Model("Resource Files/3d_models/porsche_911/scene.gltf");
    modelModel car2Model("Resource Files/3d_models/mclaren_sienna/scene.gltf");

    Car myCar(&carShader, &messi, &wheelTex, &leather, &leather1, &merc, glm::vec3(0, 0.45f, 0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    myCar.useTexture = true;


    Car limo(&carShader, &messi, &wheelTex, &leather, &leather1, &merc, glm::vec3(-10, 0, 0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    limo.scaleBy(glm::vec3(1.7, 0.8, 1));
    limo.useTexture = true;

    Chair chair(&carShader, &messi, glm::vec3(-4, 0, 8), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    chair.useTexture = true;

    Couch couch(&carShader, &leather1, glm::vec3(-8, 0, 8), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    couch.useTexture = true;

    Table table(&carShader, &messi, glm::vec3(-6, 0, 8), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    table.useTexture = true;

    SlidingDoor door(&carShader, &messi, glm::vec3(0, 0, 10), 3.0f, 4.0f, 0.2f);

    Frame frame(&carShader, &CLS, glm::vec3(0, 3, -13.4), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    frame.useTexture = 1;

    Podium podium(1.5f, 1.5f, 0.4, 36, &carShader, &messi, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0.0f, 1.0f); //IF SIZE IS CHANGED CHANNGE BASE SIZE
    podium.useTexture = true;



    // ---------------------------------------------------------
    // 8. TRANSFORMS AND BASE SCALING
    // ---------------------------------------------------------
    Transform centerCarTransform(glm::vec3(8, 0, -5), glm::vec3(0, 0, 0), glm::vec3(1.0f));
    Transform rightCarTransform(glm::vec3(8, 0, 0), glm::vec3(0, 45.0f, 0), glm::vec3(0.5f));
    Transform leftCarTransform(glm::vec3(8, 0, 5), glm::vec3(0, -30.0f, 0), glm::vec3(0.5f));

    myCar.baseSize = glm::vec3(2.0f, 1.0f, 1.5f);
    chair.baseSize = glm::vec3(0.44f, 1.1f, 0.4f);
    couch.baseSize = glm::vec3(1.2f, 0.9f, 0.4f);
    table.baseSize = glm::vec3(0.44f, 0.45f, 0.4f);
    podium.baseSize = podium.getCalculatedBaseSize();
    

    // ---------------------------------------------------------
    // 9. LIGHTING AND GLOBAL RENDER STATE
    // ---------------------------------------------------------
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float lastTime = glfwGetTime();

    // ---------------------------------------------------------
    // 10. COLLISION & OBSTACLE REGISTRATION
    // ---------------------------------------------------------
    camera.addObstacle(chair.getBoundingBox());
    camera.addObstacle(table.getBoundingBox());
    camera.addObstacle(couch.getBoundingBox());
    camera.addObstacle(podium.getBoundingBox());
    camera.addObstacle(carModel.getBoundingBox(centerCarTransform.getModelMatrix()));
    camera.addObstacle(car1Model.getBoundingBox(rightCarTransform.getModelMatrix()));
    camera.addObstacle(car2Model.getBoundingBox(leftCarTransform.getModelMatrix()));
    camera.addObstacle(myCar.getBoundingBox());
    int carIndex = camera.obstacles.size() - 1;

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

        EnterExitCarInput(window, camera, myCar);


        // --- C. Camera & Physics Update ---
        //camera.obstacles[0] = myCar.getBoundingBox();
        // for is driving or not
        if (!myCar.isDriven)
        {
            camera.Inputs(window);
            camera.obstacles[carIndex] = myCar.getBoundingBox();
            
        }
        if (myCar.isDriven)
        {
            camera.obstacles[carIndex] = { glm::vec3(0), glm::vec3(0) };
            camera.Position = myCar.getDriverWorldPosition();
            camera.Orientation = myCar.getForward();
        }
        
        myCar.drive(window, deltaTime);
        float driveFOV = myCar.isDriven ? 75.0f : 45.0f;//FOV IS DRIVING OR NOT

        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // --- D. Clear Buffers ---
        glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //Window Resizing
        int currentWidth, currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight); // Get actual pixels

        // Prevent division by zero if window is minimized
        float aspect = (currentHeight > 0) ? (float)currentWidth / currentHeight : 1.0f;

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

        chair.update(deltaTime);
        chair.drawFull(view, projection);

        couch.update(deltaTime);
        couch.drawFull(view, projection);

        table.update(deltaTime);
        table.drawFull(view, projection);

        limo.update(deltaTime);
        limo.draw(view, projection);

        podium.draw(view, projection);

        frame.draw(view, projection);

        door.update(deltaTime, camera);
        door.draw(view, projection);

        myCar.update(deltaTime);
        myCar.draw(view, projection); // Drawing myCar last

        
        static float debugTimer = 0.0f;
        debugTimer += deltaTime;
        if (debugTimer >= 0.5f) {
            std::cout << "Cam Pos: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;
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
    leather.Delete();
    leather1.Delete();
    merc.Delete();
    CLS.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}