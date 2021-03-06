#include "Streets.h"

#include <cstdlib>
#include <ctime>
#include "Core/Managers/ResourcePath.h"
#include "UIConstants.h"
#include "MeshBuilder.h"
#include "include/math.h"

static Streets *instance; // singleton instance

Streets *Streets::GetInstance()
{
    if (instance == nullptr)
        instance = new Streets();

    return instance;
}

float Streets::StreetHalfWidth(Street *street)
{
    if (street->type == BLVD)
        return UIConstants::Streets::BLVD_WIDTH / 2;
    return UIConstants::Streets::STR_WIDTH / 2;
}

std::tuple<float, float, float, float> Streets::ZoneBetweenStreets(int v1, int v2, int h1, int h2)
{
    return make_tuple(
        vertStreets[v1]->modelMatrix[3].x + StreetHalfWidth(vertStreets[v1]),
        vertStreets[v2]->modelMatrix[3].x - StreetHalfWidth(vertStreets[v2]),
        horizStreets[h1]->modelMatrix[3].z + StreetHalfWidth(horizStreets[h1]),
        horizStreets[h2]->modelMatrix[3].z - StreetHalfWidth(horizStreets[h2]));
}

/** Generate the streets network */
Streets::Streets()
{
    srand(time(NULL));

    // load street textures
    texture[0] = new Texture2D();
    texture[0]->Load2D((RESOURCE_PATH::TEXTURES + "road2.png").c_str(), GL_REPEAT);

    texture[1] = new Texture2D();
    texture[1]->Load2D((RESOURCE_PATH::TEXTURES + "road.png").c_str(), GL_REPEAT);

    vertMesh[0] = MeshBuilder::CreateVertRect(glm::vec3(0), 2 * UIConstants::Map::MAX, UIConstants::Streets::BLVD_WIDTH);
    vertMesh[1] = MeshBuilder::CreateVertRect(glm::vec3(0), 2 * UIConstants::Map::MAX, UIConstants::Streets::STR_WIDTH);

    // generate vertical streets
    float lastx = UIConstants::Map::MIN;
    while (lastx <= UIConstants::Map::MAX) {
        lastx = lastx + (rand() % 4 + 2) * UIConstants::Streets::MIN_DIST;
        if (lastx > UIConstants::Map::MAX) break;

        int type = min(rand() % 4, 1);
        vertStreets.push_back(new Street(static_cast<StreetType>(type), vertMesh[type],
                                         translate(glm::mat4(1), glm::vec3(lastx, 0, 0))));
    }

    // generate horizontal streets
    float lastz = UIConstants::Map::MIN;
    while (lastz <= UIConstants::Map::MAX) {
        lastz = lastz + (rand() % 4 + 2) * UIConstants::Streets::MIN_DIST;
        if (lastz > UIConstants::Map::MAX) break;

        int type = min(rand() % 4, 1);
        horizStreets.push_back(new Street(static_cast<StreetType>(type), vertMesh[type],
                                          translate(glm::rotate(glm::mat4(1), RADIANS(90), glm::vec3(0, 1, 0)),
                                                    glm::vec3(-lastz, 0.005f, 0))));
    }

    InitCrossroads();
}

Streets::~Streets() {}

void Streets::InitCrossroads()
{
    // create crossroad mesh
    crossroad = MeshBuilder::CreateRect(glm::vec3(0, 0.007f, 0), 1, 1, glm::vec3(0));

    crossroadTex = new Texture2D();
    crossroadTex->Load2D((RESOURCE_PATH::TEXTURES + "crossroad.png").c_str(), GL_REPEAT);

    // compute the model matrices for crossroads
    for (auto hStreet : horizStreets)
        for (auto vStreet : vertStreets) {
            crossroadTransf.push_back(glm::scale(
                glm::translate(glm::mat4(1), glm::vec3(vStreet->modelMatrix[3].x, 0, hStreet->modelMatrix[3].z)),
                glm::vec3(UIConstants::Streets::STR_WIDTH, 1, UIConstants::Streets::STR_WIDTH)));
        }
}
