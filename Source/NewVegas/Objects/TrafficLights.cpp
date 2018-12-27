#include "TrafficLights.h"

#include "NewVegas/UIConstants.h"
#include "NewVegas/Streets.h"
#include "Core/Managers/ResourcePath.h"

TrafficLights::TrafficLights()
{
    mesh = new Mesh("semaphore");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Trafficlight", "traffic_light.obj");

    Streets *streets = Streets::GetInstance();
    float offset = UIConstants::Streets::STR_WIDTH / 2 + 0.03f;
    for (auto hStreet : streets->horizStreets)
        for (auto vStreet : streets->vertStreets) {
            int side = rand() % 3 - 1;

            if (side)
                modelMatrices.push_back(glm::scale(
                    glm::translate(glm::mat4(1), glm::vec3(vStreet->modelMatrix[3].x + offset, 0, hStreet->modelMatrix[3].z + offset)),
                    glm::vec3(0.001f, 0.001f, 0.001f)));
        }
}

TrafficLights::~TrafficLights() {}
