#include "Streets.h"

#include <cstdlib>
#include <ctime>
#include "Core/Managers/ResourcePath.h"
#include "UIConstants.h"
#include "MeshBuilder.h"
#include "include/math.h"

static Streets *instance;

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

/** Generate the streets network */
Streets::Streets()
{
    srand(time(NULL));

    // load street textures
    texture[0] = new Texture2D();
    texture[0]->Load2D((RESOURCE_PATH::TEXTURES + "street_side2.png").c_str(), GL_REPEAT);

    texture[1] = new Texture2D();
    texture[1]->Load2D((RESOURCE_PATH::TEXTURES + "street2.png").c_str(), GL_REPEAT);

    vertMesh[0] = MeshBuilder::CreateVertRect(glm::vec3(0), 2 * UIConstants::Map::MAX, UIConstants::Streets::BLVD_WIDTH);
    vertMesh[1] = MeshBuilder::CreateVertRect(glm::vec3(0), 2 * UIConstants::Map::MAX, UIConstants::Streets::STR_WIDTH);

    // generate vertical streets
    float lastx = UIConstants::Map::MIN;

    while (lastx <= UIConstants::Map::MAX) {
        lastx = lastx + (rand() % 4 + 2) * UIConstants::Streets::MIN_DIST;
        if (lastx > UIConstants::Map::MAX) break;

        int type = rand() % 2;
        vertStreets.push_back(new Street(static_cast<StreetType>(type), vertMesh[type],
                                         glm::translate(glm::mat4(1), glm::vec3(lastx, 0, 0))));
    }

    // generate horizontal streets
    float lastz = UIConstants::Map::MIN;

    while (lastz <= UIConstants::Map::MAX) {
        lastz = lastz + (rand() % 4 + 2) * UIConstants::Streets::MIN_DIST;
        if (lastz > UIConstants::Map::MAX) break;

        int type = rand() % 4;
        if (type != 0) type = 1;
        horizStreets.push_back(new Street(static_cast<StreetType>(type), vertMesh[type],
                                          glm::translate(glm::rotate(glm::mat4(1), RADIANS(90), glm::vec3(0, 1, 0)),
                                                         glm::vec3(-lastz, 0.01f, 0))));
    }
}

Streets::~Streets() {}
