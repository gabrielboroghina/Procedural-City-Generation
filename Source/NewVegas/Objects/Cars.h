#pragma once

#include "Core/GPU/Mesh.h"

class Cars
{
public:
    std::vector<Mesh *> meshes;
    std::vector<std::pair<Mesh*, glm::mat4>> instances;

    Cars();
    ~Cars();
    void GenerateCar(float x, float z, int side);
};
