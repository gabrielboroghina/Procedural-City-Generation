#pragma once

#include <vector>
#include "Core/GPU/Mesh.h"

class Trees
{
public:
    Mesh *mesh;
    std::vector<glm::mat4> modelMatrices;

    Trees();
    ~Trees();
};
