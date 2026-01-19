#include "Showroom.h"
#include <iostream>

float directionToAngle(const string& direction) {
    if (direction == "north" || direction == "n") return 0.0f;
    if (direction == "east" || direction == "e") return 90.0f;
    if (direction == "south" || direction == "s") return 180.0f;
    if (direction == "west" || direction == "w") return 270.0f;
    return 0.0f;
}

void buildingShowroom::createBasicRoom(float width, float height, float depth,
    const string& wallTexture,
    const string& floorTexture,
    const string& ceilingTexture,
    const string& insideTexture,
    float doorWidth,
    float doorHeight,
    const string& roomDirection,
    bool isInverted)
{

    clear();

    const char* wallTex = wallTexture.empty() ? nullptr : wallTexture.c_str();
    const char* floorTex = floorTexture.empty() ? nullptr : floorTexture.c_str();
    const char* ceilingTex = ceilingTexture.empty() ? nullptr : ceilingTexture.c_str();
    const char* insideTex = insideTexture.empty() ? nullptr : insideTexture.c_str();

    this->doorWidth = doorWidth;
    this->doorHeight = doorHeight;
    this->roomDirection = roomDirection;
    this->roomRotationAngle = directionToAngle(roomDirection);
    this->inverted = isInverted;

    // Floor: normals should point UP (0,1,0) for inside the room
    // If inverted=true, floor normals should point DOWN (0,-1,0) for outside
    floors.push_back(buildingWall(width, depth, floorTex,
        vec3(0.0f, 0.0f, 0.0f),
        vec3(-90.0f, 0.0f, 0.0f),
        isInverted ? true : false));  // Floor: inverted when room is inverted

    // Ceiling: normals should point DOWN (0,-1,0) for inside the room
    // If inverted=true, ceiling normals should point UP (0,1,0) for outside
    ceilings.push_back(buildingWall(width, depth, ceilingTex,
        vec3(0.0f, height, 0.0f),
        vec3(90.0f, 0.0f, 0.0f),
        isInverted ? true : false));  // Ceiling: inverted when room is inverted

    ceilings.push_back(buildingWall(width, depth, ceilingTex,
        vec3(0.0f, height + 0.01f, 0.0f),
        vec3(90.0f, 0.0f, 0.0f),
        isInverted ? false : true));

    // Front wall (with door): normals should point INTO the room
    // If the room is inverted (facing outside), walls should also be inverted
    createWallWithDoor(width, height, doorWidth, doorHeight, wallTex,
        vec3(0.0f, height / 2, -depth / 2),
        vec3(0.0f, 0.0f, 0.0f),
        isInverted ? true : false);

    createWallWithDoor(width, height, doorWidth, doorHeight, insideTex,
        vec3(0.0f, (height / 2) + 0.01f, (-depth / 2) - 0.01f),
        vec3(0.0f, 0.0f, 0.0f),
        isInverted ? false : true);

    // Back wall: needs to point INTO the room
    walls.push_back(buildingWall(width, height, wallTex,
        vec3(0.0f, height / 2, depth / 2),
        vec3(0.0f, 180.0f, 0.0f),
        isInverted ? true : false));

    walls.push_back(buildingWall(width, height, insideTex,
        vec3(0.0f, (height / 2) + 0.01f, (depth / 2) + 0.01f),
        vec3(0.0f, 180.0f, 0.0f),
        isInverted ? false : true));


    // Left wall: needs to point INTO the room (toward +X)
    walls.push_back(buildingWall(depth, height, wallTex,
        vec3(-width / 2, height / 2, 0.0f),
        vec3(0.0f, 90.0f, 0.0f),
        isInverted ? true : false));
    walls.push_back(buildingWall(depth, height, insideTex,
        vec3((-width / 2) - 0.01f, (height / 2) + 0.01f, 0.0f),
        vec3(0.0f, 90.0f, 0.0f),
        isInverted ? false : true));

    // Right wall: needs to point INTO the room (toward -X)
    walls.push_back(buildingWall(depth, height, wallTex,
        vec3(width / 2, height / 2, 0.0f),
        vec3(0.0f, -90.0f, 0.0f),
        isInverted ? true : false));
    walls.push_back(buildingWall(depth, height, insideTex,
        vec3((width / 2) + 0.01, (height / 2) + 0.01, 0.0f),
        vec3(0.0f, -90.0f, 0.0f),
        isInverted ? false : true));

}

void buildingShowroom::createWallWithDoor(float wallWidth, float wallHeight,
    float doorWidth, float doorHeight,
    const char* wallTexture,
    vec3 position, vec3 rotation,
    bool inverted) {

    float actualDoorWidth = std::min(doorWidth, wallWidth * 0.8f);
    float actualDoorHeight = std::min(doorHeight, wallHeight * 0.9f);

    float totalGap = actualDoorWidth;
    float segmentWidth = (wallWidth - totalGap) / 2.0f;

    if (segmentWidth > 0.05f) {
        vec3 leftPos = position;
        leftPos.x = position.x - (wallWidth / 2 - segmentWidth / 2);
        walls.push_back(buildingWall(segmentWidth, wallHeight, wallTexture, leftPos, rotation, inverted));

        vec3 rightPos = position;
        rightPos.x = position.x + (wallWidth / 2 - segmentWidth / 2);
        walls.push_back(buildingWall(segmentWidth, wallHeight, wallTexture, rightPos, rotation, inverted));
    }
    else {
        walls.push_back(buildingWall(wallWidth, wallHeight, wallTexture, position, rotation, inverted));
    }

    float topHeight = wallHeight - actualDoorHeight;
    if (topHeight > 0.05f) {
        vec3 topPos = position;
        topPos.y = position.y + (wallHeight / 2 - topHeight / 2);
        walls.push_back(buildingWall(actualDoorWidth, topHeight, wallTexture, topPos, rotation, inverted));
    }
}

