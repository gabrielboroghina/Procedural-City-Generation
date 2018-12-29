#include "Buildings.h"

#include "MeshBuilder.h"
#include <GL/glew.h>
#include "Core/Managers/ResourcePath.h"
#include "UIConstants.h"
#include "Streets.h"
#include "Objects/Parks.h"
#include "include/math.h"

constexpr int hTextures = 13;
constexpr int lTextures = 10;

Buildings::Buildings()
{
    using namespace UIConstants::Streets;
    LoadTextures();

    Streets *streets = Streets::GetInstance();
    Parks *parks = Parks::GetInstance();
    for (unsigned int i = 1; i < streets->horizStreets.size(); i++)
        for (unsigned int j = 1; j < streets->vertStreets.size(); j++)
            if (!parks->hasPark.count(j * 1000 + i))
                buildings.push_back(GenerateBuilding(streets->ZoneBetweenStreets(j - 1, j, i - 1, i)));
}

Buildings::~Buildings() {}

void Buildings::LoadTextures()
{
    // cylindrical building texture
    texture[0] = new Texture2D();
    texture[0]->Load2D((RESOURCE_PATH::TEXTURES + "building01.png").c_str(), GL_MIRRORED_REPEAT);

    // reduced number of faces texture
    for (int i = 1; i <= hTextures; i++) {
        texture[i] = new Texture2D();
        texture[i]->Load2D((RESOURCE_PATH::TEXTURES + "building_h" + std::to_string(i) + ".png").c_str(), GL_REPEAT);
    }

    for (int i = 1; i <= lTextures; i++) {
        texture[20 + i] = new Texture2D();
        texture[20 + i]->Load2D((RESOURCE_PATH::TEXTURES + "building_l" + std::to_string(i) + ".png").c_str(), GL_REPEAT);
    }

    // roof texture
    roofTexture = new Texture2D();
    roofTexture->Load2D((RESOURCE_PATH::TEXTURES + "roof.jpg").c_str(), GL_REPEAT);
}

/** Probabilistic chosen number of faces */
int ComputeNumberOfFaces(int type)
{
    if (type < 10) return 3;
    if (type < 60) return 4;
    if (type < 80) return 5;
    if (type < 90) return 6;
    return 30;
}

void Buildings::GenerateSpot(Building *building, int ang, float cylRad, float height)
{
    glm::vec3 spot_pos, light_dir;

    cylRad += UIConstants::Light::SPOT_DIST_ADDITION;
    spot_pos = glm::vec3(building->globalModelMat * glm::vec4(cylRad * cos(RADIANS(ang)), height + (rand() % 4) * 0.1,
                                                              cylRad * sin(RADIANS(ang)), 1));
    light_dir = glm::normalize(glm::vec3(-0.6 * cos(RADIANS(ang)), -1, -0.6 * sin(RADIANS(ang))));
    building->spots.push_back(make_pair(spot_pos, light_dir));
}

Building *Buildings::GenerateBuilding(std::tuple<float, float, float, float> limits)
{
    using namespace UIConstants::Buildings;

    float xmin, xmax, zmin, zmax;
    std::tie(xmin, xmax, zmin, zmax) = limits;

    float height = MIN_HEIGHT + (rand() % 10 + 1) * (MAX_HEIGHT - MIN_HEIGHT) / 10;
    int numFacesType = rand() % 100;

    auto buildingCyl = MeshBuilder::CreateCylinder(height, 1, ComputeNumberOfFaces(numFacesType), rand() % 2);
    Mesh *mesh = get<0>(buildingCyl);

    // pseudo-randomly choose texture function of the building's height
    Texture2D *tex = numFacesType >= 90 ? texture[0] : texture[(height >= 2) ? (rand() % hTextures + 1) : (rand() % lTextures + 21)];

    // place the building in the desired area
    float _x, x, _z, z;
    std::tie(_x, x, _z, z) = get<2>(buildingCyl);
    float scaleFactor = min((xmax - xmin) / (x - _x), (zmax - zmin) / (z - _z));
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(scaleFactor, 1, scaleFactor));
    glm::mat4 translateMat = glm::translate(glm::mat4(1), glm::vec3(xmin - _x * (xmax - xmin) / (x - _x), 0,
                                                                    zmin - _z * (zmax - zmin) / (z - _z)));
    glm::mat4 globalModelMat = translateMat * scaleMat;

    Building *building = new Building(globalModelMat);
    building->AddComp(mesh, tex, glm::mat4(1));
    building->AddComp(get<1>(buildingCyl), roofTexture, glm::mat4(1));

    // assembly other primitives to the building's base
    int levels = rand() % 3 + 1;
    float lastLevelScale = 1;
    float top = height;
    for (int i = 2; i <= levels; i++) {
        glm::mat4 transform;

        lastLevelScale *= 1 - (rand() % 5 + 1) / 20.0f;
        float heightScale = (rand() % 5 + 1) / 12.0f;

        transform = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, top, 0)), glm::vec3(lastLevelScale, heightScale, lastLevelScale));
        top += heightScale * height;

        building->AddComp(mesh, tex, transform);
        building->AddComp(get<1>(buildingCyl), roofTexture, transform);
    }

    // generate spotlights
    int ang = rand() % 120;
    GenerateSpot(building, ang, scaleFactor, top);
    GenerateSpot(building, ang + 120, scaleFactor, top);
    GenerateSpot(building, ang + 240, scaleFactor, top);

    return building;
}
