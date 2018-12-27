#include "Trees.h"

#include "Core/Managers/ResourcePath.h"
#include "NewVegas/Streets.h"
#include "NewVegas/UIConstants.h"

Trees::Trees()
{
    mesh = new Mesh("tree");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tree", "tree.obj");

    Streets *streets = Streets::GetInstance();
    float offset = UIConstants::Streets::STR_WIDTH / 2 + 0.03f;
    for (auto hStreet : streets->horizStreets)
        for (float x = UIConstants::Map::MIN + 0.3; x <= UIConstants::Map::MAX - 0.3; x += UIConstants::Trees::DIST) {
            int side = rand() % 3 - 1;

            for (auto vStreet : streets->vertStreets)
                if (abs(x - vStreet->modelMatrix[3].x) < UIConstants::Streets::STR_WIDTH) {
                    // don't plant a tree in the middle of a vertical street
                    side = 0;
                    break;
                }

            if (side)
                modelMatrices.push_back(glm::scale(
                    glm::translate(glm::mat4(1), glm::vec3(x, 0, hStreet->modelMatrix[3].z + side * offset)),
                    glm::vec3(0.07f, 0.05f, 0.07f)));
        }
}

Trees::~Trees() {}