void buildingShowroom::drawAt(Shader& shader, mat4 view, mat4 projection, vec3 worldPosition) {

    mat4 roomTransform = mat4(1.0f);
    roomTransform = translate(roomTransform, worldPosition);
    roomTransform = rotate(roomTransform, radians(roomRotationAngle), vec3(0.0f, 1.0f, 0.0f));

    drawTransformed(shader, roomTransform, view, projection);
}

void buildingShowroom::drawTransformed(Shader& shader, mat4 roomTransform, mat4 view, mat4 projection) {

    for (auto& wall : walls) {
        wall.DrawWithTransform(shader, roomTransform, view, projection);
    }

    for (auto& floor : floors) {
        floor.DrawWithTransform(shader, roomTransform, view, projection);
    }

    for (auto& ceiling : ceilings) {
        ceiling.DrawWithTransform(shader, roomTransform, view, projection);
    }
}

void buildingShowroom::setPosition(vec3 worldPosition) {
    position = worldPosition;
}

void buildingShowroom::draw(Shader& shader, mat4 view, mat4 projection) {
    drawAt(shader, view, projection, position);
}

void buildingShowroom::clear() {
    for (auto& w : walls) {
        w.Delete();
    }
    walls.clear();

    for (auto& f : floors) {
        f.Delete();
    }
    floors.clear();

    for (auto& c : ceilings) {
        c.Delete();
    }
    ceilings.clear();
}

buildingShowroom::buildingShowroom() : doorWidth(1.0f), doorHeight(2.0f), roomDirection("north"),
roomRotationAngle(0.0f), inverted(false) {
}

buildingShowroom::~buildingShowroom() {
    clear();
}

buildingShowroom::buildingShowroom(const buildingShowroom& other) {
    doorWidth = other.doorWidth;
    doorHeight = other.doorHeight;
    roomDirection = other.roomDirection;
    roomRotationAngle = other.roomRotationAngle;
    position = other.position;
    inverted = other.inverted;
}

buildingShowroom::buildingShowroom(buildingShowroom&& other) noexcept {
    doorWidth = other.doorWidth;
    doorHeight = other.doorHeight;
    roomDirection = move(other.roomDirection);
    roomRotationAngle = other.roomRotationAngle;
    position = other.position;
    inverted = other.inverted;

    walls = move(other.walls);
    floors = move(other.floors);
    ceilings = move(other.ceilings);

    other.walls.clear();
    other.floors.clear();
    other.ceilings.clear();
}

buildingShowroom& buildingShowroom::operator=(const buildingShowroom& other) {
    if (this != &other) {
        clear();
        doorWidth = other.doorWidth;
        doorHeight = other.doorHeight;
        roomDirection = other.roomDirection;
        roomRotationAngle = other.roomRotationAngle;
        position = other.position;
        inverted = other.inverted;
    }
    return *this;
}

buildingShowroom& buildingShowroom::operator=(buildingShowroom&& other) noexcept {
    if (this != &other) {
        clear();

        doorWidth = other.doorWidth;
        doorHeight = other.doorHeight;
        roomDirection = std::move(other.roomDirection);
        roomRotationAngle = other.roomRotationAngle;
        position = other.position;
        inverted = other.inverted;

        walls = std::move(other.walls);
        floors = std::move(other.floors);
        ceilings = std::move(other.ceilings);

        other.walls.clear();
        other.floors.clear();
        other.ceilings.clear();
    }
    return *this;
}

vec3 buildingShowroom::getPosition() const {
    return position;
}

float buildingShowroom::getDoorWidth() const {
    return doorWidth;
}

float buildingShowroom::getDoorHeight() const {
    return doorHeight;
}

string buildingShowroom::getRoomDirection() const {
    return roomDirection;
}

float buildingShowroom::getRoomRotationAngle() const {
    return roomRotationAngle;
}

bool buildingShowroom::isCreated() const {
    return !walls.empty();
}

vec3 buildingShowroom::getDimensions() const {
    return vec3(0.0f);
}

void buildingShowroom::updateDoorSize(float newWidth, float newHeight) {
    doorWidth = newWidth;
    doorHeight = newHeight;
}

void buildingShowroom::updateRoomDirection(const string& newDirection) {
    roomDirection = newDirection;
    roomRotationAngle = directionToAngle(newDirection);
}

bool buildingShowroom::isInverted() const {
    return inverted;
}
void buildingShowroom::setWallTextureScale(float scaleX, float scaleY) {
    for (auto& wall : walls) {
        wall.setTextureScale(scaleX, scaleY);
    }
}

void buildingShowroom::setFloorTextureScale(float scaleX, float scaleY) {
    for (auto& floor : floors) {
        floor.setTextureScale(scaleX, scaleY);
    }
}

void buildingShowroom::setCeilingTextureScale(float scaleX, float scaleY) {
    for (auto& ceiling : ceilings) {
        ceiling.setTextureScale(scaleX, scaleY);
    }
}