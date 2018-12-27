#pragma once

#include <vector>
#include "Core/GPU/Mesh.h"

class TrafficLights
{
public:
    Mesh *mesh;
    std::vector<glm::mat4> modelMatrices;

    TrafficLights();
    ~TrafficLights();
};
