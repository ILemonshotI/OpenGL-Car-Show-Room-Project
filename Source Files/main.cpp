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
#include "Skybox.h"
#include "Ground.h"
#include "Showroom.h"



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
    Camera camera(width, height, glm::vec3(15.0f, 1.0f, -42.0f));
    camera.Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
    camera.setWorldBounds(Camera::BoundingBox(
        glm::vec3(-50.0f, 0.0f, -50.0f),
        glm::vec3(50.0f, 10.0f, 50.0f)
    ));
    camera.cameraSize = glm::vec3(0.5f, 0.5f, 0.5f);

    // ---------------------------------------------------------
    // 5. SHADER SETUP
    // ---------------------------------------------------------
    Shader modelShader("Resource Files/shaders/model_shader.vert", "Resource Files/shaders/model_shader.frag");
    Shader carShader("Resource Files/shaders/car_shader.vert", "Resource Files/shaders/car_shader.frag");
    Shader skyboxShader("Resource Files/shaders/skybox.vert", "Resource Files/shaders/skybox.frag");
    Shader groundShader("Resource Files/shaders/ground.vert", "Resource Files/shaders/ground.frag");
    Shader wallShader("Resource Files/shaders/wall.vert", "Resource Files/shaders/wall.frag");

    // ---------------------------------------------------------
    // 6. LIGHTING CONFIGURATION (Building & World)
    // ---------------------------------------------------------
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Main car light
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

    glm::vec4 bLightColor = glm::vec4(1.0f, 0.8f, 0.8f, 1.0f); // Building light
    glm::vec3 bLightPos = glm::vec3(0.0f, 6.0f, 0.0f);

    wallShader.Activate();
    glUniform3f(glGetUniformLocation(wallShader.ID, "ambientColor"), 0.1f, 0.1f, 0.1f);
    glUniform3f(glGetUniformLocation(wallShader.ID, "diffuseColor"), 0.3f, 0.3f, 0.3f);
    glUniform3f(glGetUniformLocation(wallShader.ID, "specularColor"), 0.5f, 0.5f, 0.5f);
    glUniform1f(glGetUniformLocation(wallShader.ID, "shininess"), 2.0f);
    glUniform1i(glGetUniformLocation(wallShader.ID, "numLights"), 6);

    // Utility to set up the 6 building lights from second main
    auto setBLight = [&](int i, glm::vec3 pos, glm::vec3 col, float intensity) {
        string b = "lights[" + to_string(i) + "].";
        glUniform1i(glGetUniformLocation(wallShader.ID, (b + "enabled").c_str()), 1);
        glUniform3f(glGetUniformLocation(wallShader.ID, (b + "position").c_str()), pos.x, pos.y, pos.z);
        glUniform3f(glGetUniformLocation(wallShader.ID, (b + "color").c_str()), col.x, col.y, col.z);
        glUniform1f(glGetUniformLocation(wallShader.ID, (b + "intensity").c_str()), intensity);
        glUniform1f(glGetUniformLocation(wallShader.ID, (b + "constant").c_str()), 0.1f);
        glUniform1f(glGetUniformLocation(wallShader.ID, (b + "linear").c_str()), 0.01f);
        glUniform1f(glGetUniformLocation(wallShader.ID, (b + "quadratic").c_str()), 0.04f);
        glUniform1i(glGetUniformLocation(wallShader.ID, (b + "type").c_str()), 0);
        };

    setBLight(0, bLightPos, glm::vec3(bLightColor), 0.3f);
    setBLight(1, glm::vec3(140.0f, 5.0f, 140.0f), glm::vec3(0.2f, 0.2f, 1.0f), 1.0f);
    setBLight(2, glm::vec3(15.0f, 3.0f, -23.0f), glm::vec3(bLightColor), 0.3f);
    setBLight(3, glm::vec3(11.0f, 3.0f, -27.0f), glm::vec3(bLightColor), 0.3f);
    setBLight(4, glm::vec3(19.0f, 3.0f, -19.0f), glm::vec3(bLightColor), 0.3f);
    setBLight(5, glm::vec3(11.0f, 3.0f, -19.0f), glm::vec3(bLightColor), 0.3f);
    

    // ---------------------------------------------------------
    // 7. TEXTURE SETUP
    // ---------------------------------------------------------
    Texture messi("Resource Files/textures/white.jpg");
    Texture red_paint("Resource Files/textures/red_paint.jpg");
    Texture limoText("Resource Files/textures/glossy_black.jpg");
    Texture wheelTex("Resource Files/textures/wheel2.png");
    Texture leather("Resource Files/textures/79_leather texture-seamless.jpg");
    Texture leather1("Resource Files/textures/Leather.jpg");
    Texture merc("Resource Files/textures/mercedes-logo-mercedes-benz-logos-vector-eps-cdr-svg-download-15.png");
    Texture CLS("Resource Files/textures/CLS.png");
    Texture wood("Resource Files/textures/wood.jpg");
    Texture blue_paint("Resource Files/textures/blue_paint.jpg");

    // ---------------------------------------------------------
    // 8. BUILDING DRAWING SECTION (Setup)
    // ---------------------------------------------------------
    vector<string> faces = {
        "Resource Files/textures/skybox/px.png", "Resource Files/textures/skybox/nx.png",
        "Resource Files/textures/skybox/py.png", "Resource Files/textures/skybox/ny.png",
        "Resource Files/textures/skybox/pz.png", "Resource Files/textures/skybox/nz.png"
    };

    buildingSkybox skybox(faces);
    buildingGround ground(500.0f, "Resource Files/textures/ground/ground.jpg");

    // Showroom: Y set to 0.1f to sit on the floor
    buildingShowroom showroom;
    showroom.createBasicRoom(16.0f, 3.54f, 16.0f, "Resource Files/textures/rooms/wallpaper.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/black matte.jpg", 3.0f, 4.0f, "north");
    showroom.setPosition(vec3(15.0f, 0.1f, -23.0f));


    // Road: Restored to 200.0 width
    buildingWall road = buildingWall(200.0f, 1.5f, "Resource Files/textures/ground/street.jpg", vec3(15.0f, 0.05f, -10.5f), vec3(-90.0f, 0.0f, 0.0f), false);
    road.setTextureScale(1.5f, 0.75f);

    // Grass: Restored to 19.0 width/depth
    buildingWall grass = buildingWall(19.0f, 19.0f, "Resource Files/textures/ground/grass3.jpg", vec3(15.0f, 0.05f, -23.0f), vec3(-90.0f, 0.0f, 0.0f), false);
    grass.setTextureScale(2.0f, 2.0f);

    buildingShowroom room1, room2, room3, room4, buildingObj, building2, building3, building4;

    // Internal Rooms: Y adjusted to 0.0f relative to the showroom's 0.1f base
    room1.createBasicRoom(6.0f, 3.54f, 6.0f, "Resource Files/textures/rooms/wallpaper.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/wallpaper.jpg", 2.0f, 4.0f, "east", true);
    room1.setPosition(vec3(20.1f, -0.1f, -17.9f));

    room2.createBasicRoom(6.0f, 3.54f, 6.0f, "Resource Files/textures/rooms/wallpaper.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/wallpaper.jpg", 2.0f, 4.0f, "west", true);
    room2.setPosition(vec3(10.1f, -0.1f, -17.9f));

    room3.createBasicRoom(6.0f, 3.54f, 6.0f, "Resource Files/textures/rooms/wallpaper.jpg", "Resource Files/textures/rooms/floor2jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/wallpaper.jpg", 2.0f, 4.0f, "west", true);
    room3.setPosition(vec3(9.9f, -0.1f, -28.1f));

    room4.createBasicRoom(6.0f, 3.54f, 6.0f, "Resource Files/textures/rooms/wallpaper.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/wallpaper.jpg", 2.0f, 4.0f, "east", true);
    room4.setPosition(vec3(20.1f, -0.1f, -28.1f));

    // External Buildings (Keeping original positions as you requested moving away from them)
    buildingObj.createBasicRoom(30.0f, 36.0f, 30.0f, "Resource Files/textures/rooms/building2.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/building2.jpg", 0.0f, 0.0f, "east", true);
    buildingObj.setPosition(vec3(30.0f, 0.0f, 30.0f));
    buildingObj.setWallTextureScale(10.0f, 10.0f);

    building2.createBasicRoom(30.0f, 36.0f, 30.0f, "Resource Files/textures/rooms/building4.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/building4.jpg", 0.0f, 0.0f, "east", true);
    building2.setPosition(vec3(-4.0f, 0.0f, 30.0f));
    building2.setWallTextureScale(10.0f, 10.0f);

    building3.createBasicRoom(30.0f, 36.0f, 30.0f, "Resource Files/textures/rooms/bulding3.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/bulding3.jpg", 0.0f, 0.0f, "east", true);
    building3.setPosition(vec3(-38.0f, 0.0f, 30.0f));
    building3.setWallTextureScale(10.0f, 10.0f);

    building4.createBasicRoom(30.0f, 36.0f, 30.0f, "Resource Files/textures/rooms/bulding3.jpg", "Resource Files/textures/rooms/floor2.jpg", "Resource Files/textures/rooms/roof.jpg", "Resource Files/textures/rooms/bulding3.jpg", 0.0f, 0.0f, "east", true);
    building4.setPosition(vec3(-38.0f, 0.0f, -10.0f));
    building4.setWallTextureScale(10.0f, 10.0f);



    
    // ---------------------------------------------------------
    // 9. 3D MODEL & CUSTOM OBJECT LOADING
    // ---------------------------------------------------------
    modelModel carModel("Resource Files/3d_models/lamborghini_centenario/scene.gltf");
    modelModel car1Model("Resource Files/3d_models/porsche_911/scene.gltf");
    modelModel car2Model("Resource Files/3d_models/mclaren_sienna/scene.gltf");
    modelModel car3Model("Resource Files/3d_models/audi_rs5/scene.gltf");
    modelModel fountain("Resource Files/3d_models/fountain/scene.gltf");
    modelModel tableModel("Resource Files/3d_models/office_table/scene.gltf");
    modelModel officeChairModel("Resource Files/3d_models/modern_office_chair/scene.gltf");
    modelModel computer("Resource Files/3d_models/desktop_computer/scene.gltf");
    modelModel modernChair1("Resource Files/3d_models/futuristic_chair/scene.gltf");
    modelModel modernChair2("Resource Files/3d_models/futuristic_chair/scene.gltf");
    modelModel potJade("Resource Files/3d_models/pot_jade_plant/scene.gltf");
    modelModel trashcan("Resource Files/3d_models/trashcan/scene.gltf");
    modelModel lightlamp("Resource Files/3d_models/butterfly_lamp/scene.gltf");

   


    Car myCar(&carShader, &red_paint, &wheelTex, &leather, &leather1, &merc, glm::vec3(1.4, 0.4, -10.0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    myCar.useTexture = true;
    myCar.setRotation(180.0f);
    Car myCarForShow(&carShader, &blue_paint, &wheelTex, &leather, &leather1, &merc, glm::vec3(9.3, 0.6, -18.0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    myCarForShow.useTexture = true;
    myCarForShow.setRotation(180.0f);
    Car limo(&carShader, &limoText, &wheelTex, &leather, &leather1, &merc, glm::vec3(9.0, 0.55, -28.0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    limo.scaleBy(glm::vec3(1.7, 0.8, 1));
    limo.useTexture = true;
    limo.setRotation(180.0f);

    Chair chair(&carShader, &leather1, glm::vec3(21.5, 0.7, -23.0f), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    chair.setRotation(0.0f);
    chair.setScale(glm::vec3(1.6f, 1.6f, 1.6f));
    chair.useTexture = true;
    Couch couch(&carShader, &leather, glm::vec3(19.5, 0.3, -21.4), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    couch.useTexture = true;
    couch.setRotation(180.0f);
    couch.setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    Couch couch2(&carShader, &leather, glm::vec3(17.7, 0.3, -24.6), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    couch2.useTexture = true;
    couch2.setScale(glm::vec3(2.0f, 2.0f, 2.0f));
    Table table(&carShader, &wood, glm::vec3(20.0, 0.9, -23.0f), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    table.useTexture = true;
    table.setRotation(0.0f);
    table.setScale(glm::vec3(2.0f, 2.0f, 2.0f));

    // --- Main Entrance Door ---
 // Width 3.0, Height 3.54 to match the showroom
    SlidingDoor mainDoor(&carShader, &messi, glm::vec3(14.25f, 0.1f, -31.0f), 3.0f, 3.54f, 0.2f);

    Frame frame(&carShader, &CLS, glm::vec3(14.3, 2.0, -15.3), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    frame.useTexture = 1;
    frame.setScale(glm::vec3(4.0f, 4.0f, 4.0f));
    Podium podium1(1.3f, 1.5f, 0.4, 36, &carShader, &messi, glm::vec3(21, 0, -29.7), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    podium1.useTexture = true;
    Podium podium2(1.3f, 1.5f, 0.4, 36, &carShader, &messi, glm::vec3(21, 0, -26.5), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    podium2.useTexture = true;
    Podium podium3(1.3f, 1.5f, 0.4, 36, &carShader, &messi, glm::vec3(21, 0, -19.3), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    podium3.useTexture = true;
    Podium podium4(1.3f, 1.5f, 0.4, 36, &carShader, &messi, glm::vec3(21, 0, -16.5), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    podium4.useTexture = true;
    Podium podium5(1.3f, 1.5f, 0.4, 36, &carShader, &messi, glm::vec3(9.3, 0, -18.0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    podium5.useTexture = true;
    Podium podium6(1.3f, 1.5f, 0.4, 36, &carShader, &messi, glm::vec3(9.3, 0, -28.0), glm::vec3(0, 1, 0), 0.0f, 1.0f);
    podium6.useTexture = true;


    // ---------------------------------------------------------
    // 10. TRANSFORMS AND BASE SCALING
    // ---------------------------------------------------------
    Transform car1Transform(glm::vec3(21, 0.2, -19.3), glm::vec3(0, -55, 0), glm::vec3(0.65f));
    Transform car4Transform(glm::vec3(21, 0.2, -16.5), glm::vec3(0, -125.0f, 0), glm::vec3(0.65f));
    Transform car2Transform(glm::vec3(21, 0.2, -29.7), glm::vec3(0, -45.0f, 0), glm::vec3(0.65f));
    Transform car3Transform(glm::vec3(21, 0.2, -26.5), glm::vec3(0, -135.0f, 0), glm::vec3(0.65f));
    Transform fountainTransform(glm::vec3(15.0, 0.0, -36.5f), glm::vec3(0, 0, 0), glm::vec3(0.003f));
    Transform chairTransform(glm::vec3(8.5, 0.8, -23.0f), glm::vec3(0, 90, 0), glm::vec3(1.1f));
    Transform tableTransform(glm::vec3(9.3, 0.6, -23.0f), glm::vec3(0, -90, 0), glm::vec3(1.0f));
    Transform computerTransform(glm::vec3(9.3, 1.36f, -23.0f), glm::vec3(0, -90, 0), glm::vec3(1.0f));
    Transform mchair1Transform(glm::vec3(11.1, 0.1, -25.4f), glm::vec3(0, -90, 0), glm::vec3(1.2f));
    Transform mchair2Transform(glm::vec3(11.5, 0.1, -20.6f), glm::vec3(0, 90, 0), glm::vec3(1.2f));
    Transform plantTransform1(glm::vec3(12.0, 0.1, -31.8f), glm::vec3(0, 0, 0), glm::vec3(4.0f));
    Transform plantTransform2(glm::vec3(18.0, 0.1, -31.8f), glm::vec3(0, 0, 0), glm::vec3(4.0f));

    Transform trash1Transform(glm::vec3(13.4, 0.1, -16.6), glm::vec3(0, 90, 0), glm::vec3(0.1f));
    Transform trash2Transform(glm::vec3(16.8, 0.1, -16.6), glm::vec3(0, 90, 0), glm::vec3(0.1f));

    Transform lamp1Transform(glm::vec3(15.0f, 3.0f, -23.0f) ,glm::vec3(0, 0, 0), glm::vec3(2.0f));
   // Transform lamp2Transform(glm::vec3(19.0f, 3.0f, -27.0f), glm::vec3(0, 0, 0), glm::vec3(3.0f));
    //Transform lamp3Transform(glm::vec3(11.0f, 3.0f, -27.0f), glm::vec3(0, 0, 0), glm::vec3(3.0f));
    //Transform lamp4Transform(glm::vec3(19.0f, 3.0f, -19.0f), glm::vec3(0, 0, 0), glm::vec3(3.0f));
    //Transform lamp5Transform(glm::vec3(11.0f, 3.0f, -19.0f), glm::vec3(0, 0, 0), glm::vec3(3.0f));

    myCar.baseSize = glm::vec3(2.0f, 1.0f, 1.5f);
    chair.baseSize = glm::vec3(0.44f, 1.1f, 0.4f);
    couch.baseSize = glm::vec3(1.2f, 0.9f, 0.4f);
    table.baseSize = glm::vec3(0.44f, 0.45f, 0.4f);
    podium1.baseSize = podium1.getCalculatedBaseSize();
    podium2.baseSize = podium1.getCalculatedBaseSize();
    podium3.baseSize = podium1.getCalculatedBaseSize();
    podium4.baseSize = podium1.getCalculatedBaseSize();
    podium5.baseSize = podium1.getCalculatedBaseSize();
    podium6.baseSize = podium1.getCalculatedBaseSize();

    // ---------------------------------------------------------
    // 11. COLLISION & OBSTACLE REGISTRATION
    // ---------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float lastTime = glfwGetTime();



    // --- EXTERNAL BUILDING COLLISIONS ---

// buildingObj at (30.0, 0.0, 30.0)
    camera.addObstacle({ vec3(15.0f, 0.0f, 15.0f), vec3(45.0f, 36.0f, 45.0f) });

    // building2 at (-4.0, 0.0, 30.0)
    camera.addObstacle({ vec3(-19.0f, 0.0f, 15.0f), vec3(11.0f, 36.0f, 45.0f) });

    // building3 at (-38.0, 0.0, 30.0)
    camera.addObstacle({ vec3(-53.0f, 0.0f, 15.0f), vec3(-23.0f, 36.0f, 45.0f) });

    // building4 at (-38.0, 0.0, -10.0)
    camera.addObstacle({ vec3(-53.0f, 0.0f, -25.0f), vec3(-23.0f, 10.0f, 5.0f) });
    // Showroom Center: 15, -23 | Size: 16x16
// North Wall (The one with the door - split into two)
// Left side of door
    camera.addObstacle({ vec3(7.0f, 0.0f, -31.1f), vec3(13.5f, 4.0f, -30.9f) });
    // Right side of door
    camera.addObstacle({ vec3(16.5f, 0.0f, -31.1f), vec3(23.0f, 4.0f, -30.9f) });

    // South Wall (Solid)
    camera.addObstacle({ vec3(7.0f, 0.0f, -15.1f), vec3(23.0f, 4.0f, -14.9f) });

    // West Wall (Solid)
    camera.addObstacle({ vec3(6.9f, 0.0f, -31.0f), vec3(7.1f, 4.0f, -15.0f) });

    // East Wall (Solid)
    camera.addObstacle({ vec3(22.9f, 0.0f, -31.0f), vec3(23.1f, 4.0f, -15.0f) });

    // --- ROOM 1 (Door on WEST wall) ---
    camera.addObstacle({ vec3(17.1f, 0.0f, -20.95f), vec3(23.1f, 3.5f, -20.85f) }); // North
    camera.addObstacle({ vec3(17.1f, 0.0f, -14.95f), vec3(23.1f, 3.5f, -15.05f) }); // South
    camera.addObstacle({ vec3(23.05f, 0.0f, -21.0f),  vec3(23.15f, 3.5f, -14.8f) }); // East (Solid)
    camera.addObstacle({ vec3(17.05f, 0.0f, -21.0f),  vec3(17.15f, 3.5f, -18.9f) }); // West (Part 1)
    camera.addObstacle({ vec3(17.05f, 0.0f, -16.9f),  vec3(17.15f, 3.5f, -14.8f) }); // West (Part 2)

    // --- ROOM 2 (Door on EAST wall) ---
    camera.addObstacle({ vec3(7.1f, 0.0f, -20.95f),  vec3(13.1f, 3.5f, -20.85f) }); // North
    camera.addObstacle({ vec3(7.1f, 0.0f, -14.95f),  vec3(13.1f, 3.5f, -15.05f) }); // South
    camera.addObstacle({ vec3(6.95f, 0.0f, -21.0f),   vec3(7.05f, 3.5f, -14.8f) });  // West (Solid)
    camera.addObstacle({ vec3(13.05f, 0.0f, -21.0f),  vec3(13.15f, 3.5f, -18.9f) }); // East (Part 1)
    camera.addObstacle({ vec3(13.05f, 0.0f, -16.9f),  vec3(13.15f, 3.5f, -14.8f) }); // East (Part 2)

    // --- ROOM 3 (Door on EAST wall) ---
    camera.addObstacle({ vec3(6.9f, 0.0f, -31.15f),  vec3(12.9f, 3.5f, -31.05f) }); // North
    camera.addObstacle({ vec3(6.9f, 0.0f, -25.15f),  vec3(12.9f, 3.5f, -25.05f) }); // South
    camera.addObstacle({ vec3(6.85f, 0.0f, -31.1f),   vec3(6.95f, 3.5f, -25.1f) });  // West (Solid)
    camera.addObstacle({ vec3(12.85f, 0.0f, -31.1f),  vec3(12.95f, 3.5f, -29.1f) }); // East (Part 1)
    camera.addObstacle({ vec3(12.85f, 0.0f, -27.1f),  vec3(12.95f, 3.5f, -25.1f) }); // East (Part 2)

    // --- ROOM 4 (Door on WEST wall) ---
    camera.addObstacle({ vec3(17.1f, 0.0f, -31.15f), vec3(23.1f, 3.5f, -31.05f) }); // North
    camera.addObstacle({ vec3(17.1f, 0.0f, -25.15f), vec3(23.1f, 3.5f, -25.05f) }); // South
    camera.addObstacle({ vec3(23.05f, 0.0f, -31.1f),  vec3(23.15f, 3.5f, -25.1f) });  // East (Solid)
    camera.addObstacle({ vec3(17.05f, 0.0f, -31.1f),  vec3(17.15f, 3.5f, -29.1f) }); // West (Part 1)
    camera.addObstacle({ vec3(17.05f, 0.0f, -27.1f),  vec3(17.15f, 3.5f, -25.1f) }); // West (Part 2)

    camera.addObstacle(chair.getBoundingBox());
    camera.addObstacle(table.getBoundingBox());
    camera.addObstacle(couch.getBoundingBox());
    camera.addObstacle(couch2.getBoundingBox());
    camera.addObstacle(podium1.getBoundingBox());
    camera.addObstacle(podium2.getBoundingBox());
    camera.addObstacle(podium3.getBoundingBox());
    camera.addObstacle(podium4.getBoundingBox());
    camera.addObstacle(podium5.getBoundingBox());
    camera.addObstacle(podium6.getBoundingBox());
    camera.addObstacle(carModel.getBoundingBox(car1Transform.getModelMatrix()));
    camera.addObstacle(car1Model.getBoundingBox(car2Transform.getModelMatrix()));
    camera.addObstacle(car2Model.getBoundingBox(car3Transform.getModelMatrix()));
    camera.addObstacle(car3Model.getBoundingBox(car4Transform.getModelMatrix()));
    camera.addObstacle(fountain.getBoundingBox(fountainTransform.getModelMatrix()));
    camera.addObstacle(tableModel.getBoundingBox(tableTransform.getModelMatrix()));
    camera.addObstacle(modernChair1.getBoundingBox(mchair1Transform.getModelMatrix()));
    camera.addObstacle(modernChair2.getBoundingBox(mchair2Transform.getModelMatrix()));
    camera.addObstacle(trashcan.getBoundingBox(trash1Transform.getModelMatrix()));
    camera.addObstacle(trashcan.getBoundingBox(trash2Transform.getModelMatrix()));
    camera.addObstacle(officeChairModel.getBoundingBox(chairTransform.getModelMatrix()));
    camera.addObstacle(potJade.getBoundingBox(plantTransform1.getModelMatrix()));
    camera.addObstacle(potJade.getBoundingBox(plantTransform2.getModelMatrix()));
    camera.addObstacle(myCarForShow.getBoundingBox());
    camera.addObstacle(limo.getBoundingBox());
    camera.addObstacle(myCar.getBoundingBox());
    int carIndex = camera.obstacles.size() - 1;

    // Showroom walls
   /* camera.addObstacle({ glm::vec3(-14.5f,0,-14.5f), glm::vec3(-13.5f,5,14.5f) });
    camera.addObstacle({ glm::vec3(13.5f,0,-14.5f), glm::vec3(14.5f,5,14.5f) });
    camera.addObstacle({ glm::vec3(-14.5f,0,13.5f), glm::vec3(14.5f,5,14.5f) });
    camera.addObstacle({ glm::vec3(-14.5f,0,-14.5f), glm::vec3(-5.0f,5,-13.5f) });
    camera.addObstacle({ glm::vec3(5.0f,0,-14.5f), glm::vec3(14.5f,5,-13.5f) });*/

    // ---------------------------------------------------------
    // 12. MAIN RENDER LOOP
    // ---------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Input Handling
        CarDoorInput(window, camera, myCar);
        EnterExitCarInput(window, camera, myCar);

        if (!myCar.isDriven) {
            camera.Inputs(window);
            camera.obstacles[carIndex] = myCar.getBoundingBox();
        }
        else {
            camera.obstacles[carIndex] = { glm::vec3(0), glm::vec3(0) };
            camera.Position = myCar.getDriverWorldPosition();
            camera.Orientation = myCar.getForward();
        }

        myCar.drive(window, deltaTime);
        camera.updateMatrix(45.0f, 0.1f, 1000.0f); // Extended far plane for buildings

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 1000.0f);

        // --- BUILDING DRAWING SECTION (Render) ---
        ground.Draw(groundShader, view, projection);
        skybox.Draw(skyboxShader, view, projection);

        wallShader.Activate();
        glUniform3f(glGetUniformLocation(wallShader.ID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

        showroom.draw(wallShader, view, projection);
        room1.draw(wallShader, view, projection);
        room2.draw(wallShader, view, projection);
        room3.draw(wallShader, view, projection);
        room4.draw(wallShader, view, projection);
        buildingObj.draw(wallShader, view, projection);
        grass.Draw(wallShader, view, projection);
        road.DrawWithTransform(wallShader, mat4(1.0f), view, projection);
        building2.draw(wallShader, view, projection);
        building3.draw(wallShader, view, projection);
        building4.draw(wallShader, view, projection);

        // --- CAR SHOWROOM MODELS ---
        modelShader.Activate();
        glUniform4f(glGetUniformLocation(modelShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(modelShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glm::mat4 identity = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(identity));

        carModel.Draw(modelShader, camera, car1Transform.getModelMatrix());
        car1Model.Draw(modelShader, camera, car2Transform.getModelMatrix());
        car2Model.Draw(modelShader, camera, car3Transform.getModelMatrix());
        car3Model.Draw(modelShader, camera, car4Transform.getModelMatrix());
        fountain.Draw(modelShader, camera, fountainTransform.getModelMatrix());
        tableModel.Draw(modelShader, camera, tableTransform.getModelMatrix());
        officeChairModel.Draw(modelShader, camera, chairTransform.getModelMatrix());
        computer.Draw(modelShader, camera, computerTransform.getModelMatrix());
        modernChair1.Draw(modelShader, camera, mchair1Transform.getModelMatrix());
        modernChair2.Draw(modelShader, camera, mchair2Transform.getModelMatrix());
        potJade.Draw(modelShader, camera, plantTransform1.getModelMatrix());
        potJade.Draw(modelShader, camera, plantTransform2.getModelMatrix());
        trashcan.Draw(modelShader, camera, trash1Transform.getModelMatrix());
        trashcan.Draw(modelShader, camera, trash2Transform.getModelMatrix());
        lightlamp.Draw(modelShader, camera, lamp1Transform.getModelMatrix());
        /*lightlamp.Draw(modelShader, camera, lamp2Transform.getModelMatrix());
        lightlamp.Draw(modelShader, camera, lamp3Transform.getModelMatrix());
        lightlamp.Draw(modelShader, camera, lamp4Transform.getModelMatrix());
        lightlamp.Draw(modelShader, camera, lamp5Transform.getModelMatrix());*/
        

        // --- CUSTOM OBJECTS ---
        carShader.Activate();
        glUniform4f(glGetUniformLocation(carShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(carShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        chair.update(deltaTime); chair.drawFull(view, projection);
        couch.update(deltaTime); couch.drawFull(view, projection);
        couch2.update(deltaTime); couch2.drawFull(view, projection);
        table.update(deltaTime); table.drawFull(view, projection);
        limo.update(deltaTime); limo.draw(view, projection);
        podium1.draw(view, projection);
        podium2.draw(view, projection);
        podium3.draw(view, projection);
        podium4.draw(view, projection);
        podium5.draw(view, projection);
        podium6.draw(view, projection);
        frame.draw(view, projection);
        mainDoor.update(deltaTime, camera); mainDoor.draw(view, projection);
        
        myCarForShow.update(deltaTime); myCarForShow.draw(view, projection);
        limo.update(deltaTime); limo.draw(view, projection);
        myCar.update(deltaTime); myCar.draw(view, projection);

        // Debugger
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
    // 13. CLEANUP
    // ---------------------------------------------------------
    skybox.Delete();
    skyboxShader.Delete();
    groundShader.Delete();
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