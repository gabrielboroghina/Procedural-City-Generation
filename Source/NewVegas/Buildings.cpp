#include "Buildings.h"

#include "MeshBuilder.h"
#include <GL/glew.h>
#include "Core/Managers/ResourcePath.h"
#include "UIConstants.h"
#include "Streets.h"

constexpr int hTextures = 13;
constexpr int lTextures = 10;

Buildings::Buildings()
{
    using namespace UIConstants::Streets;
    LoadTextures();

    Streets *streets = Streets::GetInstance();
    for (size_t i = 1; i < streets->horizStreets.size(); i++)
        for (size_t j = 1; j < streets->vertStreets.size(); j++) {
            buildings.push_back(GenerateBuilding(
                streets->vertStreets[j - 1]->modelMatrix[3].x + Streets::StreetHalfWidth(streets->vertStreets[j - 1]),
                streets->vertStreets[j]->modelMatrix[3].x - Streets::StreetHalfWidth(streets->vertStreets[j]),
                streets->horizStreets[i - 1]->modelMatrix[3].z + Streets::StreetHalfWidth(streets->horizStreets[i - 1]),
                streets->horizStreets[i]->modelMatrix[3].z - Streets::StreetHalfWidth(streets->horizStreets[i])));
        }
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

Building *Buildings::GenerateBuilding(float xmin, float xmax, float zmin, float zmax)
{
    using namespace UIConstants::Buildings;

    float height = MIN_HEIGHT + (rand() % 10 + 1) * (MAX_HEIGHT - MIN_HEIGHT) / 10;
    int numFacesType = rand() % 100;

    auto cyl = MeshBuilder::CreateCylinder(height, 1, ComputeNumberOfFaces(numFacesType), rand() % 2);
    Mesh *mesh = cyl.first;

    // pseudo-randomly choose texture function of the building's height
    Texture2D *tex = numFacesType >= 90 ? texture[0] : texture[(height >= 2) ? (rand() % hTextures + 1) : (rand() % lTextures + 21)];

    // place the building in the desired area
    float _x, x, _z, z;
    std::tie(_x, x, _z, z) = cyl.second;
    float scaleFactor = min((xmax - xmin) / (x - _x), (zmax - zmin) / (z - _z));
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(scaleFactor, 1, scaleFactor));
    glm::mat4 translateMat = glm::translate(glm::mat4(1), glm::vec3(xmin - _x * (xmax - xmin) / (x - _x), 0,
                                                                    zmin - _z * (zmax - zmin) / (z - _z)));
    glm::mat4 globalModelMat = translateMat * scaleMat;

    Building *building = new Building(globalModelMat);
    building->AddComp(mesh, tex, glm::mat4(1));

    // TODO assembly other primitives to the building's base


    return building;
}
