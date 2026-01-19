// In Showroom.h
#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "building_Wall.h"

using namespace glm;
using namespace std;

class buildingShowroom {
private:
    vector<buildingWall> walls;
    vector<buildingWall> floors;
    vector<buildingWall> ceilings;

    float doorWidth;
    float doorHeight;
    string roomDirection;
    float roomRotationAngle;
    vec3 position;
    bool inverted;  // Add this

    void createWallWithDoor(float wallWidth, float wallHeight,
        float doorWidth, float doorHeight,
        const char* wallTexture,
        vec3 position, vec3 rotation,
        bool inverted = false);  // Add inverted parameter

public:
    buildingShowroom();
    ~buildingShowroom();
    buildingShowroom(const buildingShowroom& other);
    buildingShowroom(buildingShowroom&& other) noexcept;
    buildingShowroom& operator=(const buildingShowroom& other);
    buildingShowroom& operator=(buildingShowroom&& other) noexcept;

    void createBasicRoom(float width, float height, float depth,
        const string& wallTexture,
        const string& floorTexture,
        const string& ceilingTexture,
        const string& insideTexture,
        float doorWidth,
        float doorHeight,
        const string& roomDirection,
        bool isInverted = false);  // Add isInverted parameter with default false

    void drawAt(Shader& shader, mat4 view, mat4 projection, vec3 worldPosition);
    void drawTransformed(Shader& shader, mat4 roomTransform, mat4 view, mat4 projection);
    void setPosition(vec3 worldPosition);
    void draw(Shader& shader, mat4 view, mat4 projection);
    void setWallTextureScale(float scaleX, float scaleY);  // Add this
    void setFloorTextureScale(float scaleX, float scaleY); // Optional
    void setCeilingTextureScale(float scaleX, float scaleY); // Optional
    void clear();

    vec3 getPosition() const;
    float getDoorWidth() const;
    float getDoorHeight() const;
    string getRoomDirection() const;
    float getRoomRotationAngle() const;
    bool isCreated() const;
    vec3 getDimensions() const;
    void updateDoorSize(float newWidth, float newHeight);
    void updateRoomDirection(const string& newDirection);

    // New method
    bool isInverted() const;
};